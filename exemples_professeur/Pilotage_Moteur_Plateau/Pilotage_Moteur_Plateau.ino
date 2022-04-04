#include <multi_channel_relay.h>

#define BPHoraire 8
#define BPAntiHoraire 6
#define CaptPosRef 3

Multi_Channel_Relay pontH;

void setup() {
  Serial.begin(115200);
  pinMode(BPHoraire, INPUT);
  pinMode(BPAntiHoraire, INPUT);
  pinMode(CaptPosRef, INPUT);
  // Definition de l'adresse I2C de la carte des relais
  pontH.begin(0x11);
  delay(1000);
}

void loop() {
  if (digitalRead(BPHoraire)) {
    pontH.channelCtrl(0b1001);
  } else if (digitalRead(BPAntiHoraire)) {
    pontH.channelCtrl(0b0110);
  } else {
    pontH.channelCtrl(0b0000);
  }
  Serial.println("Capt pos Ref : " + String(digitalRead(CaptPosRef)));
  delay(100);
}
