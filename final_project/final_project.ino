// Importation des librairies utilisée :
#include <multi_channel_relay.h>
#include <Pixy2.h>
#include <SoftwareSerial.h>
#include <rgb_lcd.h>
#include <Wire.h>

// Définition des ports utilisés :
#define CAPTOR_IF_POSITION_TRAY 5
#define RELAY_BM_MOTOR A3
#define CAPTOR_BM_END_TURN 2
#define CAPTOR_BUTTON_1 6
#define CAPTOR_BUTTON_2 8
#define FOUR_CHANNEL_RELAY_HBRIDGE 0x11;

// define objects
Multi_Channel_Relay HBridge;
Pixy2 pixy;
SoftwareSerial raspberrySerial(3, 4); // RX, TX
rgb_lcd lcdDisplay;

// set up constants
const int TOKENS_VALUES[8] = {1, 5, 10, 25, 50, 100, 500, 1000};

// set up variables
int position;
int nbTokens[8];
bool screenState = 0; // state 0 for 1 to 25, state 1 for 50 to 1000

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");

  // Set pins modes
  pinMode(CAPTOR_IF_POSITION_TRAY, INPUT);
  pinMode(RELAY_BM_MOTOR, OUTPUT);
  pinMode(CAPTOR_BM_END_TURN, INPUT);
  pinMode(CAPTOR_BUTTON_1, INPUT_PULLUP);
  pinMode(CAPTOR_BUTTON_2, INPUT_PULLUP);

  // Setup pixy
  pixy.init();

  // Setup HBridge
  HBridge.begin(0x11);

  // Setup LCD display
  lcdDisplay.begin(16, 2);

  raspberrySerial.begin(9600);
  // Serial.println(getToken());

  start();
}

void loop()
{
  int token = getToken();
  nbTokens[token] += 1;
  goToGoal(token);
}

void start()
{
  // Stop the tray motor
  HBridge.channelCtrl(0b0000);

  // Turn off the light
  pixy.setLamp(LOW, LOW);
  pixy.setLED(0, 0, 0);
  pixy.changeProg("line");

  // stop BM motor
  digitalWrite(RELAY_BM_MOTOR, HIGH);

  lcdDisplay.setCursor(1, 0);
  lcdDisplay.print("Appuyez bouton 1");
  while (digitalRead(CAPTOR_BUTTON_1) == LOW)
    ;
  while (digitalRead(CAPTOR_BUTTON_1) == HIGH)
    ;
  initialize();
}

void initialize()
{
  /*
  Function to initialize all default values of the project
  */

  // Set BM motor initial position
  ejectToken();

  for (int i = 0; i < 8; i++)
  {
    nbTokens[i] = 0;
  }

  screenState = 0;
  setScreen(screenState);

  while (true)
  {
    int orderStatus = checkOrder();

    if (orderStatus == 2)
    {
      lcdDisplay.clear();
      lcdDisplay.setCursor(0, 0);
      lcdDisplay.print("Boite manquante");
      lcdDisplay.setCursor(0, 1);
      lcdDisplay.print("Appuyez bouton 1");
      while (digitalRead(CAPTOR_BUTTON_1) == LOW)
        ;
      while (digitalRead(CAPTOR_BUTTON_1) == HIGH)
        ;
    }
    else if (orderStatus == 0)
    {
      lcdDisplay.clear();
      lcdDisplay.setCursor(0, 0);
      lcdDisplay.print("Mauvais ordre");
      lcdDisplay.setCursor(0, 1);
      lcdDisplay.print("Appuyez bouton 1");

      while (digitalRead(CAPTOR_BUTTON_1) == LOW)
        ;
      while (digitalRead(CAPTOR_BUTTON_1) == HIGH)
        ;
    }
    else
    {
      break;
    }
  }
}

int checkOrder()
{
  /*
  Function to check if the boxes are ine the good order.
  Return 2 if a box is missing, 1 if the boxes are in the good order, 0 if the boxes are in the wrong order
  */
  if (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW)
  {
    rotateTrayClockwise();
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW)
    {
      buttonsInterpreter();
    };
    stopTray();
  }
  int value = readBarCode();
  if (value == -1)
  {
    return 2;
  }

  for (uint8_t i = 0; i < 8; i++)
  {
    rotateTrayCounterClockwise();
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == HIGH)
    {
      buttonsInterpreter();
    };
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW)
    {
      buttonsInterpreter();
    };
    stopTray();
    value += 1;
    if (value > 7)
    {
      value -= 8;
    }
    int new_value = readBarCode();
    if (new_value != value)
    {
      if (new_value == -1)
        return 2;
      return 0;
    }
  }
  position = value + 2;
  if (position > 7)
  {
    position -= 8;
  }
  return 1;
}

int readBarCode()
{
  int8_t i;
  char buf[128];
  pixy.setLamp(HIGH, LOW);
  unsigned long start = millis();
  while (millis() - start < 500)
  {
    buttonsInterpreter();
  }
  pixy.line.getAllFeatures();
  pixy.setLamp(LOW, LOW);

  if (pixy.line.numBarcodes == 0)
  {
    Serial.println(-1);
    return -1;
  }
  Serial.println(pixy.line.barcodes[i].m_code);

  return pixy.line.barcodes[i].m_code;
}

void rotateTrayCounterClockwise()
{
  HBridge.channelCtrl(0b1001);
}

void rotateTrayClockwise()
{
  HBridge.channelCtrl(0b0110);
}

void stopTray()
{
  HBridge.channelCtrl(0b0000);
}

void ejectToken()
{
  digitalWrite(RELAY_BM_MOTOR, LOW); // Front descendant pour alimenter le moteur
  while (digitalRead(CAPTOR_BM_END_TURN) == LOW)
    ;                                 // Attente capteur fin de tour
  digitalWrite(RELAY_BM_MOTOR, HIGH); // Front montant pour couper le moteur
}

int getToken()
{
  raspberrySerial.write("get");
  while (!raspberrySerial.available())
    ;
  int index_val = raspberrySerial.read();
  return index_val;
}

void goToGoal(int goal)
{
  int diff = goal - position;
  int moveGoal;

  if (diff == 0)
  {
    moveGoal = 0;
  }
  else if (diff < -4)
  {
    rotateTrayClockwise();
    moveGoal = 8 - abs(diff);
  }
  else if (diff < 0)
  {
    rotateTrayCounterClockwise();
    moveGoal = abs(diff);
  }
  else if (diff < 4)
  {
    rotateTrayClockwise();
    moveGoal = diff;
  }
  else
  {
    rotateTrayCounterClockwise();
    moveGoal = 8 - diff;
  }
  position = goal;

  for (int i = 0; i < moveGoal; i++)
  {
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == HIGH)
      ;
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW)
      ;
  }
  stopTray();

  ejectToken();
}

void buttonsInterpreter()
{
  int action = 0; // No button pressed. Nothing to do.
  if (digitalRead(CAPTOR_BUTTON_1) == HIGH)
  {
    action = 1; // Button 1 pressed. Pause the program.
    while (digitalRead(CAPTOR_BUTTON_1) == HIGH)
    {
      while (digitalRead(CAPTOR_BUTTON_2 == HIGH))
      {
        action = 3; // Button 1 and button 2 pressed. Reset the program.
      }
    }
  }
  else if (digitalRead(CAPTOR_BUTTON_2) == HIGH)
  {
    action = 2; // Button 2 pressed. Switch the screen.
    while (digitalRead(CAPTOR_BUTTON_2) == HIGH)
    {
      while (digitalRead(CAPTOR_BUTTON_1) == HIGH)
      {
        action = 3;
      }
    }
  }

  if (action == 1)
  {
    pauseProgram();
  }
  else if (action == 2)
  {
    switchScreen();
  }
  else if (action == 3)
  {
    start();
  }
}

void pauseProgram()
{
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("Pause");
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Appuyez bouton 1");
  while (digitalRead(CAPTOR_BUTTON_1) == LOW)
    ;
  while (digitalRead(CAPTOR_BUTTON_1) == HIGH)
    ;
}

void switchScreen()
{
  screenState != screenState;
  setScreen(screenState);
}

void setScreen(int screen)
{
  lcdDisplay.clear();

  if (screen == 0)
  {
    lcdDisplay.setCursor(0, 0);
    lcdDisplay.print("1000:   |500 :  ");
    lcdDisplay.setCursor(0, 1);
    lcdDisplay.print("100 :   |50  :  ");

    lcdDisplay.setCursor(6, 0);
    lcdDisplay.print(nbTokens[7]);
    lcdDisplay.setCursor(14, 0);
    lcdDisplay.print(nbTokens[6]);
    lcdDisplay.setCursor(6, 1);
    lcdDisplay.print(nbTokens[5]);
    lcdDisplay.setCursor(14, 1);
    lcdDisplay.print(nbTokens[4]);
  }
  else
  {
    lcdDisplay.setCursor(0, 0);
    lcdDisplay.print("25  :   |10  :  ");
    lcdDisplay.setCursor(0, 1);
    lcdDisplay.print("5   :   |1   :  ");

    lcdDisplay.setCursor(6, 0);
    lcdDisplay.print(nbTokens[3]);
    lcdDisplay.setCursor(14, 0);
    lcdDisplay.print(nbTokens[2]);
    lcdDisplay.setCursor(6, 1);
    lcdDisplay.print(nbTokens[1]);
    lcdDisplay.setCursor(14, 1);
    lcdDisplay.print(nbTokens[0]);
  }
}
