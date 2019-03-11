//Main Data-recieving code, currently only for gps data

#include <SPI.h>
#include "FileWriter.h"
#include "WheelSpeed.h"

const byte HE_1 = 0;
const byte HE_2 = 1;
const byte LAP = 2;
const byte RUN = 3;

const byte TRIGGERS = 12;
const byte CHARS = 3;

bool createNF = false;
bool createND = false;

unsigned long prev_write; 
unsigned long REFRESH_RATE = 250;//miliseconds

const int MAX_BUFFER = 12; //TODO: find max buffer

File file;
SdFatSdioEX sdEx;
String data_buffer;

//Instantiate front wheel speed
WheelSpeed fWheel = WheelSpeed(TRIGGERS);
WheelSpeed rWheel = WheelSpeed(TRIGGERS);
  

void setup() {
    fileSetUp(file, sdEx);

    const byte fWheelInterrupt = digitalPinToInterrupt(HE_1);
    attachInterrupt(fWheelInterrupt, fWheelISR, RISING);
  
    const byte rWheelInterrupt = digitalPinToInterrupt(HE_2);
    attachInterrupt(rWheelInterrupt, rWheelISR, RISING);

    const byte lapInterrupt = digitalPinToInterrupt(LAP);
    attachInterrupt(lapInterrupt, lapper, FALLING);

    const byte runInterrupt = digitalPinToInterrupt(RUN);
    attachInterrupt(runInterrupt, runner, FALLING);
  
    prev_write = millis();
	  
 }//end setup

void loop() {
  // Time for next record.
  if((createNF)&&(prev_write - millis() >= REFRESH_RATE)){
      noInterrupts();     
      createNewFile(file, sdEx);
      createNF = false;
      interrupts();
      prev_write = millis();
  }
  
  if((createND)&&(prev_write - millis() >= REFRESH_RATE)){
      noInterrupts();     
      createNewDir(sdEx);
      createND = false;
      createNewFile(file, sdEx);
      interrupts();
      prev_write = millis();
  }
  
  if(prev_write - millis() >= REFRESH_RATE){
    prev_write = millis();
    file.print(hour());
    file.print(":");
    file.print(minute());
    file.print(":");
    file.print(second());
    file.print(",");
    file.print(fWheel.getRPS() * 5.75958653);
    file.print(",");
    file.print(rWheel.getRPS() * 5.75958653);
    //file.print(String(millis(), 5)); // Assumes 11" effective wheel radius
    file.print("\n");
  }
  
   // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }
}//end loop

void fWheelISR() {
  fWheel.calcRPS();
}

void rWheelISR() {
  rWheel.calcRPS();
}

void lapper(){
  createNF = true;
}

void runner(){
  createND = true;
}
