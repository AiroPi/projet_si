#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd affi_lcd;

const int colorR = 0;
const int colorG =255;
const int colorB = 0;

void setup() {
    // Definition du nb de colonnes et lignes de l'afficheur LCD :
    affi_lcd.begin(16, 2);

    affi_lcd.setRGB(colorR, colorG, colorB);

    // Affiche un message.
    affi_lcd.print(" Projet POKER ! ");

    delay(1000);
}

void loop() {
    // Place le curseur en colonne 8, ligne 1
    // Note: ligne 1 est la seconde ligne, la premiere a pour numero 0 :
    affi_lcd.setCursor(8, 1);
    // Affiche le nombre de sencodes depuis le dernier RESET :
    affi_lcd.print(millis() / 1000);

    delay(100);
}
