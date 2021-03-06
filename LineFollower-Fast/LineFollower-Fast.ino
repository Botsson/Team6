#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;
int count = 0;

int MAX_SPEED = 300;

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
  motors.setSpeeds(0,0);
}

int getDirection() {
  unsigned int sensors[6];
  int position = reflectanceSensors.readLine(sensors);
  int error = position - 2500;
  int speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  return speedDifference;
}

void loop() {
  count++;

  if(count >= 2600) {
    motors.setSpeeds(700, 700);
  } else if (count <= 10) {
    reflectanceSensors.calibrate();
  }

  if (count > 800) {
    MAX_SPEED = 500;
  }
  
  int lineDirection = getDirection();
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


