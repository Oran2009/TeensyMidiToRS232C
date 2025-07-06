#pragma once
#include <Arduino.h>

void onNoteOn(byte channel, byte note, byte velocity);
void onNoteOff(byte channel, byte note, byte velocity);
void onControlChange(byte channel, byte control, byte value);
void onProgramChange(byte channel, byte program);
void onAfterTouchPoly(byte channel, byte note, byte velocity);
void onAfterTouchChannel(byte channel, byte pressure);
void onPitchChange(byte channel, int pitch);