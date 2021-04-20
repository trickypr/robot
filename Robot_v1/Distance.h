#pragma once
// This file contains all of the code for measuring distance

// Constants for distance sensor
#define DIST_TRIG 4
#define DIST_ECHO 3
// You can make this longer distance at more expenses
#define DIST_MAX 200         //cm
#define DIST_SOUND_SPEED 346 // The speed of sound is different in Canberra

const float DIST_TIMEOUT = DIST_MAX * 60;

void distanceSetup()
{
  pinMode(DIST_TRIG, OUTPUT);
  pinMode(DIST_ECHO, INPUT);
}

float getDistance()
{
  // Send the trigger signal
  digitalWrite(DIST_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(DIST_TRIG, LOW);

  // Measure the ping time
  unsigned long pingTime = pulseIn(DIST_ECHO, HIGH, DIST_TIMEOUT);

  // Use this time to calculate the distance
  float distance = (float)pingTime * DIST_SOUND_SPEED / 2 / 10000;

  // Return the value ready to use
  return distance;
}
