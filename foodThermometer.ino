#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <U8g2lib.h>
#include <Wire.h>

#define RREF      431.0
#define RNOMINAL  97.25
//float RNOMINAL = 97.25;
#define DEF_FONT u8g2_font_7x13_t_symbols
#define SMALL_FONT u8g2_font_profont10_tr
#define SMLINE_HEIGHT 8
#define LINE_HEIGHT 11

#define DEFAULT_MODE 1
#define CANDY_MODE 2
#define BEEF_MODE 3
#define PORC_MODE 4
#define POULTRY_MODE 5
#define ALARM_MODE 6

typedef struct {
  char description [25];
} descriptionType;

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_MAX31865 max3 = Adafruit_MAX31865(13, 12, 15, 2);

int prevTemp;
byte tmode = DEFAULT_MODE;
int alarmTemp = 0;

void setup(void) {
  //Serial.begin(115200);
  pinMode(0, INPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  u8g2.begin(0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
  u8g2.enableUTF8Print();
  max3.begin(MAX31865_3WIRE);
  prevTemp = (int) (max3.temperature(RNOMINAL, RREF) * 100);
}

boolean checkForFaults() {
  uint8_t fault = max3.readFault();
  if (fault) {

    u8g2.firstPage();
    do {
      u8g2.setFont(SMALL_FONT);
      u8g2.setCursor(0, SMLINE_HEIGHT);
      u8g2.print(F("Erreur"));
      u8g2.setCursor(0, 2 * SMLINE_HEIGHT);

      u8g2.print(F("Fault 0x"));
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

void printStage(int currentTemp, int currentDelta, byte pos, int *stagesTemp, const descriptionType *stagesNames) {
  byte stage = 0;
  while (currentTemp > stagesTemp[stage] && stage < sizeof(stagesTemp)) {
    stage++;
  }
  unsigned int timeUntil = ((currentDelta > 0 && stage < sizeof(stagesTemp)) ? (int)((stagesTemp[stage] - currentTemp) / currentDelta) : 0);
  descriptionType currentStage;
  descriptionType nextStage;
  memcpy_P (&currentStage, &stagesNames [stage], sizeof currentStage);
  memcpy_P (&nextStage, &stagesNames [stage+1], sizeof currentStage);
  char timeToNextStage[17];
  snprintf(timeToNextStage, 17, "%d s avant", timeUntil);

  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print(currentStage.description);
  if (timeUntil > 0) {
    u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
    u8g2.print(timeToNextStage);
  }
  u8g2.setCursor(0, pos + 3 * SMLINE_HEIGHT);
  u8g2.print(F("Prochain stade"));
  u8g2.setCursor(0, pos + 4 * SMLINE_HEIGHT);
  u8g2.print(nextStage.description);
}

void candyMaking(int currentTemp, int currentDelta, byte pos) {
  const static descriptionType PROGMEM stagesNames[11]  = {{"Confiserie"},
                                                           {"Petit filet/Soft thread"},
                                                           {"Grand filet/Thread"},
                                                           {"Petit boule/Soft ball"},
                                                           {"Grand boule/Firm ball"},
                                                           {"Boule/Hard ball"},
                                                           {"Petit casse/Soft crack"},
                                                           {"Grand casse/Hard crack"},
                                                           {"Caramel"},
                                                           {"Sucre brule/Burnt sugar"},
                                                           {""}
                                                          };
  int stagesTemp[9] =  {10500, 10700, 11200, 11800.12500, 13500, 14500, 15000, 17100};
  printStage(currentTemp, currentDelta, pos, stagesTemp, stagesNames);

}
void beefCooking(int currentTemp, int currentDelta, byte pos) {
  const static descriptionType PROGMEM stagesNames[5]  = {{"Boeuf/Agneau"},
                                                          {"Saignant"},
                                                          {"A point"},
                                                          {"Bien cuit"},
                                                          {""}
                                                         };
  int stagesTemp[9] =  {6000, 7000, 7500};
  printStage(currentTemp, currentDelta, pos, stagesTemp, stagesNames);

}
void porcCooking(int currentTemp, int currentDelta, byte pos) {
  const static descriptionType PROGMEM stagesNames[4]  = {{"Porc"},
                                                          {"Fume"},
                                                          {"Frais"},
                                                          {""}
                                                         };
  int stagesTemp[9] =  {6000, 7500};
  printStage(currentTemp, currentDelta, pos, stagesTemp, stagesNames);

}
void poultryCooking(int currentTemp, int currentDelta, byte pos) {
  const static descriptionType PROGMEM stagesNames[5]  = {{"Volaille"},
                                                          {"Poulet min"},
                                                          {"Poulet max"},
                                                          {"Canard"},
                                                          {""}
                                                         };
  int stagesTemp[9] =  {8000, 8500, 9000};
  printStage(currentTemp, currentDelta, pos, stagesTemp, stagesNames);

}
void alarmMode(int currentTemp, int currentDelta, byte pos) {
  int timeUntil = 0;
  timeUntil = ((currentDelta > 0 && (alarmTemp * 100) > currentTemp)||(currentDelta < 0 && (alarmTemp * 100) < currentTemp) ? (int)((alarmTemp * 100 - currentTemp) / currentDelta) : 0);
  char timeToAlarm[17];
  snprintf(timeToAlarm, 17, "%d s avant", timeUntil);
  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print(timeToAlarm);
  u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
  u8g2.print(F("alarme a:"));
  u8g2.print(alarmTemp);
  u8g2.print(F("C / "));
  u8g2.print((int)(alarmTemp * 1.8 + 32));
  u8g2.print(F("F"));

}

void loop(void) {
  checkForFaults();
  //  while (Serial.available() > 0) {
  //    RNOMINAL = Serial.parseFloat();
  //      Serial.print("changed RNOMINAL to ");
  //      Serial.println(RNOMINAL);
  //  }

  char delta[8];
  char deltaF[8];
    char temperature[9];
  char temperatureF[9];
  int currentTemp = (int)(max3.temperature(RNOMINAL, RREF) * 100);
    int currentDelta = currentTemp - prevTemp;

  snprintf(temperature, 9, "%.2fC ", ((float)currentTemp) / 100);
  snprintf(temperatureF, 9, "%.2fF ", (((float)currentTemp) / 100) * 1.8 + 32);
  if (currentDelta < 0) {
    snprintf( delta, 8, "▽%.2f", ((float)currentDelta * -1) / 100);
    snprintf( deltaF, 8, "▽%.2f", (((float)currentDelta) * -1 / 100) * 1.8);
  } else {
    snprintf( delta, 8, "△%.2f", ((float)currentDelta) / 100);
    snprintf( deltaF, 8, "△%.2f", (((float)currentDelta) / 100) * 1.8);
  }
  prevTemp = currentTemp;

  u8g2.firstPage();
  do {

    u8g2.setFont(DEF_FONT);
    int offset = (int)((127 - (u8g2.getStrWidth(delta) + u8g2.getStrWidth(temperature))) / 2);
    u8g2.setCursor(offset, LINE_HEIGHT);
    u8g2.print(temperature);
    u8g2.print(delta);
    offset = (int)((127 - (u8g2.getStrWidth(deltaF) + u8g2.getStrWidth(temperatureF))) / 2);
    u8g2.setCursor(offset, 2 * LINE_HEIGHT);
    u8g2.print(temperatureF);
    u8g2.print(deltaF);
    u8g2.drawHLine(0, 2 * LINE_HEIGHT + 1, 128);
    if (tmode == CANDY_MODE) candyMaking(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    if (tmode == BEEF_MODE) beefCooking(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    if (tmode == PORC_MODE) porcCooking(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    if (tmode == POULTRY_MODE) poultryCooking(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    if (tmode == ALARM_MODE) alarmMode(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
  } while ( u8g2.nextPage() );

  int starttime = millis();
  int endtime = starttime;
  while ((endtime - starttime) <= 1000)
  {
    yield(); //FOR ESP8266 ONLY
    int event = u8g2.getMenuEvent();
    if (event != 0) {
      if (event == U8X8_MSG_GPIO_MENU_SELECT) {
        if (tmode < ALARM_MODE) tmode++;
        else tmode = 1;
        break;
      }

    }
    int loopcount = 1;
    while ((digitalRead(14) == LOW || digitalRead(16) == LOW) && tmode == ALARM_MODE) {
      int substart = millis();
      int subend = substart;
      int delaylength = 500;
      while ((subend - substart) < 3000) {
        yield(); //FOR ESP8266 ONLY
        if (digitalRead(14) == HIGH && digitalRead(16) == HIGH) loopcount = 1;
        u8g2.firstPage();
        do {

          u8g2.setFont(DEF_FONT);
          u8g2.setCursor(0, LINE_HEIGHT);
          u8g2.print(F("Alarme pour"));
          u8g2.setCursor(0, 2 * LINE_HEIGHT);
          u8g2.print(alarmTemp);
          u8g2.print(F("C / "));
          u8g2.print((int)(alarmTemp * 1.8 + 32));
          u8g2.print(F("F"));
          u8g2.drawHLine(0, 2 * LINE_HEIGHT + 1, 128);

        } while ( u8g2.nextPage() );

        if (digitalRead(14) == LOW && alarmTemp > 0) {
          alarmTemp -= 1;
          delay((int)(delaylength / (2 * loopcount)));
        } else if (digitalRead(16) == LOW && alarmTemp < 400) {
          alarmTemp += 1;
          delay((int)(delaylength / (2 * loopcount)));
        }

        if (loopcount < 20) loopcount++;
        if (digitalRead(14) == LOW || digitalRead(16) == LOW) substart = millis();
        subend = millis();
      }
    }
    endtime = millis();
  }
}
