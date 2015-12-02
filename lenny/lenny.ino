#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;
unsigned int sensors[6];

const int MAX_SPEED = 400;
const int MAX_SENSOR = 1000;
const int SENSOR_PIVOT = 500;

void lightsOn() {
  digitalWrite(13, HIGH);
}

void lightsOff() {
  digitalWrite(13, LOW);
}

void setup() {
  reflectanceSensors.init();
  button.waitForButton();
  pinMode(13, OUTPUT);
  lightsOn();
  reflectanceSensors.calibrate();
  motors.setSpeeds(0,0);
}

int getDirection() {
  int position = reflectanceSensors.readLine(sensors);
  int error = position - 2500;
  int speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  return speedDifference;
}

boolean onLine() {
  int i;
  boolean above = true;
  for(i = 0; i < 6; i++) {
    above &= (SENSOR_PIVOT < sensors[i])
  }
  return above;
}

void loop() {
  int lineDirection = getDirection();
  if (onLine()) {
    lineDirection = MAX_SPEED / 2;  
  }
  int m1Speed = MAX_SPEED + lineDirection;
  int m2Speed = MAX_SPEED - lineDirection;
  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > MAX_SPEED)
    m1Speed = MAX_SPEED;
  if (m2Speed > MAX_SPEED)
    m2Speed = MAX_SPEED;
  motors.setSpeeds(m1Speed, m2Speed);
}

