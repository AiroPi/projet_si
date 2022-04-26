// Importation des librairies utilisée :
#include <multi_channel_relay.h>
#include <Pixy2.h>

// Définition des ports utilisés :
#define CAPTOR_IF_POSITION_TRAY 3
#define MOTOR_BM_RELAY 4
#define CAPTOR_BM_END_TURN 2

Multi_Channel_Relay HBridge;
Pixy2 pixy;


void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");

  // Setup pixy
    pixy.init();
}

void loop() {
  // put your main code here, to run repeatedly:

}
