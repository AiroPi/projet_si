#define Button 8
#define Engine 3

void setup() {
  Serial.begin(115200);
  pinMode(Button, INPUT_PULLUP);
  pinMode(Engine, OUTPUT);
  // Definition de l'adresse I2C de la carte des relais
  //relay.begin(0x11);
  //delay(1000);
  Serial.println("Debut du test");
}

void loop() {
  //if (digitalRead(Button)) {
  //  Serial.println("Ca tourne");
  //  digitalWrite(Engine,HIGH);
  //} else {
  //  Serial.println("Ce tourne pas");
  //  digitalWrite(Engine,LOW);
  //}
  digitalWrite(Engine,LOW);
  delay(1000);
  digitalWrite(Engine,HIGH);
  delay(1000);
}
