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
    for( r ; r < strip.numPixels(); r++){
    strip.setPixelColor(r - pixLen, Color(0, 0, 129));
    strip.setPixelColor( r , Color(130, 60, 0));//orange
    strip.setPixelColor((strip.numPixels()- pixLen) + r, Color(0, 0, 129));//clear out end
    strip.show();
    delay(spd);
    }
  } 
  else {
    colorWipe(Color(130, 60, 0),0);
  }  
}


void meet(uint8_t spd){
  uint8_t pixLen = 4;
  for (uint8_t r = 0;r < (pixLen/2 + strip.numPixels())/2; r++){
    if ( r !=  ((pixLen / 2 + strip.numPixels())/2) ){
      strip.setPixelColor(r, Color(130, 60, 0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color(130, 60, 0));
      strip.setPixelColor(r - pixLen , Color(0, 0, 129));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color(0, 0, 129));
      strip.show();
      delay(spd);
    }
    else{
      delay(spd);
    }
  }
  for (uint8_t r = strip.numPixels()/2 +3;r < pixLen/2 + strip.numPixels(); r++){
  
      strip.setPixelColor(r, Color(120, 60, 0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color(120, 60, 0));
      strip.setPixelColor(r - pixLen , Color(0, 0, 129));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color(0, 0, 129));
      strip.show();
      delay(spd);
    }
}


void solid(uint32_t stripColor){
 byte i;
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, stripColor);
      strip.show();
  }
}

void flash(byte colorSet){

 uint32_t color1;
 uint32_t color2;

 switch (colorSet / 10){//decides which colors are what
    
  case 0:{
    color1= Color(0, 0, 0);//0black 
    break;
  }
  
  case 1:{
    color1= Color(130, 30, 0);//1orange 
    break;
  }
  
  case 2:{
    color1= Color(0, 0, 128);//2blue 
    break;
  }
  
  case 3:{
    color1= Color(129, 120, 0);//3yellow
    break;
  }

  case 4:{
    color1= Color(0, 129, 0);//4green
    break;
  }

  case 5:{
    color1= Color(127, 0, 0);//5red
    break;
  }

  case 6:{
    color1= Color(0, 129, 64);//6cyan
    break;
  }
  
  case 7:{
    color1= Color(100, 129, 129);//7 White
    break;
  }
  
  case 8:{
    color1= Color(67, 0, 129);//8purp
    break;
  }
  
  case 9:{
    color1= Color(129, 0, 67);//9 pionk
    break;
  }
 }
//color 2
 switch (colorSet % 10){//decides which colors are what
    
  case 0:{
    color2= Color(0, 0, 0);//0black 
    break;
  }
  
  case 1:{
    color2= Color(130, 30, 0);//1orange 
    break;
  }
  
  case 2:{
    color2= Color(0, 0, 128);//2blue
    break;
  }

  case 3:{
    color2= Color(129, 120, 0);//3yellow
    break;
  }

  case 4:{
    color2= Color(0, 129, 0));//4green
    break;
  }

  case 5:{
    color2= Color(127, 0, 0);//5red
    break;
  }

  case 6:{
    color2= Color(0, 129, 64);//6cyan
    break;
  }
  
  case 7:{
    color2= Color(100, 129, 129);//7 White
    break;
  }
  
  case 8:{
    color2= Color(67, 0, 129);//8purp
    break;
  }
  
  case 9:{
    color2= Color(129, 0, 67);//9 pionk
    break;
  }
 }
 
  //actual led code
  
  solid(color1);
  delay(20);
  solid(color2);
  delay(20);
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
  
  //mode set
  mode = 0;
  mode = SPItransfer(255); 
}
  
void loop() {
  byte flashColors;//used in mode 2
  
  byte pixelLength;//determines pixel lenth
  byte passSpeed;//the delay value to be passed in 
  byte passMode;//determines what to use based on the passed in mode

 mode = SPItransfer(255);
  if (mode <= 99){
    passMode = 1; //speed strip code
    passSpeed = ((100 - mode)* 2);
  }
  if ((100 <= mode) && (mode < 200)){
    passMode = 2; //2 color flash
    flashColors = 100 - mode;
  }  
  if ((200 <= mode) && (mode < 220)){
    passMode = 6; //orange and blue color wipe
    passSpeed= 224 - mode;
  }
  if ((220 <= mode)&& (mode < 230)){
    passMode = 3;//rainbow cycle
    passSpeed= 234 - mode;
  }
  if ((230 <= mode)&&(mode < 240)){
    passMode = 4;//rainbow
    passSpeed= 244 - mode;
  }
  if ((240 <= mode)&&(mode < 256)){
    passMode = 5;//meet
    passSpeed= (263 - mode)* 2;
  }

  pixelLength = 1 + ((200 - passSpeed) / 10);
  
  switch(passMode){//switch structure for the LED mode
     
     case 1: {//do speed chaser mode
      cycle(passSpeed, pixelLength);
      break;
    }  
    
    case 2: {//do 2 color flash mode
      flash(flashColors);
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
    
    case 6: {
      colorWipe(Color(0 , 9, 128), passSpeed);
      colorWipe(Color(130, 30 , 0), passSpeed);
      break;
    }
    case 9: {
      //solid(Color(0, 0, 0));//0black
    // solid(Color(130, 30, 0));//1orange
     //solid(Color(0, 0, 128));//2blue
    // solid(Color(129, 120, 0));//3yellow
    // solid(Color(0, 129, 0));//4green
    // solid(Color(127, 0, 0));//5red
    // solid(Color(0, 129, 64));//6cyan
     //solid(Color(100, 129, 129));//7 White
    //solid(Color(67, 0, 129));//8purp
    //solid(Color(129, 0, 67));//9 pionk
    }
    default: break;  
  } 
}


