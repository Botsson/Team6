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

const int MAX_SPEED = 500;

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
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();
    delay(20);
  }

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

if(count >= 2600)
{
  motors.setSpeeds(700, 700);
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

//  if (m1Speed == 0 && m2Speed == MAX_SPEED)
//  {
//    motors.setSpeeds(500, 500);
//  } else if (m2Speed == 0 && m1Speed == MAX_SPEED) {
//    motors.setSpeeds(500, 500);
//  } else 
    motors.setSpeeds(m1Speed, m2Speed);
}

