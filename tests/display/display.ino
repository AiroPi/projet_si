#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd affi_lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
bool screen = 0;
bool nbtoken = 0;

void setup()
{
  // Definition du nb de colonnes et lignes de l'afficheur LCD :
  affi_lcd.begin(16, 2);
  pinMode(Button, INPUT_PULLUP);
  affi_lcd.setRGB(colorR, colorG, colorB);
  // Affiche un message.
  affi_lcd.setCursor(1, 0);
  affi_lcd.print("appuyez");
  setScreen();
  setNbtoken();

  delay(1000);
}

void loop()
{
  if (digitalRead(Button) == 1)
  {
    screen = !screen;
    setScreen();
    nbtoken = !nbtoken;
    setNbtoken();
    while (digitalRead(Button) == 1)
      ;
  }
  for (int i = 0; i < 20; i = i + 1)
  {
    valtok[i] = i;
  }
}

void setScreen()
{
  affi_lcd.clear();

  if (screen == 0)
  {
    affi_lcd.setCursor(0, 0);
    affi_lcd.print("1000:   |500 :  ");
    affi_lcd.setCursor(0, 1);
    affi_lcd.print("100 :   |50  :  ");
  }
  else
  {
    affi_lcd.setCursor(0, 0);
    affi_lcd.print("25  :   |10  :  ");
    affi_lcd.setCursor(0, 1);
    affi_lcd.print("5   :   |1   :  ");
  }
}
void setNbtoken()
{
  if (screen == 0)
  {
    affi_lcd.setCursor(6, 0);
    affi_lcd.print(valtok[7]);
    affi_lcd.setCursor(14, 0);
    affi_lcd.print(valtok[6]);
    affi_lcd.setCursor(6, 1);
    affi_lcd.print(valtok[5]);
    affi_lcd.setCursor(14, 1);
    affi_lcd.print(valtok[4]);
  }
  else
  {
    affi_lcd.setCursor(6, 0);
    affi_lcd.print(valtok[3]);
    affi_lcd.setCursor(14, 0);
    affi_lcd.print(valtok[2]);
    affi_lcd.setCursor(6, 1);
    affi_lcd.print(valtok[1]);
    affi_lcd.setCursor(14, 1);
    affi_lcd.print(valtok[0]);
  }
}
