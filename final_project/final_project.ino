// Importation des librairies utilisée :
#include <multi_channel_relay.h>
#include <Pixy2.h>

// Définition des ports utilisés :
#define CAPTOR_IF_POSITION_TRAY 3
#define RELAY_BM_MOTOR 4
#define CAPTOR_BM_END_TURN 2
#define FOUR_CHANNEL_RELAY_HBRIDGE 0x11;

// define objects
Multi_Channel_Relay HBridge;
Pixy2 pixy;

// set up variables
int Position;

void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");

  // Set pins modes
  pinMode(CAPTOR_IF_POSITION_TRAY, INPUT);
  pinMode(RELAY_BM_MOTOR, OUTPUT);
  pinMode(CAPTOR_BM_END_TURN, INPUT);

  // Setup pixy
  pixy.init();

  // Setup HBridge
  HBridge.begin(0x11);

  initProject();
  checkOrder();
}

void loop() {

}

void initProject() {
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
  digitalWrite(RELAY_BM_MOTOR, LOW); // Front descendant pour alimenter le moteur
  while (digitalRead(CAPTOR_BM_END_TURN) == LOW); // Attente capteur fin de tour
  digitalWrite(RELAY_BM_MOTOR, HIGH); // Front montant pour couper le moteur
}

void checkOrder() {
  if (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW) {
    rotateTrayCounterClockwise();
    while (digitalRead(CAPTOR_IF_POSITION_TRAY) == LOW);
    stopTray();
  }
  int value = readBarCode();
  if (value == -1) return 2

  for (uint8_t i = 0; i < 8; i++) {
    value += 1;
    if (readBarCode() != value) {
      if (value == -1) return 2
      return 0
    }
    rotateTrayCounterClockwise();
    while (digitalRead(CaptPosRef) == HIGH);
    while (digitalRead(CaptPosRef) == LOW);
    stopTray();
  }
  return 1
}

void readBarCode() {
  int8_t i;
  char buf[128];
  pixy.setLamp(HIGH, LOW);
  pixy.line.getAllFeatures();
  pixy.setLamp(LOW, LOW);

  if (lenght(pixy.line.barcodes) == 0) {
    return -1
  }
  return pixy.line.barcodes[i].m_code
}

void rotateTrayCounterClockwise() {
  HBridge.channelCtrl(0b1001);
}

void rotateTrayCounterClockwise() {
  HBridge.channelCtrl(0b0110);
}

void stopTray() {
  HBridge.channelCtrl(0b0000);
}
