// This is the sketch for my robot in IT

// These includes aren't necessary for the compiler to work, but they stop vscode
// intelisense from having a fit
#include "Distance.h"
#include "Motors.h"
#include "Numbers.h"
#include "IR.h"

#define STOP_DISTANCE 20
#define REVERSE_DISTANCE 15
#define DISTANCE_DELAY 350
#define RESET_DELAY 1000

#define LEFT_OFFSET 0
#define RIGHT_OFFSET 10

#define MOTOR_SPEED 200

enum states
{
  navigateState,
  mapState,
  idleState,
};

bool motorRunning = true;
bool motorWasRunning = false;
bool lastRand = false;

uint8 timesLooped = 0;
uint8 speed = percent(50);
byte state = mapState;

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

void map()
{
  // Maps the area around the robot by rotating. This function should:
  // [ ] Spin in a full circle
  // [ ] Take distance readings with each spin
  // [ ] Send these distance readings back over serial for a computer to process

  #define SAMPLES 28

  float distances[SAMPLES] = {};

  for (byte i = 0; i < SAMPLES - 1; i++)
  {
    distances[i] = getDistance();
    motorsTurnRightSpeed(MOTOR_SPEED);
    delay(100);
  }

  motorsStop();

  String out = "Distances: ";

  for (byte i = 0; i < SAMPLES; i++)
  {
    if (i != 0) out += " ";
    out += distances[i];
  } 

  Serial.println(out);
  Serial.println("Done");

  // Reset the state to idle once done with processing
  state = idleState;
}

void goToByte(byte direction) {
  // Turn in that direction
  if (direction != 0)
    motorsTurnRightSpeed(MOTOR_SPEED);

  // Continue turning for the direction multipled by 100
  delay(100 * direction);

  // Stop the motors
  motorsStop();
}

void loop()
{
  // The main loop has to do  a few things:
  // [ ] Check the state and decicde on what to do
  // [ ] Syncronise state with a connected device (e.g. Computer) via Serial
  // [ ] Delay and other background tasks that are not dependant on each function

  // If the serial is available, read it and drive to the int that it was told to go
  // to
  if (Serial.available() > 0) {
    // Go in the direction that processing wants to go in
    byte incoming = Serial.read();
    goToByte(incoming);

    // Go forward until the distance is smaller
    motorsStraightSpeed(MOTOR_SPEED);

    // Loop until we have a smaller distance
    while (getDistance () > 20) {}

    // Stop motors once we have reached that smaller distance
    motorsStop();

    // Tell the robot to map again
    state = mapState;
    delay(2000);
  }

  if (state == idleState)
  {
    // If the state is idle, we shouldn't do any specific tasks. Because we aren't doing 
    // anything, we can slow down the loop to save power
    delay(500);
  } else if (state == mapState) {
    // Call the mapping function to map the current area
    map();
    delay(2000);
  }
}
