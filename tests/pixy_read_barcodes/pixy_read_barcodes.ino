#include <Pixy2.h>

Pixy2 pixy;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");

  // we need to initialize the pixy object
  pixy.init();
  pixy.setLamp(HIGH, HIGH);
  pixy.setLED(0, 0, 0);
  // Change to line tracking program
  pixy.changeProg("line");
}

void loop() {
    int8_t i;
    char buf[128];

    pixy.line.getAllFeatures();

    // print all barcodes
    for (i = 0; i < pixy.line.numBarcodes; i++)
    {
        sprintf(buf, "barcode %d: ", i);
        Serial.print(buf);
        pixy.line.barcodes[i].print();
    }
}
