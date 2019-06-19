/* GROUND CONTROL v2
 *  Adding an I2C device, the ADXL343, to get I2C up and running.  We 
 *  want to use it eventually, anyway, and this way I don't have to 
 *  have a servo working. 
 *  Done. Reports m/s^2.  
 *  
 *  GROUND CONTROL v1
 *  reads joystick x (rudd) and y (divP), converts to tenths of deg and reports
 */

/* MASTER READER CODE:
 *  Should be set, include-wise, to do I2C with a servo
 *  
 *  Want Ground Control to request, and then read, 3 bytes of data sent from the 
 *  uniquely addressed Slave Servo (2 bytes for the angle to 0.1 degree, one byte 
 *  for Status).
 *  
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);

 int ledPin = 13;
 int joyPinX = 8;                 // slider variable connecetd to analog pin
 int joyPinY = 9;                 // slider variable connecetd to analog pin
 int rudd = 0;                   // 
 int divP= 0;                   // 

//****************************************************************************
int XtenthsOfDegrees(int data) {
  int ang;
  ang = ((data * 5 / 4) - 950);
  if (ang < -300) ang = -300;
  if (ang > 300) ang = 300;
  return (ang);
}
//****************************************************************************
int YtenthsOfDegrees(int data) {
  int ang;
  ang = ((data * 5 / 4) - 971);
  if (ang < -300) ang = -300;
  if (ang > 300) ang = 300;
  return (ang);
}
//****************************************************************************
//****************************************************************************
void setup() 
{ Serial.begin(9600);
  while (!Serial);
  
  /* Initialise the sensor */
  if(!accel.begin())
  { /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL343_RANGE_2_G); // Set range to +/- 2g
  Serial.println("Accelerometer intialized ok"); Serial.println("");
  
  Wire.begin();        // join i2c bus (address optional for master)
                       // Is this necessary if I2C has already been initialized for the ADXL343??
                       
  pinMode(ledPin, OUTPUT);              // initializes digital pin 13 as output
}
  

//****************************************************************************
void loop() 
{ /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("UpTilt: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("RollRight: "); Serial.print(event.acceleration.y); Serial.print("  ");Serial.print("m/s^2   ");
  delay(500);
 
  rudd = XtenthsOfDegrees(analogRead(joyPinX));
  delay(100);             
  divP = YtenthsOfDegrees(analogRead(joyPinY));

/*
  digitalWrite(ledPin, HIGH);           
  delay(222);
  digitalWrite(ledPin, LOW);
  delay(222);
 
  Serial.print ("joyPinX = ");
  Serial.print (analogRead(joyPinX));
  Serial.print ("  joyPinY = ");
  Serial.print (analogRead(joyPinY));
 */ 
 
  Serial.print ("rudd = ");
  Serial.print (rudd);
  Serial.print (".  divP = ");
  Serial.print (divP); 
  Serial.println (".");
 }

 
