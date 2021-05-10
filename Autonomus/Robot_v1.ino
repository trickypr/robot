// This is the sketch for my robot in IT

// These includes aren't necessary for the compiler to work, but they stop vscode
// intelisense from having a fit
#include "Distance.h"
#include "Motors.h"
#include "Numbers.h"
#include "IR.h"

#define STOP_DISTANCE 20

enum states
{
  straight,
  left,
  right,
  reverse,
};

bool motorRunning = true;
bool motorWasRunning = false;
bool pauseJustPressed = false;
bool overrideDist = false;
byte stateResetCounter = 0;
uint8 speed = percent(50);
byte state = straight;

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
    Serial.println("Checking distance stuff");

    // Stop the motor and make a note of the distance
    motorRunning = false;
    motorsStop();
    float straightDistance = distance;

    // Turn left and get the left distance
    motorsTurnLeftSpeed(200);
    delay(500);
    motorsStop();
    float leftDistance = getDistance();
    delay(100);

    Serial.print("Left distance: ");
    Serial.println(leftDistance);

    // Return to the straight distance
    motorsTurnRightSpeed(200);
    delay(500);
    motorsStop();
    delay(100);

    // Turn right and get the right distance
    motorsTurnRightSpeed(200);
    delay(500);
    motorsStop();
    float rightDistance = getDistance();

    Serial.print("Right distance: ");
    Serial.println(rightDistance);

    // Now we can perform some logic based on this to determine the movements
    // we should take
    if (leftDistance > straightDistance) {
      motorsTurnLeftSpeed(200);
      delay(500);
      motorsStop();
      motorRunning = true;
    } else if (rightDistance > straightDistance) {
      motorsTurnRightSpeed(200);
      delay(500);
      motorsStop();
      motorRunning = true;
    } else {
      motorsTurnRightSpeed(200);
      delay(500);
      motorsStop();
      return;
    }
  }

  // All of the logic to handle changes. This is out here because that means
  // I dont have to deal with every edge case. It all just happens at once

  // Make the robot move
  if (motorRunning)
  {
    if (state == left)
    {
      motorsLeftSpeed(200, false);
      motorsRightSpeed(200, true);
    }
    else if (state == right)
    {
      motorsRightSpeed(200, false);
      motorsLeftSpeed(200, true);
    }
    else if (state == reverse) 
    {
      motorsReverseSpeed(speed);
    }
    else
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
    }

    motorWasRunning = true;
  }
  else
  {
    motorsStop();
    motorWasRunning = false;
  }

  // Lets free up the arduino for a period of time to save on battery power
  delay(10);
}
