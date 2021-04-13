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

bool motorRunning = false;
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
    if (data.command == 0x15 && IrReceiver.repeatCount == 0)
    {
      // Invert the motor running var
      motorRunning = !motorRunning;
    }

    // If we get command 0x09, the ⏭︎ is pressed
    if (data.command == 0x09)
    {
      // Increment the offset
      state = left;
    }

    // If we get command 0x7, the ⏮︎ was pressed
    if (data.command == 0x7)
    {
      state = right;
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
      Serial.print(motorRunning);
      Serial.print(", State: ");
      Serial.print(state);
      Serial.print(", Speed: ");
      Serial.println(speed);
    }
  }

  // All of the logic to handle changes. This is out here because that means
  // I dont have to deal with every edge case. It all just happens at once
  motorsStop();

  // Make the robot move
  if (motorRunning)
  {
    if (state == left)
    {
      motorsLeftSpeed(speed);
    }
    else if (state == right)
    {
      motorsRightSpeed(speed);
    }
    else
    {
      motorsStraightSpeed(speed);
    }
  }
  else
  {
    motorsStop();
  }
}
