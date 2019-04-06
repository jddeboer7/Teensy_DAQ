#include "FileWriter.h"

// Write data header.
void writeHeader(File &file) {
  file.print(F("Time"));
  file.print(F(",Front WheelSpeed"));
  file.print(F(",Rear WheelSpeed"));
  file.println();
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void nameFile(char *fileName, SdFatSdioEX &sdEx){
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
}

void createRootDir(SdFatSdioEX &sdRoot){
  char rootName[ROOT_NUM] = ROOT_NAME;
  
  if (!sdRoot.exists(rootName)){
    if(!sdRoot.mkdir(rootName))
      error2("mkdir root error");
  }
  sdRoot.chdir(ROOT_NAME);
  Serial.print("In Root Directory\n");
  
}

void createNewDir(SdFatSdioEX &sdEx, SdFatSdioEX &sdRoot){
  char runName[BCHAR_NUM] = DIR_BASE_NAME "00";
  byte index = 4;
 
  if (BASE_NAME_SIZE > BCHAR_NUM) 
    error("FILE_BASE_NAME too long");

  sdEx = sdRoot;
  while (sdEx.exists(runName)) {
    if (runName[index] != '9') {
      runName[index]++;
    } else if (runName[index-1] != '9') {
      runName[index] = '0';
      runName[index]++;
    } else {
      error("Can't create dir name");
    }
  }
  
  if(!sdEx.mkdir(runName))
    error("mkdir error");
  
  sdEx.chdir(runName);
  Serial.print("Made new directory ");
  Serial.print(runName);
  Serial.print("\n");
}

void createNewFile(File &file, SdFatSdioEX &sdEx){
  char fileName[FCHAR_NUM] = FILE_BASE_NAME "00.csv";
  
  Serial.println("closing file");
  if (!file.close()) 
    error("file.close");
  
  nameFile(fileName, sdEx);

  Serial.print("Opening new file ");
  Serial.print(fileName);
  Serial.print("\n");
  file = sdEx.open(fileName, O_RDWR | O_CREAT);

  writeHeader(file);
}

void fileSetUp(File &file, SdFatSdioEX &sdRoot, SdFatSdioEX &sdEx) {
  char runName[BCHAR_NUM] = DIR_BASE_NAME "00";
  char fileName[FCHAR_NUM] = FILE_BASE_NAME "00.csv";

  setSyncProvider(getTeensy3Time);
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial);
  delay(1000);
  
  String s = String(month()) + "/" + String(day()) + "/" + String(year()) + "_" + runName;
  String p = String(hourFormat12()) + ":" + String(minute()) + ":" + String(second()) + "_" + fileName;
  Serial.println("Folder Name: " + s);
  Serial.println("File Name: " + p);
  
  Serial.println(F("Type any character to start"));
  while (!Serial.available());
  
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sdEx.begin()) {
    sdEx.initErrorHalt();
  }
  sdEx.chvol();
  
  if (!sdRoot.begin()) {
    sdRoot.initErrorHalt();
  }
  sdRoot.chvol();
 
  createRootDir(sdRoot);
  createNewDir(sdEx, sdRoot);

  // Open a new file 
  nameFile(fileName, sdEx);
  if (!file.open(fileName, O_RDWR | O_CREAT))
    error("file.open");

  // Read any Serial data.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);
 
  Serial.print(F("Logging to: "));
  Serial.println(fileName);

  writeHeader(file);
}
