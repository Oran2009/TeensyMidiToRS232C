// midiEvents.cpp
#include "midiEvents.h"
#include "commands.h"
#include "globals.h"
#include "mappings.h"
#include <USBHost_t36.h>

// Externally defined in your main.cpp:
extern void (*mapToCmd)(uint8_t, uint8_t, uint8_t, uint8_t);

void onNoteOn(byte channel, byte note, byte velocity) {
  if (DEBUG) {
    Serial.print("Note On | ch: ");
    Serial.print(channel);
    Serial.print(" note: ");
    Serial.print(note);
    Serial.print(" vel: ");
    Serial.println(velocity);
  }

  // Send to USB MIDI
  usbMIDI.sendNoteOn(note, velocity, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0x90 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(note);
  MIDI_SERIAL.write(velocity);

  // Handle Note On
  mapToCmd(0x90, channel, note, velocity);
  sendCmd();
}

void onNoteOff(byte channel, byte note, byte velocity) {
  if (DEBUG) {
    Serial.print("Note Off | ch: ");
    Serial.print(channel);
    Serial.print(" note: ");
    Serial.print(note);
    Serial.print(" vel: ");
    Serial.println(velocity);
  }

  // Send to USB MIDI
  usbMIDI.sendNoteOff(note, velocity, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0x80 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(note);
  MIDI_SERIAL.write(velocity);

  // Optionally Handle Note Off
}

void onControlChange(byte channel, byte control, byte value) {
  if (DEBUG) {
    Serial.print("Control Change | ch: ");
    Serial.print(channel);
    Serial.print(" cc: ");
    Serial.print(control);
    Serial.print(" val: ");
    Serial.println(value);
  }

  // Send to USB MIDI
  usbMIDI.sendControlChange(control, value, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0xB0 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(control);
  MIDI_SERIAL.write(value);

  // Handle Control Change
  mapToCmd(0xB0, channel, control, value);
  sendCmd();
}

void onProgramChange(byte channel, byte program) {
  if (DEBUG) {
    Serial.print("Program Change | ch: ");
    Serial.print(channel);
    Serial.print(" program: ");
    Serial.println(program);
  }

  // Send to USB MIDI
  usbMIDI.sendProgramChange(program, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0xC0 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(program);

  // Handle Program Change
  mapToCmd(0xC0, channel, program, 0);
  sendCmd();
}

void onAfterTouchPoly(byte channel, byte note, byte velocity) {
  if (DEBUG) {
    Serial.print("Poly Aftertouch | ch: ");
    Serial.print(channel);
    Serial.print(" note: ");
    Serial.print(note);
    Serial.print(" val: ");
    Serial.println(velocity);
  }

  // Send to USB MIDI
  usbMIDI.sendAfterTouchPoly(note, velocity, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0xA0 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(note);
  MIDI_SERIAL.write(velocity);

  // Optionally Handle Polyphonic Aftertouch
}

void onAfterTouchChannel(byte channel, byte pressure) {
  if (DEBUG) {
    Serial.print("Channel Aftertouch | ch: ");
    Serial.print(channel);
    Serial.print(" pressure: ");
    Serial.println(pressure);
  }

  // Send to USB MIDI
  usbMIDI.sendAfterTouch(pressure, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0xD0 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(pressure);

  // Optionally Handle Channel Aftertouch
}

void onPitchChange(byte channel, int pitch) {
  if (DEBUG) {
    Serial.print("Pitch Bend | ch: ");
    Serial.print(channel);
    Serial.print(" value: ");
    Serial.println(pitch);
  }

  // Send to USB MIDI
  usbMIDI.sendPitchBend(pitch, channel);

  // Send to 5-pin MIDI OUT
  MIDI_SERIAL.write(0xE0 | ((channel - 1) & 0x0F));
  MIDI_SERIAL.write(pitch & 0x7F);        // LSB
  MIDI_SERIAL.write((pitch >> 7) & 0x7F); // MSB

  // Optionally Handle Pitch Bend
}

void readMidiFromDIN() {
  static uint8_t state = 0; // 0 = waiting for status byte
  static uint8_t command = 0;
  static uint8_t channel = 0;
  static uint8_t data1 = 0;
  static uint8_t data2 = 0;

  while (MIDI_SERIAL.available()) {
    uint8_t byteRead = MIDI_SERIAL.read();

    if (byteRead & 0x80) { // status byte
      command = byteRead & 0xF0;
      channel = byteRead & 0x0F;
      // For Program Change and Channel Pressure, only 1 data byte
      if (command == 0xC0 || command == 0xD0) {
        state = 2; // expect 1 data byte
      } else {
        state = 1; // expect 2 data bytes
      }
    } else {
      if (state == 1) { // expecting first data byte
        data1 = byteRead;
        state = 3;             // expecting second data byte
      } else if (state == 2) { // expecting only one data byte
        data1 = byteRead;
        // Call handler
        switch (command) {
        case 0xC0:
          onProgramChange(channel + 1, data1);
          break;
        case 0xD0:
          onAfterTouchChannel(channel + 1, data1);
          break;
        }
        state = 0;
      } else if (state == 3) { // expecting second data byte
        data2 = byteRead;
        // Call handler
        switch (command) {
        case 0x80:
          onNoteOff(channel + 1, data1, data2);
          break;
        case 0x90:
          onNoteOn(channel + 1, data1, data2);
          break;
        case 0xA0:
          onAfterTouchPoly(channel + 1, data1, data2);
          break;
        case 0xB0:
          onControlChange(channel + 1, data1, data2);
          break;
        case 0xE0:
          onPitchChange(channel + 1, (data2 << 7) | data1);
          break;
        }
        state = 0;
      }
    }
  }
}
