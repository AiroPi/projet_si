#include <multi_channel_relay.h>

#define BPHoraire 2
#define BPAntiHoraire 3
#define CaptPosRef 5

Multi_Channel_Relay relay;

void setup() {
  Serial.begin(115200);
  pinMode(BPHoraire, INPUT);
  pinMode(BPAntiHoraire, INPUT);
  pinMode(CaptPosRef, INPUT);
  // Definition de l'adresse I2C de la carte des relais
  relay.begin(0x11);
  delay(1000);
}

void loop() {
  if (digitalRead(BPHoraire)) {
    relay.channelCtrl(0b1001);
  } else if (digitalRead(BPAntiHoraire)) {
    relay.channelCtrl(0b0110);
  } else {
    relay.channelCtrl(0b0000);
  }
  Serial.println("Capt pos Ref : " + String(digitalRead(CaptPosRef)));
  delay(100);
}
