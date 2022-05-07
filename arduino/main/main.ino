#include <math.h>
// Define Thermistor pin
#define thermistor_output A1

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
double temp = 0;

float containerHeight = 11;
float conatinerLevel = 0;
int drainLevel = 0;

void setup()
{
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

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
}

bool isOn = false;

void loop()
{

  if (!isOn)
  {
    /// CONTAINER LEVEL MEASUREMENT
    conatinerLevel = calcDistance();
    while (conatinerLevel <= 100)
    {
      warinig("Dialysis Fluid Container isn't full, please add dialysis fluid");

      /// CONTAINER LEVEL MEASUREMENT
      conatinerLevel = calcDistance();
    }

    status("Dialysis Fluid Container is full");
    /// TEMP MEASUREMENT
    temp = getTemp();
    while (temp < 42)
    {
      warinig("Temp < 42, heater is working now");
      digitalWrite(heater, LOW);

      /// TEMP MEASUREMENT
      temp = getTemp();
    }
    digitalWrite(heater, HIGH);

    status("Temperature is ok");

    /// PH MEASUREMENT
    ph = ColorReadValue(true);

    while (ph > 8 || ph < 6)
    {
      if (ph > 8)
      {
        warinig("PH > 8, dialysis is not possible");
      }
      else if (ph < 6)
      {
        warinig("PH < 6, dialysis is not possible");
      }

      /// PH MEASUREMENT
      ph = ColorReadValue(true);
    }

    status("PH is ok");
    status("NOW YOU CAN DIALYSE, process is started");
    delay(1000);

    isOn = true;

    digitalWrite(pump, HIGH);
  }

  /// CONTAINER LEVEL MEASUREMENT
  conatinerLevel = calcDistance();

  while (conatinerLevel > 95)
  {
    warinig("Dialysis Fluid Container needs to be refilled");

    /// Stop Pump
    digitalWrite(pump, LOW);

    /// Rechecking conditions before pump restart
    isOn = false;

    /// CONTAINER LEVEL MEASUREMENT
    conatinerLevel = calcDistance();
  }

  status("Good, Dialysis Fluid Container is refilled Successfully");

  /// BLOOD LEAKAGE TEST
  DialysisColor = ColorReadValue(false);
  while (DialysisColor > 700)
  {
    warinig("There is a Blood Leakage, Check the dialysis fluid");

    /// Stop Pump
    digitalWrite(pump, LOW);

    /// Rechecking conditions before pump restart
    isOn = false;

    /// CONTAINER LEVEL MEASUREMENT
    conatinerLevel = calcDistance();
  }
  status("Good, Dialysis Fluid is now pure");
  /// DRAIN CONTAINER LEVEL MEASUREMENT
  drainLevel = drainLevelCalc();
  while (drainLevel > 95)
  {
    warinig("Drain if full, please empty it");

    /// Stop Pump
    digitalWrite(pump, LOW);

    /// Rechecking conditions before pump restart
    isOn = false;

    /// CONTAINER LEVEL MEASUREMENT
    conatinerLevel = calcDistance();
  }
  status("Good, Drain Container is now empty");

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
  delay(100);

  // Read Green Frequency
  greenFR = getGreenFR(pin);
  // Delay to stabilize sensor
  delay(100);

  // Read Blue Frequency
  blueFR = getBlueFR(pin);
  // Delay to stabilize sensor
  delay(100);

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
  }
  // Print output to Serial Monitor
  //  Serial.print("Red FR = ");
  //  Serial.print(redFR);
  //  Serial.print(" - Green FR = ");
  //  Serial.print(greenFR);
  //  Serial.print(" - Blue FR = ");
  //  Serial.println(blueFR);
  //  Serial.println();

  return value;
}

/////////////////////////// Temp Measuerment ///////////////////////////
double getTemp()
{
  int thermistor_adc_val;
  double output_voltage, thermistor_resistance, therm_res_ln, temperature;
  thermistor_adc_val = analogRead(thermistor_output);
  output_voltage = ((thermistor_adc_val * 5.0) / 1023.0);
  thermistor_resistance = ((5 * (10.0 / output_voltage)) - 10); /* Resistance in kilo ohms */
  thermistor_resistance = thermistor_resistance * 1000;         /* Resistance in ohms   */
  therm_res_ln = log(thermistor_resistance);
  temperature = (1 / (0.001129148 + (0.000234125 * therm_res_ln) + (0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln))); /* Temperature in Kelvin */
  temperature = temperature - 273.15;                                                                                                /* Temperature in degree Celsius */

  return temperature;
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
  return value;
}

void warinig(String msg)
{
  Serial.print("Warning: ");
  Serial.println(msg);
  Serial.print(" !!!");
}

void status(String msg)
{
  Serial.print("Status: ");
  Serial.println(msg);
  Serial.print(" ...");
}

/// Container: 3.5 cm ///
/// Dialyste Normal 550-700
/// PH Normal 7 [6-8]
/// Drain Level Normal: