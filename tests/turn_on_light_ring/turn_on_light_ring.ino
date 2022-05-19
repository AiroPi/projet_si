#include "Adafruit_NeoPixel.h"

#define LedRing 7

#define NUMPIXELS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LedRing, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup()
{
    pixels.setBrightness(40);
    pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{

    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
        pixels.show();
    }

    // for (int i = 0; i < NUMPIXELS; i++)
    // {

    //     // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    //     pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.

    //     pixels.show(); // This sends the updated pixel color to the hardware.

    //     delay(delayval); // Delay for a period of time (in milliseconds).
    // }
}
