#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <Arduino.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SdFat.h>


// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data"
#define DIR_BASE_NAME "Run"

SdFatSdioEX sdEx;
File file;

// Time in micros for next data record.
uint32_t logTime;
const uint8_t ANALOG_COUNT = 1;

File fileSetUp();

#endif
