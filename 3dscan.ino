/*
 */

#include <Servo.h>

int SENSOR_PIN = A0;
int BUTTON_PIN = 8;

int pos;
Servo zServo;  // create servo object to control a servo
Servo xServo;

bool isButtonPressed = false;
bool isScanning = false;

// state variables to store servo position
int zRotation = 0;
int xRotation = 0;

void setup() {
  zServo.attach(9);  // attaches the servo on pin 9 to the servo object
  xServo.attach(10);

  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
}

bool checkIfButtonPressed() {
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState != isButtonPressed && buttonState) {
    isButtonPressed = buttonState;
    return true;
  }
  isButtonPressed = buttonState;
  return false;
}

// Returns the corresponding distance, in feet, for a given analog input value
long mapDistance(int analogInput) {
  return map(analogInput, 198, 42, 2, 10);
}

// Read the current distance from the sensor
long readDistance() {
  return mapDistance(analogRead(SENSOR_PIN));
}

// write a triplet of values to the serial port
void recordDistance(int xPos, int zPos, long distance) {
  
}

void scanSurface() {
  for (xRotation = 0; xRotation <= 180; xRotation ++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    xServo.write(xRotation);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position

    for (zRotation = 0; zRotation <= 180; zRotation ++) {
      zServo.write(zRotation);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      recordDistance(xRotation, zRotation, readDistance());
    }
  }
}

void loop() {
  if (checkIfButtonPressed()) {
    isScanning = !isScanning;
  }
  if (isScanning) {
    scanSurface();
  }
}
