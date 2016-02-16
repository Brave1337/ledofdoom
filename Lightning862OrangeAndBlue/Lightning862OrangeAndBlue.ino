#include "Adafruit_WS2801.h"
#include "SPI.h" 
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
/*
// fake class
class cycleRibbonSize
{
  public:
    int fakerSpeed;
     uint8_t roboSpeed = 5;//temp
  cycleRibbonSize(speedinput)
  {
 //robospeed = whatever the speed is returned as
 unit8_t  = roboSpeed 

  }
  int figureOutSize(int f_speed);  
};

int cycleRibbonSize::figureOutSize(int f_speed)
{
  
}
*/




uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

uint8_t fakeSpeed;

uint8_t mode;
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
  strip.show();
  
  //mode set
  mode = 3;

  fakeSpeed = 129;
  
 colorWipe(Color( 0, 10, 123),0);

  strip.show();
}


void loop() {
  uint8_t passSpeed = (180 - fakeSpeed)/2;
  uint8_t pixelLength = fakeSpeed / 10; 
  
  // Some example procedures showing how to display to the pixels
  
  switch(mode){
    case 1:{
     rainbow(20);
      break;
    }  
    case 2: {
      rainbowCycle(1);
      break;
    }
    case 3: {
      colorWipe(Color( 0, 10, 123),0);
      colorWipe(Color( 120, 60, 0),10);
      break;
    }
    case 4: {//4 strip
      cycle(passSpeed, pixelLength);
      break;    
    }
    case 5: {
     meet(55);
    }
    default: break;  
  }
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
void cycle(uint8_t spd, uint8_t pixLen){
 
 uint8_t r = 0; 
  if (pixLen < 18){
    for( r ; r <= pixLen - 1 + strip.numPixels(); r++){
    strip.setPixelColor(r - pixLen, Color( 0, 10, 123));
    strip.setPixelColor( r , Color( 120, 60, 0));//orange
    strip.show();
    delay(spd);
    }
  } 
   else {
      colorWipe(Color( 120, 60, 0),0);
   }
    
}
void meet(uint8_t spd){

uint8_t pixLen = 4;
  for (uint8_t r = 0;r < (pixLen/2 + strip.numPixels())/2; r++){
    if ( r !=  ((pixLen / 2 + strip.numPixels())/2) ){
      strip.setPixelColor(r, Color( 120, 60, 0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color( 120, 60, 0));
      strip.setPixelColor(r - pixLen , Color( 0, 10, 123));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color( 0, 10, 123));
      strip.show();
      delay(spd);
    }
    else{
      delay(spd);
    }
  }
  for (uint8_t r = strip.numPixels()/2 +3;r < pixLen/2 + strip.numPixels(); r++){
  
      strip.setPixelColor(r, Color( 120, 60, 0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color( 120, 60, 0));
      strip.setPixelColor(r - pixLen , Color( 0, 10, 123));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color( 0, 10, 123));
      strip.show();
      delay(spd);
    }
}

