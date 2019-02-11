#include "FileWriter.h"

unsigned long REFRESH_RATE = 250;//miliseconds

// Write data header.
void writeHeader() {
  file.print(F("Time"));
  file.print(F(",WheelSpeed"));
  file.println();
}

// Error messages stored in flash.
#define error(msg) sdEx.errorHalt(F(msg))

File fileSetUp() {
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";
  char runName[6] = DIR_BASE_NAME "00";

  String s = String(month()) + "/" + String(day()) + "/" + String(year()) + "_" + runName;
  String p = String(hourFormat12()) + ":" + String(minute()) + ":" + String(second()) + "_" + fileName;
  Serial.println("Folder Name: " + s);
  Serial.println("File Name: " + p);

  
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sdEx.begin()) {
    sdEx.initErrorHalt();
  }
  sdEx.chvol();
  // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sdEx.exists(runName)) {
    if (runName[3 + 1] != '9') {
      runName[3 + 1]++;
    } else if (runName[3] != '9') {
      runName[3 + 1] = '0';
      runName[3]++;
    } else {
      error("Can't create dir name");
    }
  }
  
  if(!sdEx.mkdir(runName)){
    error("mkdir error");
  }
  
  sdEx.chdir(runName);
  
  while (sdEx.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_RDWR | O_CREAT)) {
    error("file.open");
  }
  setSyncProvider(RTC.get);

  Serial.print(F("Logging to: "));
  Serial.println(fileName);

  writeHeader();  

  return file;
}
