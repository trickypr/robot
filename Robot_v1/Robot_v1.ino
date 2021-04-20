// This is the sketch for my robot in IT

// These includes aren't necessary for the compiler to work, but they stop vscode
// intelisense from having a fit
#include "Distance.h"
#include "Motors.h"
#include "Numbers.h"
#include "IR.h"

enum states
{
  straight,
  left,
  right
};

bool motorRunning = true;
bool motorWasRunning = false;
bool pauseJustPressed = false;
bool reversing = false;
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
  // The main code for the robot
  // If remote input has been received
  if (IRHasReceivedCommand())
  {
    // Retrive the data that the remote has sent
    IRData data = IRGetData();

    // If we get command 0x15, the ⏵︎ button is pressed
    if (data.command == 0x15 && !pauseJustPressed)
    {
      // Invert the motor running var
      motorRunning = !motorRunning;
      pauseJustPressed = true;
    }
    else
    {
      pauseJustPressed = false;
    }

    // If we get command 0x09, the ⏭︎ is pressed
    if (data.command == 0x09)
    {
      // Increment the offset
      state = left;
    }
    else if (data.command == 0x7) // If we get command 0x7, the ⏮︎ was pressed
    {
      state = right;
    }
    else // Otherwise, revert to straight
    {
      state = straight;
    }

    // If we get command 0x40, the + is pressed
    if (data.command == 0x40)
    {
      speed += 1;
    }

    // If we get command 0x19, the - is pressed
    if (data.command == 0x19)
    {
      speed -= 1;
    }

    if (Serial)
    {
      Serial.print("Running: ");
      Serial.print(motorRunning);
      Serial.print(", State: ");
      Serial.print(state);
      Serial.print(", Speed: ");
      Serial.println(speed);
    }
  }

  // Now, lets check if we have got too close to anything. We can change the motor
  // running state if we have, to correct the course of the robot.
  //
  // TODO: Move distance to intergers, as they are more performant than floats
  float distance = getDistance();

  if (distance < 20 && !reversing)
  {
    motorRunning = false;
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
