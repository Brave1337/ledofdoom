#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

//fake class
/*class cycleRibbonSize
{
  public:
  cycleRibbonSize()
  {
  
  }
  int ribbonSize;
  int cycleSpeed;

  int figureOutSize(int f_speed);  
};

int testme::figureOutSize(int f_speed)
{
  return 0x420;
}
*/




uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels
uint8_t delLed[] = {0,1,2,3};
bool doColorWipe = true;
uint8_t cycleSpeed;
// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(32, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//Adafruit_WS2801 strip = Adafruit_WS2801(25);



void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  
 strip.begin();
 cycleSpeed = 21;
}

void loop() {
  cycleWithWipe(cycleSpeed, Color(0, 20, 255), Color(255, 100, 0));
}

 void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
int i, j;
 
 for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = b;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= r;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0); 
   }
   else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void cycle(uint8_t spd, uint32_t color1, uint32_t color2){
 
 uint32_t col1 = color1;
 
 uint32_t col2 = color2; 
 
 for( uint8_t i = 0; i <= + strip.numPixels(); i++){
 
 delLed[0] = delLed[1];
 delLed[1] = delLed[2];
 delLed[2] = delLed[3];
 delLed[3] = delLed[4];
 delLed[4] = i;
 
 strip.setPixelColor(delLed[0], col1);
 
 strip.setPixelColor(delLed[4] , col2);
 
 strip.show();
 
 delay(spd);
 }
 
 void cycleWithWipe(uint8_t waitTime, uint32_t color1, uint32_t color2){
  if(doColorWipe){
   colorWipe(0, color1);
   
   strip.setPixelColor( 0 , color2);
   
   strip.setPixelColor( 1 , color2);
   
   strip.setPixelColor( 2 , color2);
   
   strip.setPixelColor( 3 , color2);

   
   doColorWipe = false;
  }
 cycle(waitTime, color1, color2);
 }
 
 void cycleWithWipe(uint8_t waitTime, uint32_t initColor, uint32_t color1, uint32_t color2){
 if(doColorWipe){
   colorWipe(0, initColor);
   
   strip.setPixelColor( 0 , color2);
   
   strip.setPixelColor( 1 , color2);
   
   strip.setPixelColor( 2 , color2);
   
   strip.setPixelColor( 3 , color2);
   
   doColorWipe = false;
  }
 cycle(waitTime, color1, color2);
 }
}

