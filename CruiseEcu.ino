/*

Custom code for David's 2008 Toyota Yaris with custom arduino pedal interceptor shield

****** PINOUTS ******
  BUTTONS
    Cruise on/off:      A0
    Set/resume:         A1
    Throttle In Low:    A2
    Throttle In High:   A3
    Cruise LED:         A4
    Throttle Out High:  5
    Throttle Out Low:   6
    Steer lever left:   7
    Steer lever right:  8

  CAN tranciever code credit
  Copyright (c) Sandeep Mistry. All rights reserved.
  Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <CAN.h>
#include "src/ThrottlePedal.h"
#include "src/TurnLevers.h"
#include "src/CruiseStalk.h"
#include "src/Brake.h"
#include "src/SpeedSensor.h"
#include "src/CruiseState.h"
#include "src/EngineRpm.h"
#include "src/DefaultCanMessages.h"

ThrottlePedal lowPedal = ThrottlePedal(A2, 6, 0.75);
ThrottlePedal highPedal = ThrottlePedal(A3, 5, 1.6);
TurnLevers turnLevers = TurnLevers(7, 8);
CruiseStalk cruiseStalk = CruiseStalk(A0, A1, A4);
Brake brake = Brake();
DefaultCanMessages defaultCanMessages = DefaultCanMessages();
EngineRpm engineRpm = EngineRpm();
SpeedSensor speedSensor = SpeedSensor();
unsigned long lastUpdate = 0;

/* 
  Deactivated when: 
  - brake pressed
  - too much time passed between brake can messages
  - too much time passed between speed can messages
  - cruise stalk on/off toggled to off
*/
CruiseState cruiseState = CruiseState(cruiseStalk);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // start the can bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting can failed");
    while (1);
  }
  CAN.onReceive(processCan);

  lowPedal.begin();
  highPedal.begin();
  turnLevers.begin();
  cruiseStalk.begin();
  // Serial.println("Successfully set up");
}

void loop() {
  unsigned long loopStartMs = millis();
  lowPedal.update();
  highPedal.update();
  turnLevers.update();
  cruiseStalk.update();

  bool safeToCruise = engineRpm.isSafe() && speedSensor.isSafe();
  if (safeToCruise) {
    //if ((millis() - lastUpdate) >= 10) {
      lastUpdate = millis();
      updateCruiseState();
    //}
  } else {
    Serial.println("NOT SAFE");
    cruiseState.clearSetSpeed();
    cruiseState.deactivate();
    lowPedal.setThrottlePosition(0.);
    highPedal.setThrottlePosition(0.);
  }

  turnLevers.writeToCan();
  brake.writeToCan();
  speedSensor.writeToCan();
  cruiseState.writeToCan();
  lowPedal.writeToCan();
  defaultCanMessages.writeToCan();
  //Serial.print("Loop duration: ");
  //Serial.println(millis() - loopStartMs);
}

void updateCruiseState() {
  if (cruiseStalk.getCruiseOn()) {
    bool setPressed = cruiseStalk.getSetPressed();
    bool resPressed = cruiseStalk.getResPressed();
    if (!cruiseState.getCruiseActive()) {
      // Serial.println("Cruise Activated");
      if (setPressed) {
        cruiseState.activate(max(29.0, speedSensor.getCurrentSpeed()), lowPedal.getPedalPosition());
      } else if (resPressed) {
        cruiseState.resume(lowPedal.getPedalPosition());
      }
    } else if (cruiseState.getCruiseActive()) {
      if (setPressed) {
        cruiseState.decrementSetSpeed();
      } else if (resPressed) {
        cruiseState.incrementSetSpeed();
      }
    }
    if (cruiseState.getCruiseActive()) {
      double newThrottle = cruiseState.updateThrottle(speedSensor.getCurrentSpeed());
      lowPedal.setThrottlePosition(newThrottle);
      highPedal.setThrottlePosition(newThrottle);
    }
  } else {
    //Serial.println("Cruise not on");
    cruiseState.clearSetSpeed();
    cruiseState.deactivate();
    lowPedal.setThrottlePosition(0.);
    highPedal.setThrottlePosition(0.);
  }
}

void processCan(int packetSize) {
  int canPacketId = CAN.packetId();
  switch (canPacketId) {
    // case 0x343:
    //   uint8_t accControl[8];
    //   get_can_msg(accControl, 8);
    //   if ((accControl[3] & 0x01) == 1) {
    //     cruiseState.deactivate();
    //     Serial.println("canCEL!");
    //   }
    //   break;
    case 0x2c4:
      engineRpm.update();
      if (engineRpm.getEngineRpm() > 3000) {
        cruiseState.deactivate();
      }
      break;
    case 0x2d0:
      speedSensor.update();
      break;
    case 0x3b4:
      brake.update();
      if (brake.getBrakePressed()) {
        cruiseState.deactivate();
        lowPedal.setThrottlePosition(0.);
        highPedal.setThrottlePosition(0.);
      }
      break;
    case 0x610:
      speedSensor.updateLowRes();
      break;
  }
}
