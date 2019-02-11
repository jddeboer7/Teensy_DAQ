//Main Data-recieving code, currently only for gps data

#include <SPI.h>
#include <TinyGPS.h>
#include "FileWriter.h"

//GPS variables
TinyGPS gps;
HardwareSerial Uart_gps = HardwareSerial();
String data_buffer;
bool datafile = 0;
File file;

void setup() {
  file = fileSetUp();
  Serial.begin(115200);
  Uart_gps.begin(19200);
  Serial2.begin(19200); //Slave connection
  while(!Serial || !Uart_gps || !Serial2){
    ;
  }
  
 }//end setup

void loop() {
  /**TODO: add other sensors**/
  /**TODO: write GPS in timer-triggered ISR **/
  if (dataFile) {
    data_buffer = GPS_data();
    dataFile.println(data_buffer);
  }  
  
}//end loop

//ISR for slave
void serialEvent2(){
  while(Serial2.available() > 0){ //need to read enitre string, might not be the way to go
    data_buffer += Serial2.read(); 
  }
  
}
