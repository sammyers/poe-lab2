/*
 */

#include <Servo.h>
#include <math.h>

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

String line = "";

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
  return map(analogInput, 490, 230, 20, 50);
}

long readAverageDistance() {
  long totalReading = 0;
  for (int i = 0; i < 5; i++) {
    totalReading += analogRead(SENSOR_PIN);
  }
  return mapDistance(totalReading / 5);
}

// write a triplet of values to the serial port
void recordDistance(int xPos, int zPos, long distance) {
  line = line + xPos + ", " + zPos + ", " + distance;
  Serial.println(line);
  line = "";
}

void scanSurface() {
  for (xRotation = 0; xRotation <= 180; xRotation ++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    xServo.write(xRotation);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position

    for (zRotation = 0; zRotation <= 180; zRotation ++) {
      zServo.write(zRotation);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      recordDistance(xRotation, zRotation, readAverageDistance());
    }
  }
}

void loop() {
  long distance = readAverageDistance();
  recordDistance(1, 2, distance);
  delay(1000);
  // if (checkIfButtonPressed()) {
  //   isScanning = !isScanning;
  // }
  // if (isScanning) {
  //   scanSurface();
  // }
}