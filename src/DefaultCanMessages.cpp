#include "DefaultCanMessages.h"

void DefaultCanMessages::writeToCan() {
  uint8_t espPacket[8] = {0x0};
  espPacket[7] = 0x08;
  writeCanMessage(0x3b7, espPacket);

  writeCanMessage(0x423, {0x00}, 1);
}
