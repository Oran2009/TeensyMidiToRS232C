#include "commands.h"
#include "globals.h"
#include "mappings.h"
#include "midiEvents.h"
#include <Arduino.h>
#include <USBHost_t36.h>

// Globals
bool DEBUG = true;

// Set the serial port
HardwareSerialIMXRT &RS232C_SERIAL = Serial1;
HardwareSerialIMXRT &MIDI_SERIAL = Serial2;

// USB MIDI
USBHost myusb;
MIDIDevice_BigBuffer midi1(myusb);

// Set mapping function (implemented in mappings.cpp)
void (*mapToCmd)(uint8_t, uint8_t, uint8_t, uint8_t) = testMap;

// Test
unsigned long lastTestSend = 0; // for timing the test command

// --- Setup & Loop ---
void setup() {
  Serial.begin(115200);
  RS232C_SERIAL.begin(9600, SERIAL_7O1);
  MIDI_SERIAL.begin(31250, SERIAL_8N1);

  delay(1500); // Let USB devices power up
  myusb.begin();

  midi1.setHandleNoteOn(onNoteOn);
  midi1.setHandleNoteOff(onNoteOff);
  midi1.setHandleControlChange(onControlChange);
  midi1.setHandleProgramChange(onProgramChange);
  midi1.setHandleAfterTouchPoly(onAfterTouchPoly);
  midi1.setHandleAfterTouchChannel(onAfterTouchChannel);
  midi1.setHandlePitchChange(onPitchChange);

  if (DEBUG)
    Serial.println("Setup complete, debug mode ON");
}

void loop() {
  myusb.Task();
  midi1.read();
  readMidiFromDIN();
}