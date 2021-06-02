#pragma once
#include "Types.h"

// =============================================================================
// Motor config - specifies all of the pins and other parameters that the L298N needs
#define MOTOR_OUT_1 12
#define MOTOR_OUT_2 7
#define MOTOR_ENABLE_1 9

#define MOTOR_OUT_3 11
#define MOTOR_OUT_4 8
#define MOTOR_ENABLE_2 10

// =============================================================================
// Motor logic

/**
 * Initialises the motor driver. MUST be called before using any of the motor functions
 */
inline void motorsSetup()
{
  // Initialise the pins required for the motor to run correctly
  pinMode(MOTOR_OUT_1, OUTPUT);
  pinMode(MOTOR_OUT_2, OUTPUT);
  pinMode(MOTOR_OUT_3, OUTPUT);
  pinMode(MOTOR_OUT_4, OUTPUT);
  pinMode(MOTOR_ENABLE_1, OUTPUT);
  pinMode(MOTOR_ENABLE_2, OUTPUT);
}

void motorsRightSpeed(uint8 speed, bool reverse)
{
  // Setup one side

  if (reverse)
  {
    digitalWrite(MOTOR_OUT_1, LOW);
    digitalWrite(MOTOR_OUT_2, HIGH);
  }
  else
  {
    digitalWrite(MOTOR_OUT_1, HIGH);
    digitalWrite(MOTOR_OUT_2, LOW);
  }

  // Enable side
  analogWrite(MOTOR_ENABLE_1, speed);
}

void motorsLeftSpeed(uint8 speed, bool reverse)
{
  // Setup one side
  if (reverse)
  {
    digitalWrite(MOTOR_OUT_3, LOW);
    digitalWrite(MOTOR_OUT_4, HIGH);
  }
  else
  {
    digitalWrite(MOTOR_OUT_3, HIGH);
    digitalWrite(MOTOR_OUT_4, LOW);
  }
  // Enable side
  analogWrite(MOTOR_ENABLE_2, speed);
}

/**
 * Go straight with a specified speed
 */
void motorsStraightSpeed(uint8 speed)
{
  // Setup one side
  digitalWrite(MOTOR_OUT_1, HIGH);
  digitalWrite(MOTOR_OUT_2, LOW);
  analogWrite(MOTOR_ENABLE_1, speed);
  // Setup the other side
  digitalWrite(MOTOR_OUT_3, HIGH);
  digitalWrite(MOTOR_OUT_4, LOW);
  analogWrite(MOTOR_ENABLE_2, speed);
}

/**
 * Go reverse with a specified speed
 */
void motorsReverseSpeed(uint8 speed)
{
  // Setup one side
  digitalWrite(MOTOR_OUT_1, LOW);
  digitalWrite(MOTOR_OUT_2, HIGH);
  analogWrite(MOTOR_ENABLE_1, speed);
  // Setup the other side
  digitalWrite(MOTOR_OUT_3, LOW);
  digitalWrite(MOTOR_OUT_4, HIGH);
  analogWrite(MOTOR_ENABLE_2, speed);
}

void motorsSpeedAndOffset(uint8 speed, int8 offset)
{
  // Setup one side
  digitalWrite(MOTOR_OUT_1, HIGH);
  digitalWrite(MOTOR_OUT_2, LOW);
  // Setup the other side
  digitalWrite(MOTOR_OUT_3, HIGH);
  digitalWrite(MOTOR_OUT_4, LOW);
  // Start them both at the same time
  if (offset > 0)
  {
    analogWrite(MOTOR_ENABLE_1, speed - offset);
    analogWrite(MOTOR_ENABLE_2, speed);
  }
  else
  {
    analogWrite(MOTOR_ENABLE_1, speed);
    analogWrite(MOTOR_ENABLE_2, speed - offset);
  }
}

/**
 * Go straight
 */
void motorsStraight()
{
  motorsStraightSpeed(255);
}

/**
 * Stop robot
 */
void motorsStop()
{
  // Stop the motors
  analogWrite(MOTOR_ENABLE_1, 0);
  analogWrite(MOTOR_ENABLE_2, 0);
}

void motorsTurnLeftSpeed(uint8 speed) {
  motorsLeftSpeed(speed, false);
  motorsRightSpeed(speed, true);
}

void motorsTurnRightSpeed(uint8 speed) {
  motorsRightSpeed(speed, false);
  motorsLeftSpeed(speed, true);
}