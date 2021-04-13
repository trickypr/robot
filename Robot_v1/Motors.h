#pragma once
#include "Types.h"

// =============================================================================
// Motor config - specifies all of the pins and other parameters that the L298N needs
#define MOTOR_OUT_1 13
#define MOTOR_OUT_2 12
#define MOTOR_ENABLE_1 9

#define MOTOR_OUT_3 7
#define MOTOR_OUT_4 8
#define MOTOR_ENABLE_2 10

#define MOTOR_OUT_All                                                                  \
  {                                                                                    \
    MOTOR_OUT_1, MOTOR_OUT_2, MOTOR_OUT_3, MOTOR_OUT_4, MOTOR_ENABLE_1, MOTOR_ENABLE_2 \
  }

// =============================================================================
// Motor logic

/**
 * Initialises the motor driver. MUST be called before using any of the motor functions
 */
void motorsSetup()
{
  // This is assigned as a variable here for a number of reasons:
  // - The compiler will have a fit if I use the MOTOR_OUT_All constat like MOTOR_OUT_All[i]
  // - This stops it from bloating memory, as it will be cleared once this function goes out of scope
  uint8 all_out[6] = MOTOR_OUT_All;

  // Set all of the control pins to outputs. Note that I am using bytes to save memory
  for (uint8 i = 0; i < 4; i++)
  {
    // Initialises the specified pin
    pinMode(all_out[i], OUTPUT);
  }
}

void motorsRightSpeed(uint8 speed)
{
  // Setup one side
  digitalWrite(MOTOR_OUT_1, HIGH);
  digitalWrite(MOTOR_OUT_2, LOW);
  // Enable side
  analogWrite(MOTOR_ENABLE_1, speed);
}

void motorsLeftSpeed(uint8 speed)
{
  // Setup one side
  digitalWrite(MOTOR_OUT_3, HIGH);
  digitalWrite(MOTOR_OUT_4, LOW);
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
  // Setup the other side
  digitalWrite(MOTOR_OUT_3, HIGH);
  digitalWrite(MOTOR_OUT_4, LOW);
  // Start them both at the same time
  analogWrite(MOTOR_ENABLE_1, speed);
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