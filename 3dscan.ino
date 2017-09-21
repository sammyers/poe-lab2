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

int xStart;
int zStart;

String line = "";

void setup() {
  zServo.attach(9);  // attaches the servo on pin 9 to the servo object
  xServo.attach(10);

  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);

  // xStart = xServo.read() - 50;
  // zStart = zServo.read() - ;

  xServo.write(0);
  zServo.write(0);
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
  int zSweepStart;
  int zSweepEnd;
  int increment;

  for (xRotation = xStart; xRotation <= xStart + 90; xRotation ++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    xServo.write(xRotation);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position

    if (xRotation % 2 == 0) {
      zSweepStart = zStart;
      zSweepEnd = zStart + 90;
      increment = 1;
    } else {
      zSweepStart = zStart + 90;
      zSweepEnd = zStart;
      increment = -1;
    }
    for (zRotation = zSweepStart; zRotation <= zSweepEnd; zRotation += increment) {
      zServo.write(zRotation);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      recordDistance(xRotation - xStart, zRotation - zStart, readAverageDistance());
    }
  }
}

void loop() {
  // long distance = readAverageDistance();
  // recordDistance(1, 2, distance);
  // delay(1000);
  if (!isScanning) {
    scanSurface();
    isScanning = true;
  }
}