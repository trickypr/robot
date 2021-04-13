#include "Motors.h"

void setup()
{
  motorsSetup();

  motorsLeftSpeed(255);
  delay(1000);
  motorsStop();

  motorsRightSpeed(255);
  delay(1000);
  motorsStop();
}

void loop() {}