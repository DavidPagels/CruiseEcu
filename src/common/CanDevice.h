#ifndef CanDevice_h
#define CanDevice_h
#include <CAN.h>
#include "Arduino.h" 

class CanDevice {
public:
    virtual void writeToCan() {};
protected:
    void writeCanMessage(int address, uint8_t *can_msg, int len = 8);
    void getCanMessage(uint8_t *can_msg, int len);
    int canChecksum(uint8_t *dat, uint8_t len, uint16_t addr);
};
#endif