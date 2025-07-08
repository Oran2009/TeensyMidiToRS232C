#include "commands.h"
#include "globals.h"

char cmd[14] = {};
static uint8_t switchStore[20];
static uint8_t paramStore[127];

void setCmd(const char *inputCmd, uint8_t inputParam) {
  strcpy(cmd, inputCmd);
  for (uint8_t i = 0; i < 11; i++) {
    if (cmd[i] == 0x00 || cmd[i + 1] == 0x00)
      return;

    if (cmd[i] == '~' && cmd[i + 1] == '0') {
      char hexParam[4] = {};
      sprintf(hexParam, "%02X", inputParam * 2);
      cmd[i] = hexParam[0];
      cmd[i + 1] = hexParam[1];
    }
    if (cmd[i] == '~' && cmd[i + 1] != '0') {
      paramStore[int(cmd[i + 1])] = inputParam * 2;
      char hexParam[4] = {};
      sprintf(hexParam, "%02X", inputParam * 2);
      cmd[i] = hexParam[0];
      cmd[i + 1] = hexParam[1];
    }
    if (cmd[i] == '*') {
      char hexParam[4] = {};
      sprintf(hexParam, "%02X", paramStore[int(cmd[i + 1])]);
      cmd[i] = hexParam[0];
      cmd[i + 1] = hexParam[1];
    }
  }
}

void setCmdNoReplace(const char *inputCmd) { strcpy(cmd, inputCmd); }

void setCmdSwitch(const char *inputCmd1, const char *inputCmd2,
                  uint8_t switchIndex) {
  if (switchStore[switchIndex] == 0)
    setCmd(inputCmd1);
  else
    setCmd(inputCmd2);
  switchStore[switchIndex] = (switchStore[switchIndex] + 1) % 2;
}

void setCmdStep(const char *inputCmd, uint8_t param, uint8_t maxValue) {
  strcpy(cmd, inputCmd);
  char maxHex[6] = {};
  sprintf(maxHex, "%X", maxValue + 1);
  int maxHexSize = 0;
  for (uint8_t i = 0; i < 6; i++) {
    if (maxHex[i] != 0)
      maxHexSize++;
  }
  int value = (param * maxValue) / 128;
  char valueHex[6] = {};
  if (maxHexSize == 1)
    sprintf(valueHex, "%1X", value);
  else if (maxHexSize == 2)
    sprintf(valueHex, "%02X", value);
  else
    sprintf(valueHex, "%X", value);

  for (uint8_t i = 0; i < 11; i++) {
    if (cmd[i] == '~') {
      for (uint8_t j = 0; j < 6; j++) {
        if (valueHex[j] != 0)
          cmd[i + j] = valueHex[j];
      }
    }
  }
  setCmd(cmd);
}

void setCmdToggle(const char *inputCmd, uint8_t maxValue, uint8_t switchIndex) {
  strcpy(cmd, inputCmd);
  char valueChar[4] = {};
  sprintf(valueChar, "%02d", switchStore[switchIndex]);
  for (uint8_t i = 0; i < 11; i++) {
    if (cmd[i] == '~') {
      for (uint8_t j = 0; j < 4; j++) {
        if (valueChar[j] != 0)
          cmd[i + j] = valueChar[j];
      }
    }
  }
  switchStore[switchIndex] = (switchStore[switchIndex] + 1) % maxValue;
}

void sendCmd(HardwareSerialIMXRT &s) {
  if (cmd[0] == 0)
    return;
  s.write(2);
  s.write(cmd);
  s.write(3);
  if (DEBUG) {
    Serial.print("Sent to RS232C_SERIAL: ");
    Serial.println(cmd);
  }
  memset(cmd, 0, sizeof(cmd));
}