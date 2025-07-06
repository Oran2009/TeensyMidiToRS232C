#include "mappings.h"
#include "commands.h"

// Example: Video Mixer Mapping
void testMap(uint8_t cmd, uint8_t channel, uint8_t param1, uint8_t param2) {
    if (cmd == 0x90) { // Note On
        if (param2 == 0) return;
        if (param1 == 60) setCmdNoReplace(MX30_A_BUS_SOURCE_1);
        else if (param1 == 61) setCmdNoReplace(MX30_A_BUS_SOURCE_2);
        // ...add more note mappings...
    } else if (cmd == 0xB0) { // Control Change
        if (param1 == 0) setCmd(MX30_A_B_MIX_LEVEL, param2);
        else if (param1 == 1) setCmd(MX30_THRESHOLD_LUM_KEY, param2);
        // ...add more CC mappings...
    }
    // ...add more message types as needed...
}