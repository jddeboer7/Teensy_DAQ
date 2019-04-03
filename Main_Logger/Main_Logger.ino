//Main Data-recieving code, currently only for gps data

#include <SPI.h>
#include "FileWriter.h"
#include "WheelSpeed.h"

const byte HE_FW = 0;
const byte HE_RW = 1;
const byte LAP = 3;
const byte RUN = 2;

const byte TRIGGERS_FW = 4;
const byte TRIGGERS_RW = 26;

bool createNF = false;
bool createND = false;
// temporary
bool started = false;

unsigned long prev_write; 
unsigned long REFRESH_RATE = 250; // milliseconds

const int MAX_BUFFER = 12; //TODO: find max buffer

File file;
SdFatSdioEX sdEx;
String data_buffer;

// Instantiate wheel speed sensors
WheelSpeed fWheel = WheelSpeed(TRIGGERS_FW);
WheelSpeed rWheel = WheelSpeed(TRIGGERS_RW);


void setup() {
    fileSetUp(file, sdEx);

    const byte fWheelInterrupt = digitalPinToInterrupt(HE_FW);
    attachInterrupt(fWheelInterrupt, fWheelISR, RISING);
  
    const byte rWheelInterrupt = digitalPinToInterrupt(HE_RW);
    attachInterrupt(rWheelInterrupt, rWheelISR, RISING);

    const byte lapInterrupt = digitalPinToInterrupt(LAP);
    attachInterrupt(lapInterrupt, lapper, FALLING);
    
    // temporary!
    pinMode(RUN, INPUT);

    // temporarily commented
//    const byte runInterrupt = digitalPinToInterrupt(RUN);
//    attachInterrupt(runInterrupt, runner, FALLING);
  
    prev_write = millis();
	  
 } //end setup

void loop() {

//  // temporary!
//  if(!started) {
//    Serial.println("1");
//    if(digitalRead(RUN)) {
//      started = true;
//      Serial.println("2");
//    } else {
//      Serial.println("3");
//      return;
//    }
//  }
  
  // Time for next record
  if((createNF)&&(prev_write - millis() >= REFRESH_RATE)){
      noInterrupts();     
      createNewFile(file, sdEx);
      createNF = false;
      interrupts();
      prev_write = millis();
  }
  
  if((createND)&&(prev_write - millis() >= REFRESH_RATE)){
      noInterrupts(); 
      sdEx.chdir("..");    
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
    file.print(fWheel.getRPS() * 60);
    file.print(",");
    file.print(rWheel.getRPS() * 60);
//    file.print(",");
//    file.print(fWheel.getRPS() * 5.75958653);
//    file.print(",");
//    file.print(rWheel.getRPS() * 5.75958653);
//    file.print(String(millis(), 5)); // Assumes 11" effective wheel radius
    file.print("\n");
  }
  
   // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }
} // End loop

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
