#include "MHZ19.h"

#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)
#define GREEN 2
#define YELLOW 3
#define RED 4
#define BUZZER 5

MHZ19 myMHZ19;                                             // Constructor for library


unsigned long getDataTimer = 0;
unsigned long getBuzzerTimer = 0;

void setup()
{
  Serial.begin(9600);                                     // Device to serial monitor feedback
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  Serial1.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start
  myMHZ19.begin(Serial1);                                // *Serial(Stream) refence must be passed to library begin().
  myMHZ19.autoCalibration();    // Turn auto calibration ON (OFF autoCalibration(false))
  testComponents();
}

void loop()
{
  if (millis() - getDataTimer >= 2000)
  {
    int CO2;

    /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even
      if below background CO2 levels or above range (useful to validate sensor). You can use the
      usual documented command with getCO2(false) */

    CO2 = myMHZ19.getCO2();                             // Request CO2 (as ppm)
    if ( CO2 < 1000)
    {
      green();
    }
    if ( 999 < CO2 && CO2 < 2001)
    {
      yellow();
      if ( millis() - getBuzzerTimer >= 10000 ) // buzz every 10s
      {
        buzz();
      }

    }

    if ( CO2 > 2000)
    {
      red();
      // Serial.print("rot");
      if ( millis() - getBuzzerTimer >= 1000 ) // buzz every 1s
      {
        buzz();
      }

    }
    // Serial.print("CO2 (ppm): ");
    //  Serial.println(CO2);

    //int8_t Temp;
    //Temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
    //Serial.print("Temperature (C): ");
    //Serial.println(Temp);

    getDataTimer = millis();
  }
  if (millis() - getBuzzerTimer >= 12000)
  {
    getBuzzerTimer = millis();
  }
}

void green()
{
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
}
void yellow()
{
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, LOW);
}
void red()
{
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
}
void allColors()
{
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, HIGH);
}
void noColors()
{
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
}
void buzz() {
  tone(BUZZER, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(BUZZER);     // Stop sound...
}
void testComponents()
{
// Testing sequence (LEDs, Buzzer)
  getDataTimer = millis();
  while (millis()-getDataTimer < 20000)
  {
  red();
  delay(500);
  noColors();
  delay(500);
  }
  getDataTimer = millis();
  while (millis()-getDataTimer < 20000)
  {
  yellow();
  delay(500);
  noColors();
  delay(500);
  }
  getDataTimer = millis();
  while (millis()-getDataTimer < 20000)
  {
  green();
  delay(500);
  noColors();
  delay(500);
  }
  allColors();  
  buzz();
  noColors();
  delay(500);
  allColors();
  buzz();
  noColors();
  delay(500);
  // End of testing sequence
}
