// Importation des librairies utilisée :
#include <multi_channel_relay.h>
#include <Pixy2.h>
#include <SoftwareSerial.h>
#include <rgb_lcd.h>
#include <Wire.h>

// Définition des ports utilisés :
#define CAPTOR_IF_POSITION_TRAY 3
#define RELAY_BM_MOTOR 4
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
  Serial.println(getToken());

  start();
}

void loop()
{
  int token = getToken();
  goToGoal(token);
}

void start()
{
  lcdDisplay.setCursor(1, 0);
  lcdDisplay.print("Appuyez bouton 1");
  while (digitalRead(CAPTOR_BUTTON_1) == LOW)
    ;
  initialize();
}

void initialize()
{
  /*
  Function to initialize all default values of the project
  */

  // Stop the tray motor
  HBridge.channelCtrl(0b0000);

  // Turn off the light
  pixy.setLamp(LOW, LOW);
  pixy.setLED(0, 0, 0);
  pixy.changeProg("line");

  // Set BM motor initial position
  ejectToken();

  for (int i = 0; i < 8; i++)
  {
    nbTokens[i] = 0;
  }

  while (true)
  {
    int orderStatus = checkOrder();
    if (orderStatus == 2)
    {
      // TODO: message "Boite manquante\nAppuyez bouton 1"
      while (digitalRead(CAPTOR_BUTTON_1) == LOW)
        ;
    }
    else if (orderStatus == 1)
    {
      // TODO: message "Mauvais ordre\nAppuyez bouton 1"
      while (digitalRead(CAPTOR_BUTTON_1) == LOW)
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
    rotateTrayCounterClockwise();
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW)
      ;
    stopTray();
  }
  int value = readBarCode();
  if (value == -1)
  {
    return 2;
  }

  for (uint8_t i = 0; i < 8; i++)
  {
    value += 1;
    if (readBarCode() != value)
    {
      if (value == -1)
        return 2;
      return 0;
    }
    rotateTrayCounterClockwise();
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == HIGH)
      ;
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW)
      ;
    stopTray();
  }
  position = value;
  return 1;
}

int readBarCode()
{
  int8_t i;
  char buf[128];
  pixy.setLamp(HIGH, LOW);
  pixy.line.getAllFeatures();
  pixy.setLamp(LOW, LOW);

  if (sizeof(pixy.line.barcodes) == 0)
  {
    return -1;
  }
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
