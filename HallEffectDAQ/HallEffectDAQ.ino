#include "LiveDisplay.h"
#include "WheelSpeed.h"
#include "Slave_Ports.h"

//TODO: add timestamps (read RTC library and examples)
//TODO: oversampling

const byte TRIGGERS = 1;
const byte CHARS = 3;
const int MAX_BUFFER_SIZE = 64; //TODO: find max write buffer 
String Buffer = "";

// Instantiate front wheel speed
WheelSpeed FR = WheelSpeed(TRIGGERS);
WheelSpeed FL = WheelSpeed(TRIGGERS);
WheelSpeed RW = WheelSpeed(TRIGGERS);
WheelSpeed En = WheelSpeed(TRIGGERS);

void setup() {
   Serial1.begin(19200);//random, verify max baud rate
   while (!Serial1) {
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

  
}//end setup

void loop() {
  if(Buffer.length() >= MAX_BUFFER_SIZE){
    writeBuffer();
  }
}//end loop

void writeBuffer(){
  cli();
  Serial1.print(Buffer);
  Buffer = "";
  sei();
}


void ISR_1() {
  FR.calcRPS();
  Buffer += "r"+ ((byte)FR.getRPS());
}

void ISR_2() {
  FL.calcRPS();
  Buffer += "l"+ ((byte)FL.getRPS());
}

void ISR_3() {
  RW.calcRPS();
  Buffer += "w"+ ((byte)RW.getRPS());
}

void ISR_4() {
  En.calcRPS();
  Buffer += "e"+ ((byte)En.getRPS());
}
