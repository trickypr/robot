#pragma once

// =============================================================================
// IR config - The pins and other constants for IR control

#define IR_RECEIVE_PIN 2
#define DECODE_NEC

#include <IRremote.h>

void IRSetup()
{
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, LED_BUILTIN);
}

bool IRHasReceivedCommand()
{
  return IrReceiver.decode();
}

IRData IRGetData()
{
  // Print a short summary of received data
  IrReceiver.printIRResultShort(&Serial);
  if (IrReceiver.decodedIRData.protocol == UNKNOWN)
  {
    // We have an unknown protocol here, print more info
    IrReceiver.printIRResultRawFormatted(&Serial, true);
  }
  Serial.println();

  /*
   * !!!Important!!! Enable receiving of the next value,
   * since receiving has stopped after the end of the current received data packet.
   */
  IrReceiver.resume(); // Enable receiving of the next value

  // Return received data
  return IrReceiver.decodedIRData;
}