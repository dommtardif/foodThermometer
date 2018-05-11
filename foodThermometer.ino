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

#define SOFTTHREAD_MIN 10500
#define THREAD_MIN 10700
#define SOFTBALL_MIN 11200
#define FIRMBALL_MIN 11800
#define HARDBALL_MIN 12500
#define SOFTCRACK_MIN 13500
#define HARDCRACK_MIN 14500
#define CARAMEL_MIN 15000
#define BURNTSUGAR_MIN 17100

#define DEFAULT_MODE 1
#define CANDY_MODE 2
#define MEAT_MODE 3
#define ALARM_MODE 4

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_MAX31865 max3 = Adafruit_MAX31865(13, 12, 15, 2);

int prevTemp;
byte tmode = DEFAULT_MODE;
int alarmTemp = 0;

void setup(void) {
  Serial.begin(115200);
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
    max3.clearFault();
    return true;
  }
  return false;
}

void candyMaking(int currentTemp, int currentDelta, byte pos) {
  char SOFTTHREAD[]  = "Petit filet/Soft thread";
  char THREAD[]      = "Grand filet/Thread";
  char SOFTBALL[]    = "Petit boule/Soft ball";
  char FIRMBALL[]    = "Grand boule/Firm ball";
  char HARDBALL[]    = "Boule/Hard ball";
  char SOFTCRACK[]   = "Petit casse/Soft crack";
  char HARDCRACK[]   = "Grand casse/Hard crack";
  char CARAMEL[]     = "Caramel";
  char BURNTSUGAR[]  = "Sucre brule/Burnt sugar";
  unsigned int timeUntil;
  const char* currentStage;
  const char* nextStage;


  if (currentTemp < SOFTTHREAD_MIN) {
    currentStage = "";
    timeUntil = ((currentDelta > 0) ? (int)((SOFTTHREAD_MIN - currentTemp) / currentDelta) : 0);
    nextStage = SOFTTHREAD;
  } else if (currentTemp < THREAD_MIN) {
    currentStage = SOFTTHREAD;
    timeUntil = ((currentDelta > 0) ? (int)((THREAD_MIN - currentTemp) / currentDelta) : 0);
    nextStage = THREAD;
  } else if (currentTemp < SOFTBALL_MIN) {
    currentStage = THREAD;
    timeUntil = ((currentDelta > 0) ? (int)((SOFTBALL_MIN - currentTemp) / currentDelta) : 0);
    nextStage = SOFTBALL;
  } else if (currentTemp < FIRMBALL_MIN) {
    currentStage = SOFTBALL;
    timeUntil = ((currentDelta > 0) ? (int)((FIRMBALL_MIN - currentTemp) / currentDelta) : 0);
    nextStage = FIRMBALL;
  } else if (currentTemp < HARDBALL_MIN) {
    currentStage = FIRMBALL;
    timeUntil = ((currentDelta > 0) ? (int)((HARDBALL_MIN - currentTemp) / currentDelta) : 0);
    nextStage = HARDBALL;
  } else if (currentTemp < SOFTCRACK_MIN) {
    currentStage = HARDBALL;
    timeUntil = ((currentDelta > 0) ? (int)((SOFTCRACK_MIN - currentTemp) / currentDelta) : 0);
    nextStage = SOFTCRACK;
  } else if (currentTemp < HARDCRACK_MIN) {
    currentStage = SOFTCRACK;
    timeUntil = ((currentDelta > 0) ? (int)((HARDCRACK_MIN - currentTemp) / currentDelta) : 0);
    nextStage = HARDCRACK;
  } else if (currentTemp < CARAMEL_MIN) {
    currentStage = HARDCRACK;
    timeUntil = ((currentDelta > 0) ? (int)((CARAMEL_MIN - currentTemp) / currentDelta) : 0);
    nextStage = CARAMEL;
  } else if (currentTemp < BURNTSUGAR_MIN) {
    currentStage = CARAMEL;
    timeUntil = ((currentDelta > 0) ? (int)((BURNTSUGAR_MIN - currentTemp) / currentDelta) : 0);
    nextStage = BURNTSUGAR;
  } else {
    currentStage = BURNTSUGAR;
    timeUntil = 0;
    nextStage = "";
  }

  char timeToNextStage[17];
  snprintf(timeToNextStage, 17, "%d s avant", timeUntil);

  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print(currentStage);
  if (timeUntil > 0) {
    u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
    u8g2.print(timeToNextStage);
  }
  u8g2.setCursor(0, pos + 3 * SMLINE_HEIGHT);
  u8g2.print(F("Prochain stade"));
  u8g2.setCursor(0, pos + 4 * SMLINE_HEIGHT);
  u8g2.print(nextStage);


}
void meatCooking(int currentTemp, int currentDelta, byte pos) {
  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print("Mode viande");
}
void alarmMode(int currentTemp, int currentDelta, byte pos) {
  int timeUntil = 0;
  timeUntil = ((currentDelta > 0 && (alarmTemp * 100) > currentTemp) ? (int)((alarmTemp * 100 - currentTemp) / currentDelta) : 0);
  char timeToAlarm[17];
  snprintf(timeToAlarm, 17, "%d s avant", timeUntil);
  u8g2.setFont(SMALL_FONT);
  u8g2.setCursor(0, pos + SMLINE_HEIGHT);
  u8g2.print(timeToAlarm);
  u8g2.setCursor(0, pos + 2 * SMLINE_HEIGHT);
  u8g2.print("alarme a:");
  u8g2.print(alarmTemp);
  u8g2.print("C / ");
  u8g2.print((int)(alarmTemp * 1.8 + 32));
  u8g2.print("F");

}

void loop(void) {
  if (checkForFaults()) {
    u8g2.firstPage();
    do {
      u8g2.setFont(DEF_FONT);
      u8g2.setCursor(0, LINE_HEIGHT);
      u8g2.print("Erreur");
    } while ( u8g2.nextPage() );
    delay(2000);
  }
  //  while (Serial.available() > 0) {
  //    RNOMINAL = Serial.parseFloat();
  //      Serial.print("changed RNOMINAL to ");
  //      Serial.println(RNOMINAL);
  //  }


  int currentTemp = (int)(max3.temperature(RNOMINAL, RREF) * 100);
  char temperature[9];
  snprintf(temperature, 9, "%.2fC ", ((float)currentTemp) / 100);
  char temperatureF[9];
  snprintf(temperatureF, 9, "%.2fF ", (((float)currentTemp) / 100) * 1.8 + 32);

  int currentDelta = currentTemp - prevTemp;

  char delta[8];
  char deltaF[8];

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
    if (tmode == MEAT_MODE) meatCooking(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
    if (tmode == ALARM_MODE) alarmMode(currentTemp, currentDelta, 2 * LINE_HEIGHT + 1);
  } while ( u8g2.nextPage() );

  int starttime = millis();
  int endtime = starttime;
  while ((endtime - starttime) <= 1000)
  {
    int event = u8g2.getMenuEvent();
    if (event != 0) {
      if (event == U8X8_MSG_GPIO_MENU_SELECT) {
        if (tmode < 4) tmode++;
        else tmode = 1;
        break;
      }

    }
    int loopcount = 1;
    while ((digitalRead(14) == LOW || digitalRead(16) == LOW) && tmode == 4) {
      int substart = millis();
      int subend = substart;
      int delaylength = 500;
      while ((subend - substart) < 3000) {
        //yield(); //FOR ESP8266 ONLY
        if (digitalRead(14) == HIGH && digitalRead(16) == HIGH) loopcount = 1;
        u8g2.firstPage();
        do {

          u8g2.setFont(DEF_FONT);
          u8g2.setCursor(0, LINE_HEIGHT);
          u8g2.print("Alarme pour");
          u8g2.setCursor(0, 2 * LINE_HEIGHT);
          u8g2.print(alarmTemp);
          u8g2.print("C / ");
          u8g2.print((int)(alarmTemp * 1.8 + 32));
          u8g2.print("F");
          u8g2.drawHLine(0, 2 * LINE_HEIGHT + 1, 128);

        } while ( u8g2.nextPage() );

        if (digitalRead(14) == LOW) {
          if (alarmTemp > 0) {
            alarmTemp -= 1;
            delay((int)(delaylength / (2 * loopcount)));
          }

        } else if (digitalRead(16) == LOW) {
          if (alarmTemp < 400) {
            alarmTemp += 1;
            delay((int)(delaylength / (2 * loopcount)));
          }
        }
        if (loopcount < 10) loopcount++;
        if (digitalRead(14) == LOW || digitalRead(16) == LOW) substart = millis();
        subend = millis();

      }
    }
    endtime = millis();
  }
}
