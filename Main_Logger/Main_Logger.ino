//Main Data-recieving code, currently only for gps data

#include <SD.h>
#include <SPI.h>
#include <TinyGPS.h>

//Logging variables
String directory = "arduino";
File dataFile;

//GPS variables
TinyGPS gps;
HardwareSerial Uart_gps = HardwareSerial();
String data_buffer;


void setup() {
  Serial.begin(115200);
  Uart_gps.begin(19200);
  Serial2.begin(19200); //Slave connection
  while(!Serial || !Uart_gps || !Serial2){
    ;
  }
    
  //Set-up SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  /**TODO: add directory and file naming protocols**/

  // Create arduino directory
  if (!SD.exists(directory.c_str())) {
    SD.mkdir(directory.c_str());
    Serial.println("Created directory '" + directory + "'.");
  } else {
    Serial.println("Directory '" + directory + "' already exists.");
  }
  // Create new file
  int filename = 1;
  String path = directory + "/" + String(filename) + ".txt";
  while(SD.exists(path.c_str())) {
    filename++;
  }
  dataFile = SD.open(path.c_str(), FILE_WRITE);
  Serial.println("Created file '" + String(filename) + ".txt' in directory.");
  
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
