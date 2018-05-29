//Please leave the following reference to Erik Kringen blog post unless you remove his code.
//Dom Tardif
//
//The beep() and playAlarm() functions come from a blog post from
//Erik Kringen (https://mycontraption.com/sound-effects-with-and-arduino/)

//Button 1 Previous - Button 3 Next : Changes the value on screen (alarm and calibration mode)
//Button 2 Menu : Changes thermometer modes
//Button 4 Cancel: On default mode without alarm, goes into calibration mode. In any mode, with an alarm, cancels the alarm
//
//Check and modify the screen and pins setup below and change according to your needs.
//Works as is on Arduino Leonardo

#include <EEPROM.h>
#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "notes.h"

//Screen Setup - modify to needs 128x64 is assumed in display routines
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE);

//PINS Setup
////Switch pins
#define P_MENU 5
#define P_NEXT 6
#define P_PREV 4
#define P_CANCEL 7
////Speaker or piezo pin
#define P_SPEAKER 8
////MAX31865 pins
#define P_CS 15
#define P_SDI 14
#define P_SDO 16
#define P_CLK 10
//END PINS setup

//Fonts
#define DEF_FONT u8g2_font_7x13_t_symbols
#define SMALL_FONT u8g2_font_profont10_tr
#define SMLINE_HEIGHT 8
#define LINE_HEIGHT 11

//Operation mode
#define DEFAULT_MODE 0
#define CANDY_MODE 1
#define BEEF_MODE 2
#define PORC_MODE 3
#define POULTRY_MODE 4
#define ALARM_MODE 5
#define CALIB_MODE 6

//Cooking stage description
typedef struct {
  char description [25];
  int temp;
} descriptionType;

Adafruit_MAX31865 max3 = Adafruit_MAX31865(P_CS, P_SDI, P_SDO, P_CLK);

//MAX31865 reference values
#define RREF      431.0
float rNominal = 100.00; //Modifiable via calibration mode - saved to eeprom

int prevTemp;
byte tmode = DEFAULT_MODE;
int alarmTemp = 0;
byte alarmState = 0;

const static descriptionType PROGMEM candyStages[11]  = {{"Confiserie-Candy making", -5000},
  {"Petit filet/Soft thread", 10500},
  {"Grand filet/Thread", 10700},
  {"Petit boule/Soft ball", 11200},
  {"Grand boule/Firm ball", 11800},
  {"Boule/Hard ball", 12500},
  {"Petit casse/Soft crack", 13500},
  {"Grand casse/Hard crack", 14500},
  {"Caramel", 15000},
  {"Sucre brule/Burnt sugar", 17100},
  {"", 30000}
};

const static descriptionType PROGMEM beefStages[5]  = {{"Boeuf/Agneau-Beef/Lamb", -5000},
  {"Saignant/Rare", 6000},
  {"A point/Medium", 7000},
  {"Bien cuit/Well done", 7500},
  {"", 30000}
};

const static descriptionType PROGMEM porcStages[4]  = {{"Porc", -5000},
  {"Fume/Smoked", 6000},
  {"Frais/Fresh", 7500},
  {"", 30000}
};

const static descriptionType PROGMEM poultryStages[5]  = {{"Volaille-Poultry", -5000},
  {"Poulet/Chicken min", 8000},
  {"Poulet/Chicken max", 8500},
  {"Canard/Duck", 9000},
  {"", 30000}
};

void setup(void) {
  EEPROM.begin();
  if (EEPROM.read(0) == 128) rNominal = (float)EEPROM.read(1) + ((float)EEPROM.read(2) / 100);
  if (EEPROM.read(3) == 128) alarmTemp = (EEPROM.read(4) * 100) + EEPROM.read(5);

  pinMode(P_MENU, INPUT_PULLUP);
  pinMode(P_NEXT, INPUT_PULLUP);
  pinMode(P_PREV, INPUT_PULLUP);
  pinMode(P_CANCEL, INPUT_PULLUP);
  pinMode(P_SPEAKER, OUTPUT);
  u8g2.begin(P_MENU, P_NEXT, P_PREV, U8X8_PIN_NONE, U8X8_PIN_NONE, P_CANCEL);
  u8g2.enableUTF8Print();
  max3.begin(MAX31865_3WIRE);
  prevTemp = (int) (max3.temperature(rNominal, RREF) * 100);
}

boolean checkForFaults() {
  uint8_t fault = max3.readFault();
  if (fault) {
    u8g2.firstPage();
    do {
      u8g2.setFont(SMALL_FONT);
      u8g2.setCursor(0, SMLINE_HEIGHT);
      u8g2.print(F("Erreur/Fault"));
      u8g2.setCursor(0, 2 * SMLINE_HEIGHT);
      u8g2.print(F("# 0x"));
      u8g2.print(fault, HEX);
      u8g2.setCursor(0, 3 * SMLINE_HEIGHT);
      if (fault & MAX31865_FAULT_HIGHTHRESH) {
        u8g2.print(F("RTD High Threshold"));
      }
      if (fault & MAX31865_FAULT_LOWTHRESH) {
        u8g2.print(F("RTD Low Threshold"));
      }
      if (fault & MAX31865_FAULT_REFINLOW) {
        u8g2.print(F("REFIN- > 0.85 x Bias"));
      }
      if (fault & MAX31865_FAULT_REFINHIGH) {
        u8g2.print(F("REFIN- < 0.85 x Bias - FORCE- open"));
      }
      if (fault & MAX31865_FAULT_RTDINLOW) {
        u8g2.print(F("RTDIN- < 0.85 x Bias - FORCE- open"));
      }
      if (fault & MAX31865_FAULT_OVUV) {
        u8g2.print(F("Under/Over voltage"));
      }
    } while ( u8g2.nextPage() );
    max3.clearFault();
    delay(2000);
    return true;
  }
  return false;
}

void printStage(int currentTemp, int currentDelta, byte pos) {
  const static descriptionType *stages;
  byte stagesSize;
  if (tmode == CANDY_MODE) {
    stagesSize = 11;
    stages = candyStages;
  }
  else if (tmode == BEEF_MODE) {
    stages = beefStages;
    stagesSize = 5;
  }
  else if (tmode == PORC_MODE) {
    stages = porcStages;
    stagesSize = 4;
  }
  else if (tmode == POULTRY_MODE) {
    stages = poultryStages;
    stagesSize = 5;
  }
  byte stage = 0;
  descriptionType currentStage;
  descriptionType nextStage;
  do {
    memcpy_P (&currentStage, &stages [stage], sizeof currentStage);
    stage++;
  } while (currentTemp > currentStage.temp && stage < (stagesSize - 1));
  memcpy_P (&nextStage, &stages [stage], sizeof nextStage);

  unsigned int timeUntil = ((currentDelta > 0 && stage <= (stagesSize - 1)) ? (int)((nextStage.temp - currentTemp) / currentDelta) : -1);

  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print(currentStage.description);
  if (timeUntil != -1) {
    u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
    u8g2.print(timeUntil);
    u8g2.print(F(" s avant/until"));
  }
  u8g2.setCursor(0, pos + 3 * SMLINE_HEIGHT);
  u8g2.print(F("Prochain stade/Next stage"));
  u8g2.setCursor(0, pos + 4 * SMLINE_HEIGHT);
  u8g2.print(nextStage.description);
}


void alarmMode(int currentTemp, int currentDelta, byte pos) {
  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  if (alarmState != 0) {
    int timeUntil = ((currentDelta > 0 && (alarmTemp * 100) > currentTemp) || (currentDelta < 0 && (alarmTemp * 100) < currentTemp) ? (int)((alarmTemp * 100 - currentTemp) / currentDelta) : -1);
    if (timeUntil != -1) {
      u8g2.print(timeUntil);
      u8g2.print(F(" s avant/until"));
    }
  }
  u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
  u8g2.print(F("alarme/alarm:"));
  u8g2.print(alarmTemp);
  u8g2.print(F("℃ / "));
  u8g2.print((int)(alarmTemp * 1.8 + 32));
  u8g2.print(F("℉"));
  u8g2.setCursor(0, pos + 3 * SMLINE_HEIGHT);
  if (alarmState != 0) u8g2.print(F("Actif/Active"));
  else u8g2.print(F("Inactif/Inactive"));
}

void calibMode(byte pos) {
  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print(F("Calibration"));
  u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
  u8g2.print(F("Res.nom.:"));
  u8g2.print(rNominal);
}

void saveToEEPROM(int pos, int value) {
  byte high, low;
  high = (int)(value / 100);
  low = value - (high * 100 );
  if (EEPROM.read(pos) != 128) EEPROM.write(pos, 128);
  if (EEPROM.read(pos + 1) != high) EEPROM.write(pos + 1, high);
  if (EEPROM.read(pos + 2) != low) EEPROM.write(pos + 2, low);

}

void setAlarmTemp() {
  int loopcount = 1;
  int substart = millis();
  int subend = substart;
  int delaylength = 500;
  bool looping = false;
  while ((digitalRead(P_PREV) == LOW || digitalRead(P_NEXT) == LOW || looping) && ((subend - substart) < 3000)) {
    looping = true;

    // yield(); //FOR ESP8266 ONLY
    if (digitalRead(P_PREV) == HIGH && digitalRead(P_NEXT) == HIGH) loopcount = 1;
    u8g2.firstPage();
    do {

      u8g2.setFont(DEF_FONT);
      u8g2.setCursor(0, LINE_HEIGHT);
      u8g2.print(F("Alarme a:"));
      u8g2.setCursor(0, 2 * LINE_HEIGHT);
      u8g2.print(F("Alarm at:"));

      u8g2.setCursor(0, 3 * LINE_HEIGHT);

      u8g2.print(alarmTemp);
      u8g2.print(F("℃ / "));
      u8g2.print((int)(alarmTemp * 1.8 + 32));
      u8g2.print(F("℉"));
      u8g2.drawHLine(0, 3 * LINE_HEIGHT + 1, 128);

    } while ( u8g2.nextPage() );

    if (digitalRead(P_PREV) == LOW && alarmTemp > 0) {
      alarmTemp -= (1 + (int)(loopcount / 4));
      delay((int)(delaylength / (2 * loopcount)));
      substart = millis();
    } else if (digitalRead(P_NEXT) == LOW && alarmTemp < 400) {
      alarmTemp += (1 + (int)(loopcount / 4)) ;
      delay((int)(delaylength / (2 * loopcount)));
      substart = millis();
    } else if (digitalRead(P_MENU) == LOW){
      alarmState = 1;
      break;
    } else if (digitalRead(P_CANCEL) == LOW){
      alarmState = 0;
      if ( u8g2.userInterfaceMessage("Alarme/Alarm", "Mise a zero?", "Reset to zero?", " Oui/Yes \n Non/No ") == 1) alarmTemp = 0;
      break;
    }

    if (loopcount < 20) loopcount++;
    subend = millis();
    alarmState = 1;
  }
  if (looping) saveToEEPROM(3, alarmTemp);

}

void checkAlarm(int currentTemp, int currentDelta) {
  if (alarmState != 0) {
    int timeUntil = ((currentDelta > 0 && (alarmTemp * 100) > currentTemp) || (currentDelta < 0 && (alarmTemp * 100) < currentTemp) ? (int)((alarmTemp * 100 - currentTemp) / currentDelta) : -1);
    if (timeUntil != -1 && timeUntil < 120 && alarmState == 1) {
      playAlarm();
      alarmState = 2;
    }
    else if (alarmState == 2 && ((alarmTemp - 1) * 100) < currentTemp) {
      playAlarm();
    }
  }
}

void beep (int speakerPin, float noteFrequency, long noteDuration)
{
  int x;
  float microsecondsPerWave = 1000000 / noteFrequency;
  float millisecondsPerCycle = 1000 / (microsecondsPerWave * 2);
  float loopTime = noteDuration * millisecondsPerCycle;
  for (x = 0; x < loopTime; x++)
  {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(microsecondsPerWave);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(microsecondsPerWave);
  }
}
void playAlarm() {
  beep(P_SPEAKER, note_A7, 100); //A
  beep(P_SPEAKER, note_G7, 100); //G
  beep(P_SPEAKER, note_E7, 100); //E
  beep(P_SPEAKER, note_C7, 100); //C
  beep(P_SPEAKER, note_D7, 100); //D
  beep(P_SPEAKER, note_B7, 100); //B
  beep(P_SPEAKER, note_F7, 100); //F
  beep(P_SPEAKER, note_C8, 100); //C
  beep(P_SPEAKER, note_A7, 100); //A
  beep(P_SPEAKER, note_G7, 100); //G
  beep(P_SPEAKER, note_E7, 100); //E
  beep(P_SPEAKER, note_C7, 100); //C
  beep(P_SPEAKER, note_D7, 100); //D
  beep(P_SPEAKER, note_B7, 100); //B
  beep(P_SPEAKER, note_F7, 100); //F
  beep(P_SPEAKER, note_C8, 100); //C
}

void loop(void) {
  checkForFaults();
  int currentTemp = (int)(max3.temperature(rNominal, RREF) * 100);
  int currentDelta = currentTemp - prevTemp;

  prevTemp = currentTemp;

  //Show current temperature and mode-specific displays
  u8g2.firstPage();
  do {
    u8g2.setFont(DEF_FONT);
    u8g2.setCursor(0, LINE_HEIGHT);
    u8g2.print((float)currentTemp / 100.0);
    u8g2.print(F("℃ △"));
    u8g2.print((float)currentDelta / 100.0);
    if (alarmState != 0) {
      u8g2.setCursor(127 - u8g2.getStrWidth("◉"), LINE_HEIGHT);
      u8g2.print("◉");
    }
    u8g2.setCursor(0, 2 * LINE_HEIGHT);
    u8g2.print((float)currentTemp / 100.0 * 1.8 + 32.0);
    u8g2.print(F("℉ △"));
    u8g2.print((float)currentDelta / 100.0 * 1.8);
    u8g2.drawHLine(0, 2 * LINE_HEIGHT + 1, 128);
    if (tmode >= CANDY_MODE && tmode <= POULTRY_MODE) printStage(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    else if (tmode == ALARM_MODE) alarmMode(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    else if (tmode == CALIB_MODE) calibMode(2 * LINE_HEIGHT + 1);
  } while ( u8g2.nextPage() );

  
  checkAlarm(currentTemp, currentDelta);

  //Wait for and react to user input
  int starttime = millis();
  int endtime = starttime;
  while ((endtime - starttime) <= 1000)
  {
    //  yield(); //FOR ESP8266 ONLY
    int event = u8g2.getMenuEvent();
    if (event != 0) {
      if (event == U8X8_MSG_GPIO_MENU_SELECT) {
        if (tmode == CALIB_MODE) {
          if ( u8g2.userInterfaceMessage("Calibration", "Enregistrer?", "Save?", " Oui/Yes \n Non/No ") == 1) saveToEEPROM(0, (int)rNominal * 100);
          else rNominal = (float)EEPROM.read(1) + ((float)EEPROM.read(2) / 100);
        }
        if (tmode < ALARM_MODE) tmode++;
        else tmode = 0;
        break;
      }
      else if (event == U8X8_MSG_GPIO_MENU_PREV) {
        if (tmode == CALIB_MODE) rNominal += 0.05; //Prev is positive to make temp go down
        break;
      }
      else if (event == U8X8_MSG_GPIO_MENU_NEXT) {
        if (tmode == CALIB_MODE) rNominal -= 0.05; //Next is negative to make temp go up
        break;
      }
      else if (event == U8X8_MSG_GPIO_MENU_HOME) {
        if (alarmState != 0) {
          u8g2.setFont(DEF_FONT);
          if ( u8g2.userInterfaceMessage("Alarme/Alarm", "Desactiver?", "Deactivate?", " Oui/Yes \n Non/No ") == 1) alarmState = 0;
        }
        else if (tmode == DEFAULT_MODE) tmode = CALIB_MODE;
        else if (tmode != CALIB_MODE) {
          char aTemp[4];
          sprintf(aTemp, "%d℃ / %d℉", alarmTemp, (int)(alarmTemp * 1.8 + 32));
          u8g2.setFont(DEF_FONT);
          if ( u8g2.userInterfaceMessage("Alarme/Alarm", aTemp, "Activer/Activate?", " Oui/Yes \n Non/No ") == 1) alarmState = 1;
        }
        break;
      }
    }
    if (tmode == ALARM_MODE) setAlarmTemp();
    endtime = millis();
  }
}
