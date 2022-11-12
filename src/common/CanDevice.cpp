#include "CanDevice.h"

void CanDevice::writeCanMessage(int address, uint8_t *can_msg, int len = 8){
  CAN.beginPacket(address);
  for (int i = 0; i < len; i++) {
    CAN.write(can_msg[i]);
  }
  CAN.endPacket();
}

void CanDevice::getCanMessage(uint8_t *can_msg, int len) {
  for (int i = 0; i < len; i++) {
    can_msg[i]  = (char) CAN.read();
  }
}

int CanDevice::canChecksum(uint8_t *dat, uint8_t len, uint16_t addr) {
  uint8_t checksum = 0;
  checksum = ((addr & 0xFF00) >> 8) + (addr & 0x00FF) + len + 1;
  //uint16_t temp_msg = msg;
  for (int i = 0; i < len; i++) {
    checksum += (dat[i]);
  }
  return checksum;
}