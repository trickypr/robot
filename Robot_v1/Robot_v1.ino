// This is the sketch for my robot in IT

// These includes aren't necessary for the compiler to work, but they stop vscode
// intelisense from having a fit
#include "Distance.h"
#include "Motors.h"
#include "Numbers.h"
#include "IR.h"

#define STOP_DISTANCE 20
#define REVERSE_DISTANCE 15
#define DISTANCE_DELAY 400
#define RESET_DELAY 1000

#define LEFT_OFFSET 0
#define RIGHT_OFFSET 10

#define MOTOR_SPEED 200

enum states
{
  navigate,
  map,
  idle,
};

bool motorRunning = true;
bool motorWasRunning = false;
bool lastRand = false;

uint8 timesLooped = 0;
uint8 speed = percent(50);
byte state = idle;

void setup()
{
  // Set up the serial monitor
  Serial.begin(9600);
  Serial.println("Arduino started");

  // Initialise the ultrasonic sensor
  distanceSetup();

  // Initialise motors
  motorsSetup();

  // Setup IR
  IRSetup();
}

void loop()
{
  // Now, lets check if we have got too close to anything. We can change the motor
  // running state if we have, to correct the course of the robot.
  //
  // TODO: Move distance to intergers, as they are more performant than floats
  float distance = getDistance();

  // Check if it is in the stop distance. Ignore if it is set to reverse.
  if (distance < STOP_DISTANCE)
  {
    timesLooped = 0;

    Serial.println("Checking distance stuff");

    if (distance < REVERSE_DISTANCE)
    {
      motorsReverseSpeed(MOTOR_SPEED);
      delay(DISTANCE_DELAY);
      motorsStop();
      delay(RESET_DELAY);
      motorsTurnLeftSpeed(MOTOR_SPEED);
      delay(DISTANCE_DELAY);
      motorsStop();
      delay(RESET_DELAY);
      return;
    }

    // Stop the motor and make a note of the distance
    motorRunning = false;
    motorsStop();
    delay(RESET_DELAY);
    float straightDistance = distance;

    // Turn left and get the left distance
    motorsTurnLeftSpeed(MOTOR_SPEED);
    delay(DISTANCE_DELAY + LEFT_OFFSET);
    motorsStop();
    float leftDistance = getDistance();
    delay(RESET_DELAY);

    Serial.print("Left distance: ");
    Serial.println(leftDistance);

    // Return to the straight distance
    motorsTurnRightSpeed(MOTOR_SPEED);
    delay(DISTANCE_DELAY + RIGHT_OFFSET);
    motorsStop();
    delay(RESET_DELAY);

    // Turn right and get the right distance
    motorsTurnRightSpeed(MOTOR_SPEED);
    delay(DISTANCE_DELAY + RIGHT_OFFSET);
    motorsStop();
    float rightDistance = getDistance();
    delay(RESET_DELAY);

    Serial.print("Right distance: ");
    Serial.println(rightDistance);

    if (rightDistance > leftDistance && rightDistance > straightDistance)
    {
      motorRunning = true;
    }
    else if (leftDistance > straightDistance)
    {
      motorsTurnLeftSpeed(MOTOR_SPEED);
      delay(DISTANCE_DELAY + LEFT_OFFSET);
      motorsStop();
      delay(RESET_DELAY);

      motorsTurnLeftSpeed(MOTOR_SPEED);
      delay(DISTANCE_DELAY + LEFT_OFFSET);
      motorsStop();
      delay(RESET_DELAY);

      motorRunning = true;
    }
    else
    {
      motorsReverseSpeed(MOTOR_SPEED);
      delay(DISTANCE_DELAY * 2);
      motorsTurnLeftSpeed(MOTOR_SPEED);
      delay(DISTANCE_DELAY);
      motorsStop();
      delay(RESET_DELAY);
      motorRunning = true;
      return;
    }
  }

  // All of the logic to handle changes. This is out here because that means
  // I dont have to deal with every edge case. It all just happens at once

  // Make the robot move
  if (motorRunning)
  {

    if (!motorWasRunning)
    {
      motorsLeftSpeed(speed, false);
      delay(200);
      motorsRightSpeed(speed, false);
    }
    else
    {
      motorsStraightSpeed(speed);
    }

    motorWasRunning = true;
  }
  else
  {
    motorsStop();
    motorWasRunning = false;
  }

  if (timesLooped > 250)
  {
    timesLooped = 0;
    Serial.println(timesLooped);
    motorsReverseSpeed(MOTOR_SPEED);
    delay(DISTANCE_DELAY);
    motorsStop();
    delay(RESET_DELAY);
    if (lastRand)
    {
      motorsTurnLeftSpeed(MOTOR_SPEED);
    }
    else
    {
      motorsTurnRightSpeed(MOTOR_SPEED);
    }
    delay(DISTANCE_DELAY);
    motorsStop();
    delay(RESET_DELAY);
  }

  // Lets free up the arduino for a period of time to save on battery power
  delay(20);
  timesLooped++;
  lastRand = !lastRand;
}
