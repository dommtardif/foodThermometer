#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-06-06 19:29:22

#include "Arduino.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_MAX31865.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "notes.h"
#include "definitions.h"

void setup(void) ;
boolean checkForFaults() ;
void printTimeUntil(int timeUntil) ;
void printStage(int currentTemp, int currentDelta, byte pos) ;
void alarmMode(int currentTemp, int currentDelta, byte pos) ;
void calibMode(byte pos) ;
void saveToEEPROM(int pos, int value) ;
void setAlarmTemp() ;
void beep (int speakerPin, float noteFrequency, long noteDuration) ;
void playAlarm() ;
void checkAlarm(int currentTemp, int currentDelta) ;
void alarmSwitch() ;
void loop(void) ;

#include "foodThermometer.ino"


#endif
