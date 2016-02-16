#include "Adafruit_WS2801.h"
#include <SPI.h> 
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

//global vars
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels
uint8_t mode;
Adafruit_WS2801 strip = Adafruit_WS2801(32, dataPin, clockPin);
              
              //SPI HELPER FUNCTIONS
//#####################################################              
//Initialize SPI slave.
void SlaveInit(void) {
  // Initialize SPI pins.
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
//pinMode(MISO, INPUT);
  pinMode(SS, INPUT);
 pinMode(MISO, INPUT);
  // Enable SPI as slave.
  SPCR = (1 << SPE);
}
// SPI Transfer.
byte SPItransfer(byte value) {
  SPDR = value;
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}
//############################################################
                 
                  //LED HElPER FUNCTIONS
//#############################################################
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
//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos){
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
  if (pixLen < 21){
    for( r ; r <= pixLen - 1 + strip.numPixels(); r++){
    strip.setPixelColor(r - pixLen, Color(0, 10, 123));
    strip.setPixelColor( r , Color(120, 30, 0));//orange
    strip.show();
    delay(spd);
    }
  } 
   else {
      colorWipe(Color(120, 30, 0),0);
   }
    
}
void meet(uint8_t spd){

uint8_t pixLen = 4;
  for (uint8_t r = 0;r < (pixLen/2 + strip.numPixels())/2; r++){
      strip.setPixelColor(r, Color(120, 30, 0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color(120, 30, 0));
      strip.setPixelColor(r - pixLen , Color(0, 10, 123));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color(0, 10, 123));
      strip.show();
      delay(spd);
    }
 
      delay(spd);
      delay(spd);
  
  for (uint8_t r = strip.numPixels()/2 + (pixLen/2)+1;r < pixLen/2 + strip.numPixels(); r++){
  
      strip.setPixelColor(r, Color(120, 30, 0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color(120, 30, 0));
      strip.setPixelColor(r - pixLen , Color(0, 10, 123));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color(0, 10, 123));
      strip.show();
      delay(spd);
    }
}
//####################################################################

// The setup() function runs each initialization or reset
void setup() {//only define theses for led
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
   clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif
  // Initialize serial for troubleshooting
  Serial.begin(9600);
    // Initialize SPI Slave.
  SlaveInit();
  //LED Initializing
  strip.begin();
  strip.show();
  colorWipe(Color(0, 10, 123),0);
  strip.show();
  //mode set
  mode = 0;
  mode = SPItransfer(255); 
}
  
// The loop function runs continuously after setup().
void loop() {
      // Write -1 slave response code and receive master command code
  uint8_t passSpeed;
  uint8_t pixelLength;
  
  mode = SPItransfer(255);
  byte passMode;
  if (mode <= 200){
    passMode = 1;
    passSpeed = (210 - mode);
  }
  if ((210 < mode) && (mode < 220)){
    passMode = 2;
    passSpeed= 220 - mode;
  }
  if ((220 <= mode)&& (mode < 230)){
    passMode = 3;
    passSpeed= 230 - mode;
  }
  if ((230 <= mode)&&(mode < 240)){
    passMode = 4;
    passSpeed= 240 - mode;
  }
  if ((240 <= mode)&&(mode < 256)){
    passMode = 5;
    passSpeed= 20 - mode;
  }
  pixelLength = 1 + (passSpeed / 10);
  switch(passMode){
     case 1: {//speedstrip strip
      cycle(passSpeed, pixelLength);
      break;
    }  
  
    case 2: {
      colorWipe(Color(0 , 10, 123), passSpeed);
      colorWipe(Color(120, 30 , 0), passSpeed);
      break;
    }
    case 3: {
      rainbowCycle(passSpeed);
      break;
    }
    case 4:{
     rainbow(passSpeed);
     break; 
    }
    case 5: {
      meet(passSpeed);
      break;
    }
    default: break;  
  } 
}


