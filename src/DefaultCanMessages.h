#ifndef DefaultCanMessages_h
#define DefaultCanMessages_h
#include "common/CanDevice.h"

class DefaultCanMessages: public CanDevice {
public:
    void writeToCan();
};
#endif
