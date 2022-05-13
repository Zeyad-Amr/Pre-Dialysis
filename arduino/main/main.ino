#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

// Define Thermistor pin
#define ONE_WIRE_BUS 10

// Define Thermistor pin
#define levelLED A1

// Define Thermistor pin
#define PHLED A2

// Define Thermistor pin
#define drainLED A3

// Define Thermistor pin
#define leackageLED A4

// Define Water Level Sensor pin
#define drainPin A0

// Define Pump pin
#define pump 3

// Define Heater pin
#define heater 11

// Define Water Level Sensor pin
#define drainPin A0

// Define Ultrasonic pins
#define echo 12
#define trig 13

// Define color sensor pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOutPH 8
#define sensorOutBD 9

// Variables for Color Pulse Width Measurements
int ph = 0;
int DialysisColor = 0;
int temp = 0;

float containerHeight = 11;
int conatinerLevel = 0;
int drainLevel = 0;
String x = "*";
bool isOn = false;
int status = 0;

// for DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius = 0;
float Fahrenheit = 0;

void setup()
{
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(levelLED, OUTPUT);
  pinMode(PHLED, OUTPUT);
  pinMode(drainLED, OUTPUT);
  pinMode(leackageLED, OUTPUT);

  digitalWrite(levelLED, LOW);
  digitalWrite(PHLED, LOW);
  digitalWrite(drainLED, LOW);
  digitalWrite(leackageLED, LOW);

  // Set Pulse Width scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Set Sensor output as input
  pinMode(sensorOutPH, INPUT);
  pinMode(sensorOutBD, INPUT);

  // Set Heater Relay as outputs
  pinMode(heater, OUTPUT);
  digitalWrite(heater, HIGH);

  // Set Pump Relay as outputs
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  // Set ULTRASONIC SENSOR
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Setup Serial Monitor
  Serial.begin(9600);
  // DallasTemperature begin
  sensors.begin();
}

void loop()
{
  if (!isOn)
  {
    /// CONTAINER LEVEL MEASUREMENT
    conatinerLevel = calcDistance();
    drainLevel = drainLevelCalc();
    temp = getTemp();
    while (conatinerLevel <= 100)
    {

      /// CONTAINER LEVEL MEASUREMENT
      conatinerLevel = calcDistance();
      digitalWrite(levelLED, HIGH);
      status = 3;
      String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
      Serial.print(z);
      delay(500);
    }
    digitalWrite(levelLED, LOW);

    /// TEMP MEASUREMENT
    temp = getTemp();
    while (temp < 38)
    {

      digitalWrite(heater, LOW);

      /// TEMP MEASUREMENT
      temp = getTemp();
      status = 1;
      String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
      Serial.print(z);
      delay(500);
    }
    digitalWrite(heater, HIGH);

    /// PH MEASUREMENT
    ph = ColorReadValue(true);

    while (ph > 8 || ph < 6)
    {

      if (ph > 8)
      {
        //        warinig("PH > 8, dialysis is not possible: ");
        digitalWrite(PHLED, HIGH);
      }
      else if (ph < 6)
      {
        //        warinig("PH < 6, dialysis is not possible: ");
        digitalWrite(PHLED, HIGH);
      }
      //      Serial.println(ph);
      /// PH MEASUREMENT
      ph = ColorReadValue(true);
      status = 5;
      String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
      Serial.print(z);
      delay(500);
    }

    // status("PH is ok: ");
    digitalWrite(PHLED, LOW);
    // Serial.println(ph);

    // status("NOW YOU CAN DIALYSE, process is started");
    status = 0;
    String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
    Serial.print(z);
    delay(1000);

    isOn = true;

    digitalWrite(pump, HIGH);
  }

  /// CONTAINER LEVEL MEASUREMENT
  conatinerLevel = calcDistance();
  if (conatinerLevel < 30)
  {
    while (conatinerLevel <= 100)
    {

      digitalWrite(levelLED, HIGH);
      /// Stop Pump
      digitalWrite(pump, LOW);

      /// Rechecking conditions before pump restart
      isOn = false;

      /// CONTAINER LEVEL MEASUREMENT
      conatinerLevel = calcDistance();
      status = 3;
      String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
      Serial.print(z);
    }
  }

  status = 0;
  String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
  Serial.print(z);

  // status("Good, Dialysis Fluid Container Level is ok: ");
  digitalWrite(levelLED, LOW);
  // Serial.println(conatinerLevel);
  //
  /// BLOOD LEAKAGE TEST
  DialysisColor = ColorReadValue(false);
  while (DialysisColor > 700)
  {

    /// Stop Pump
    digitalWrite(pump, LOW);

    /// Rechecking conditions before pump restart
    isOn = false;

    /// CONTAINER LEVEL MEASUREMENT
    DialysisColor = ColorReadValue(false);
    status = 2;
    String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
    Serial.print(z);
  }
  //    status("Good, Dialysis Fluid is pure: ");
  //  Serial.print("DialysisColor= ");
  //  Serial.println(DialysisColor);

  /// DRAIN CONTAINER LEVEL MEASUREMENT
  drainLevel = drainLevelCalc();
  status = 0;
  z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
  Serial.print(z);

  if (drainLevel > 100)
  {
    while (drainLevel > 10)
    {

      digitalWrite(drainLED, HIGH);

      /// Stop Pump
      digitalWrite(pump, LOW);

      /// Rechecking conditions before pump restart
      //      isOn = false;

      /// DRAIN CONTAINER LEVEL MEASUREMENT
      drainLevel = drainLevelCalc();
      status = 4;
      String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
      Serial.print(z);
    }
    if (drainLevel < 10)
    {
      digitalWrite(pump, HIGH);
      status = 0;
      String z = "@" + String(conatinerLevel) + x + String(drainLevel) + x + String(temp) + x + String(status) + "@";
      Serial.print(z);
    }
  }

  // status("Good, Drain Container is not full");
  digitalWrite(drainLED, LOW);
  // Serial.print("drainLevel= ");
  // Serial.println(drainLevel);

  // Serial.print("End of LOOP");
  delay(1000);
}
/// End of void Loop ///

/////////////////////////// COLOR SENSOR IMPLEMETATION ///////////////////////////

// Function to read Red BD Frequency
int getRedFR(int pin)
{
  // Set sensor to read Red only
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Define integer to represent BD Frequency
  int rFR;
  // Read the output Pulse Width
  rFR = pulseIn(pin, LOW);
  // Return the value
  return rFR;
}

// Function to read Green BD Frequency
int getGreenFR(int pin)
{
  // Set sensor to read Green only
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Define integer to represent BD Frequency
  int gFR;
  // Read the output Pulse Width
  gFR = pulseIn(pin, LOW);
  // Return the value
  return gFR;
}

// Function to read Blue BD Frequency
int getBlueFR(int pin)
{
  // Set sensor to read Blue only
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Define integer to represent BD Frequency
  int bFR;
  // Read the output Pulse Width
  bFR = pulseIn(pin, LOW);
  // Return the value
  return bFR;
}

/// Color Sensors Read values ///
int ColorReadValue(bool isPh)
{
  int pin;
  int value = 0;

  int redFR = 0;
  int greenFR = 0;
  int blueFR = 0;

  if (isPh)
  {
    pin = sensorOutPH;
  }
  else
  {
    pin = sensorOutBD;
  }

  // Read Red Frequency
  redFR = getRedFR(pin);
  // Delay to stabilize sensor
  delay(10);

  // Read Green Frequency
  greenFR = getGreenFR(pin);
  // Delay to stabilize sensor
  delay(10);

  // Read Blue Frequency
  blueFR = getBlueFR(pin);
  // Delay to stabilize sensor
  delay(10);

  if (isPh)
  {
    float pH_r, pH_g, pH_b;

    // pH Function
    pH_r = 0.0000001216 * redFR * redFR * redFR - 0.0001876 * redFR * redFR + 0.1009 * redFR - 11.62;                 // redPW
    pH_g = 0.000000007332 * greenFR * greenFR * greenFR - 0.00001259 * greenFR * greenFR + 0.01022 * greenFR + 4.676; // greenPW
    pH_b = 0.000000005716 * blueFR * blueFR * blueFR - 0.00001896 * blueFR * blueFR + 0.02183 * blueFR - 1.428;       // bluePW

    value = ((pH_r + pH_g + pH_b) * 10 / 3) + 17;
  }
  else
  {
    value = (100 * redFR + 10 * greenFR + blueFR) / 10;
    Serial.print("RED =");
    Serial.println(redFR);
    //    Serial.print("GREEN =");
    //    Serial.println(greenFR);
    //    Serial.print("BLUE =");
    //    Serial.println(blueFR);
  }

  return value;
}

/////////////////////////// Temp Measuerment ///////////////////////////
double getTemp()
{
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  return Celcius;
}

/////////////////////////// CONTAINER LEVEL MEASUREMENT ///////////////////////////
float calcDistance()
{
  float duration;
  float dis;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  dis = (duration / 2) * 0.0343;
  return (containerHeight - dis) / (containerHeight - 3.5) * 100;
}

/////////////////////////// DRAIN CONTAINER LEVEL MEASUREMENT ///////////////////////////
int drainLevelCalc()
{
  int value = analogRead(drainPin);
  int percentage = (value / 10) + 35;
  if (percentage > 50)
  {
    return percentage * 1.15;
  }
  else
  {
    return 0;
  }
}

// void warinig(String msg)
//{
//   Serial.print("Warning: ");
//   Serial.println(msg);
//   Serial.print(" !!!");
//   delay(500);
// }
//
// void status(String msg)
//{
//   Serial.print("Status: ");
//   Serial.println(msg);
//   Serial.print(" ...");
//   delay(500);
// }

/// Container: 3.5 cm ///
/// Dialyste Normal 550-700
/// PH Normal 7 [6-8]
/// Drain Level Normal:
