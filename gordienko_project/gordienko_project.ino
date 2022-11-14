#include <LiquidCrystal.h>
#include "DHT.h"

// variables
struct pos
{
  int x;
  int y;
};

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

pos defaultPos {0, 0};
pos hrsPos {0, 0};
pos hrs2Pos {1, 0};
pos dots1Pos {2, 0};
pos minsPos {3, 0};
pos mins2Pos {4, 0};
pos dots2Pos {5, 0};
pos secPos {6, 0};
pos clearSecPos {7, 0};
pos clearMinsPos {4, 0};
pos clearHrsPos {1, 0};
pos temperText {0, 1};
pos temperPos {3, 1};
pos humText {5, 1};
pos humPos {8, 1};

int sec = 0;
int mins = 0;
int hrs = 0;

int delayT = 1000; // default = 1000

int ringMins = -1;
int ringHrs = -1;

const int BUTTON_MINS = 13;
const int BUTTON_HRS = 8;
const int PIEZOPIN = 7;
const int DHTPIN = 10;

DHT dht(DHTPIN, DHT11);

float temperature = 0.0;
float hum = 0.0;

bool flag = false;

bool minButton;
bool hrsButton;

uint32_t btnTimer = 0;

// program's body

void setup()
{
  //Serial.begin(9600); // debug!!!
  lcd.begin(8, 2);
  dht.begin();
  pinMode(BUTTON_MINS, INPUT_PULLUP);
  pinMode(BUTTON_HRS, INPUT_PULLUP);
  pinMode(PIEZOPIN, OUTPUT);
}

void loop()
{
  readData();
  clockTick();
  proceedButtons();
  printing();
}

// functions

void clockTick()
{
  delay(delayT);
  sec++;

  if (sec == 60)
  {
    sec = 0;
    printText(" ", clearSecPos);
    mins++;
  }

  if (mins == 60)
  {
    mins = 0;
    printText(" ", clearMinsPos);
    hrs++;

    tone(PIEZOPIN, 1000, 1000);
  }

  if (hrs == 24)
  {
    hrs = 0;
    printText(" ", clearHrsPos);
  }

}

void proceedButtons()
{
  minButton = !digitalRead(BUTTON_MINS);
  hrsButton = !digitalRead(BUTTON_HRS);

  valueButton(minButton, mins);
  valueButton(hrsButton, hrs);
}

void readData()
{
  temperature = dht.readTemperature();
  hum = dht.readHumidity();
}

void printing()
{
  
  printText(hrs, hrsPos);
  printText(":", dots1Pos);
  printText(mins, minsPos);
  printText(":", dots2Pos);
  printText(sec, secPos);

  printText("T: ", temperText);
  printText(temperature, temperPos);
  printText("H: ", humText);
  printText(hum, humPos);
}

void valueButton(bool button, int & value)
{
  if (button && !flag && millis() - btnTimer > 100)
  {
    flag = true;
    btnTimer = millis();
  }

  if (button && flag && millis() - btnTimer > 500)
  {
    btnTimer = millis();
    value++;
  }

}

void initLcd(int cols, int rows)
{
  lcd.begin(cols, rows);
}

void printText(const char * text, pos & startPos)
{
  lcd.setCursor(startPos.x, startPos.y);
  lcd.print(text);
}

// overloaded

void printText(int text, pos & startPos)
{
  lcd.setCursor(startPos.x, startPos.y);
  lcd.print(text);
}
