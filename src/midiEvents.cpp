// midiEvents.cpp
#include "midiEvents.h"
#include "commands.h"
#include "mappings.h"
#include "globals.h"
#include <USBHost_t36.h>

// Externally defined in your main.cpp:
extern void (*mapToCmd)(uint8_t, uint8_t, uint8_t, uint8_t);
extern HardwareSerialIMXRT& HWSERIAL;

void onNoteOn(byte channel, byte note, byte velocity) {
    if (DEBUG) {
        Serial.print("Note On | ch: "); Serial.print(channel);
        Serial.print(" note: "); Serial.print(note);
        Serial.print(" vel: "); Serial.println(velocity);
    }
    usbMIDI.sendNoteOn(note, velocity, channel);
    mapToCmd(0x90, channel, note, velocity);
    sendCmd(HWSERIAL);
}

void onNoteOff(byte channel, byte note, byte velocity) {
    if (DEBUG) {
        Serial.print("Note Off | ch: "); Serial.print(channel);
        Serial.print(" note: "); Serial.print(note);
        Serial.print(" vel: "); Serial.println(velocity);
    }
    usbMIDI.sendNoteOff(note, velocity, channel);
    // Optionally handle Note Offs
}

void onControlChange(byte channel, byte control, byte value) {
    if (DEBUG) {
        Serial.print("Control Change | ch: "); Serial.print(channel);
        Serial.print(" cc: "); Serial.print(control);
        Serial.print(" val: "); Serial.println(value);
    }
    usbMIDI.sendControlChange(control, value, channel);
    mapToCmd(0xB0, channel, control, value);
    sendCmd(HWSERIAL);
}

void onProgramChange(byte channel, byte program) {
    if (DEBUG) {
        Serial.print("Program Change | ch: "); Serial.print(channel);
        Serial.print(" program: "); Serial.println(program);
    }
    usbMIDI.sendProgramChange(program, channel);
    mapToCmd(0xC0, channel, program, 0);
    sendCmd(HWSERIAL);
}

void onAfterTouchPoly(byte channel, byte note, byte velocity) {
    if (DEBUG) {
        Serial.print("Poly Aftertouch | ch: "); Serial.print(channel);
        Serial.print(" note: "); Serial.print(note);
        Serial.print(" val: "); Serial.println(velocity);
    }
    usbMIDI.sendAfterTouchPoly(note, velocity, channel);
    // Optionally handle Polyphonic Aftertouch
}

void onAfterTouchChannel(byte channel, byte pressure) {
    if (DEBUG) {
        Serial.print("Channel Aftertouch | ch: "); Serial.print(channel);
        Serial.print(" pressure: "); Serial.println(pressure);
    }
    usbMIDI.sendAfterTouch(pressure, channel);
    // Optionally handle Channel Aftertouch
}

void onPitchChange(byte channel, int pitch) {
    if (DEBUG) {
        Serial.print("Pitch Bend | ch: "); Serial.print(channel);
        Serial.print(" value: "); Serial.println(pitch);
    }
    usbMIDI.sendPitchBend(pitch, channel);
    // Optionally handle Pitch Bend
}