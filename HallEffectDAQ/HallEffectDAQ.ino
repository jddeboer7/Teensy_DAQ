#include "LiveDisplay.h"
#include "WheelSpeed.h"

#include "Slave_Ports.h"

const byte TRIGGERS = 1;
const byte CHARS = 3;

//TODO: use RTC signal to add timestamps

// Instantiate front wheel speed
WheelSpeed FR = WheelSpeed(TRIGGERS);
WheelSpeed FL = WheelSpeed(TRIGGERS);
WheelSpeed RW = WheelSpeed(TRIGGERS);
WheelSpeed En = WheelSpeed(TRIGGERS);

void setup() {

  // Open serial communications and wait for port to open:
   Serial.begin(9600);
   while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only.
   }

  // Set up front wheel
  const byte i1 = digitalPinToInterrupt(HE_FR);
  attachInterrupt(i1, ISR_1, RISING);
  const byte i2 = digitalPinToInterrupt(HE_FL);
  attachInterrupt(i2, ISR_2, RISING);
  const byte i3 = digitalPinToInterrupt(HE_RW);
  attachInterrupt(i3, ISR_3, RISING);
  const byte i4 = digitalPinToInterrupt(HE_E);
  attachInterrupt(i4, ISR_4, RISING);
}

void loop() {
  ;
}


void ISR_1() {
  FR.calcRPS();
  Serial.print("FR: ");
  Serial.println((byte) FR.getRPS());
}

void ISR_2() {
  FL.calcRPS();
  Serial.print("FL: ");
  Serial.println((byte) FL.getRPS());
}

void ISR_3() {
  RW.calcRPS();
  Serial.print("RW: ");
  Serial.println((byte) RW.getRPS());
}

void ISR_4() {
  En.calcRPS();
  Serial.print("En: ");
  Serial.println((byte) En.getRPS());
}
