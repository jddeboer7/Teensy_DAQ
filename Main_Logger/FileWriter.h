#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <Arduino.h>
#include <DS1307RTC.h>
#include <TimeLib.h>
#include "SdFat.h"

// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data"
#define DIR_BASE_NAME "Run"
#define ROOT_NAME "DAQ"
#define ROOT_NUM 4
#define BASE_NAME_SIZE (sizeof(FILE_BASE_NAME) - 1)
#define BCHAR_NUM 6
#define FCHAR_NUM 13

// Error messages stored in flash.
#define error(msg) sdEx.errorHalt(F(msg))
#define error2(msg) sdRoot.errorHalt(F(msg))

const uint8_t ANALOG_COUNT = 1;

void fileSetUp(File &file, SdFatSdioEX &sdRoot, SdFatSdioEX &sdEx);

void createNewFile(File &file, SdFatSdioEX &sdEx);

void createNewDir(SdFatSdioEX &sdEx, SdFatSdioEX &sdRoot);

#endif
