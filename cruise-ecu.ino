/*

THIS VERSION IS TO ENABLE OP WITHOUT ANYTHING ELSE
****** PINOUTS ******

    BUTTONS
  MAIN_ON to pin 8
  SET (-) to pin 9
  RES (+) to pin 6
  CANCEL to pin 7

    CAN BOARD
  INT to pin 2
  SCK to pin 13
  MOSI to pin 11
  MISO to pin 12
  CS to pin 10

    VSS
  VSS to pin 3 via pullup resistor

    INTERRUPT
  INTERRUPT OUT to pin A4

  CAN tranciever code credit
  Copyright (c) Sandeep Mistry. All rights reserved.
  Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <CAN.h>
//Variables for button presses
unsigned char pins[6] = {A0, 9};
int buttonstate0 = 0;
int lastbuttonstate0 = 0;
int buttonstate1 = 0;
int lastbuttonstate1 = 0;
int buttonstate2 = 0;
int lastbuttonstate2 = 0;
int buttonstate3 = 0;
int lastbuttonstate3 = 0;
int gas_pedal_pressed = 0;
int brake_pedal_pressed = 0;

#define min_set_speed 0

//check_can
long check_can = 0;

//debouncing
long millis_held;
unsigned long firstTime;

//flags for cruise state
boolean mainOn = false;
boolean cruiseActive = false;

//are we using metric or standard?
boolean metric = true;

//CAN default messages
uint8_t set_speed = 0x0;
uint8_t current_speed = 0x0;
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    // Serial.println("Starting CAN failed!");
    while (1);
  }

  //Setup Pins
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(8, OUTPUT);

  CAN.onReceive(processCan);
}

void loop() {
  delay(1);

  //Send data if button was pushed
  int buttonValue1 = analogRead(A0); // Pressed: ~550
  int buttonValue2 = analogRead(A1); // Set: ~550, Res: ~290
  buttonstate0 = buttonValue1 > 250;
  buttonstate1 = buttonValue2 > 400; // set
  buttonstate2 = buttonValue2 < 400 && buttonValue2 > 200; // resume
  if (buttonstate0 != lastbuttonstate0) {
    if (buttonstate0 == HIGH) {
      Serial.println("ON/OFF_BUTTON");
      firstTime = millis();
      if (millis_held > 50) {
        mainOn = !mainOn;
      }
    }
  }

  if (mainOn) {
    if (!cruiseActive) {
      if ((buttonstate1 == HIGH) && (buttonstate1 != lastbuttonstate1)) {
        Serial.println("SET_BUTTON");
        //set the current speed as set speed, but only if it's over 5km/h
        cruiseActive = true;
        set_speed = 20; // TODO: get speed from can bus
      }
    } else {
      if (buttonstate1 == HIGH) {
        Serial.println("MINUS_SET_BUTTON");
        if (set_speed > min_set_speed) {
          firstTime = millis();
          if (millis_held > 50) {
            set_speed -= round(1 * 1.60934);
          }
        }
      }
      if (buttonstate2 == HIGH) {
        Serial.println("PLUS_RES_BUTTON");
        firstTime = millis();
        if (millis_held > 50) {
          set_speed += round(1 * 1.60934);
        }
      }
    }
  }

  millis_held = (millis() - firstTime);

  if (!mainOn) {
    cruiseActive = false;
    set_speed = 0;
  }

  if (brake_pedal_pressed) {
    digitalWrite(8, HIGH);
    cruiseActive = false;
  } else {
    digitalWrite(8, LOW);
  }

  //0x1d2 msg PCM_CRUISE
  uint8_t dat[8] = {0x0};
  dat[0] = (cruiseActive << 5) & 0x20 | 0x10;
  dat[6] = (cruiseActive << 7) & 0x80;
  dat[7] = can_cksum(dat, 7, 0x1d2);
  CAN.beginPacket(0x1d2);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat[ii]);
  }
  CAN.endPacket();

  //0x1d3 msg PCM_CRUISE_2
  uint8_t dat2[8] = {0x0};
  dat2[1] = (mainOn << 7) & 0x80 | 0x28;
  dat2[2] = set_speed;
  dat2[7] = can_cksum(dat2, 7, 0x1d3);
  CAN.beginPacket(0x1d3);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat2[ii]);
  }
  CAN.endPacket();

  
  //0xaa msg defaults 1a 6f WHEEL_SPEEDS
  uint8_t dat3[8];
  uint16_t wheelspeed = 0x1a6f + (current_speed * 100);
  dat3[0] = (wheelspeed >> 8) & 0xFF;
  dat3[1] = (wheelspeed >> 0) & 0xFF;
  dat3[2] = (wheelspeed >> 8) & 0xFF;
  dat3[3] = (wheelspeed >> 0) & 0xFF;
  dat3[4] = (wheelspeed >> 8) & 0xFF;
  dat3[5] = (wheelspeed >> 0) & 0xFF;
  dat3[6] = (wheelspeed >> 8) & 0xFF;
  dat3[7] = (wheelspeed >> 0) & 0xFF;
  CAN.beginPacket(0xaa);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat3[ii]);
  }
  CAN.endPacket();

  // 0x2c2 msg yaris GAS_PEDAL
  uint8_t dat10[8] = {0x0};
  dat10[0] = 0x08;
  CAN.beginPacket(0x2c2);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat10[ii]);
  }
  CAN.endPacket();

  //0x224 msg fake brake module
  uint8_t dat11[8] = {0x0};
  dat11[0] = (brake_pedal_pressed << 5) & 0x20;
  CAN.beginPacket(0x224);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat11[ii]);
  }
  CAN.endPacket();

  //0x3b7 msg ESP_CONTROL for OP ESP Disabled warning
  uint8_t espPacket[8] = {0x0};
  espPacket[7] = 0x08;
  CAN.beginPacket(0x3b7);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(espPacket[ii]);
  }
  CAN.endPacket();

  //0x423 fake EPS message 
  CAN.beginPacket(0x423);
  CAN.write(0x00);
  CAN.endPacket();
    
  // 0xb4 speed and encoder for throttle ECU
  uint16_t kmh = (current_speed * 100);
  CAN.beginPacket(0xb4);
  CAN.print(kmh);
  CAN.endPacket();

  lastbuttonstate0 = buttonstate0;
  lastbuttonstate1 = buttonstate1;
  lastbuttonstate2 = buttonstate2;
  lastbuttonstate3 = buttonstate3;

}

void processCan(int packetSize) {
  int canPacketId = CAN.packetId();
  if (canPacketId == 0x343) {
    check_can = millis();
    uint8_t dat12[8];
    get_can_msg(dat12, 8);
    if ((dat12[3] & 0x01) == 1) {
      cruiseActive = false;
      Serial.println("CANCEL!");
    }
  } else if (canPacketId == 0x610) {
    uint8_t speed_msg[8];
    get_can_msg(speed_msg, 8);
    current_speed = speed_msg[2];
  } else if (canPacketId == 0x3b4) {
    uint8_t brake_msg[8];
    get_can_msg(brake_msg, 8);
    brake_pedal_pressed = ((brake_msg[4] & 0x01) == 1);
  } else if (canPacketId == 0x2c1) {
    uint8_t gas_msg[8];
    get_can_msg(gas_msg, 8);
    gas_pedal_pressed = ((gas_msg[0] & 0x08) == 0);
  }
}

void get_can_msg(uint8_t *can_msg, int len) {
  for (int ii = 0; ii < len; ii++) {
    can_msg[ii]  = (char) CAN.read();
  }
}

//TOYOTA CAN CHECKSUM
int can_cksum (uint8_t *dat, uint8_t len, uint16_t addr) {
  uint8_t checksum = 0;
  checksum = ((addr & 0xFF00) >> 8) + (addr & 0x00FF) + len + 1;
  //uint16_t temp_msg = msg;
  for (int ii = 0; ii < len; ii++) {
    checksum += (dat[ii]);
  }
  return checksum;
}
