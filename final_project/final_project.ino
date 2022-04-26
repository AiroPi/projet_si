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
Pixy2 Pixy;

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
    Pixy.init();

  // Setup HBridge
    HBridge.begin(0x11);

  initProject();

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
  Pixy.setLamp(LOW, LOW);

  // Set BM motor initial position
  digitalWrite(RELAY_BM_MOTOR, LOW); // Front descendant pour alimenter le moteur
  while (digitalRead(CAPTOR_BM_END_TURN) == LOW); // Attente capteur fin de tour
  digitalWrite(RELAY_BM_MOTOR, HIGH); // Front montant pour couper le moteur
}
