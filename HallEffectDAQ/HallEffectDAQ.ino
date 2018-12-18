#include "LiveDisplay.h"
#include "WheelSpeed.h"

const byte PORT_1 = 2;
const byte PORT_2 = 3;
const byte PORT_3 = 4;
const byte PORT_4 = 5;
const byte TRIGGERS = 1;
const byte CHARS = 3;

// Instantiate front wheel speed
WheelSpeed w1 = WheelSpeed(TRIGGERS);
WheelSpeed w2 = WheelSpeed(TRIGGERS);
WheelSpeed w3 = WheelSpeed(TRIGGERS);
WheelSpeed w4 = WheelSpeed(TRIGGERS);

void setup() {

  // Open serial communications and wait for port to open:
   Serial.begin(9600);
   while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only.
   }

  // Set up front wheel
  const byte i1 = digitalPinToInterrupt(PORT_1);
  attachInterrupt(i1, ISR_1, RISING);
  const byte i2 = digitalPinToInterrupt(PORT_2);
  attachInterrupt(i2, ISR_2, RISING);
  const byte i3 = digitalPinToInterrupt(PORT_3);
  attachInterrupt(i3, ISR_3, RISING);
  const byte i4 = digitalPinToInterrupt(PORT_4);
  attachInterrupt(i4, ISR_4, RISING);


}


void loop() {
  ;
}


void ISR_1() {
  w1.calcRPS();
  Serial.print("w1: ");
  Serial.println((byte) w1.getRPS());
}

void ISR_2() {
  w2.calcRPS();
  Serial.print("w2: ");
  Serial.println((byte) w2.getRPS());
}

void ISR_3() {
  w3.calcRPS();
  Serial.print("w3: ");
  Serial.println((byte) w3.getRPS());
}

void ISR_4() {
  w4.calcRPS();
  Serial.print("w4: ");
  Serial.println((byte) w4.getRPS());
}
