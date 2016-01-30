#include <Wire.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.
//LED stuff
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
//pin def
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels


uint8_t fakeSpeed;
uint8_t mode;


int distance = 0;    // Distance measured
float measuredDis = 0;
float filterVal = 0.5;
int prevDis = 0;

Adafruit_WS2801 strip = Adafruit_WS2801(32, dataPin, clockPin);

void setup()
{
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  
 // Serial output
  Serial.begin(4800);
  Serial.println("<BEGIN LIDAR>");
  
  // LIDAR control
  Wire.begin(); // join i2c bus

 strip.begin();
    
  // Update LED contents, to start they are all 'off'
  strip.show();
  
  //mode set
  mode = 7;

  fakeSpeed = 129;
  
 colorWipe(Color(0,10,123),0);

  strip.show();
}

// Get a measurement from the LIDAR Lite
int lidarGetRange()
{
  int val = -1;
  
  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)  
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)  
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit
  
  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if(2 <= Wire.available()) // if two bytes were received
  {
    val = Wire.read(); // receive high byte (overwrites previous reading)
    val = val << 8; // shift high byte to be high 8 bits
    val |= Wire.read(); // receive low byte as lower 8 bits
  }
  
  return val;
}

void serialPrintRange(int distance)
{
    Serial.print("\t\tDistance (cm): ");
    int inch = distance / 2.54;
    Serial.println(inch);
}


//LED STUFF






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
    strip.setPixelColor(r - pixLen, Color(0,10,123));
    strip.setPixelColor( r , Color(120,30,0));//orange
    strip.show();
    delay(spd);
    }
  } 
   else {
      colorWipe(Color(120,30,0),0);
   }
    
}
void meet(uint8_t spd){

uint8_t pixLen = 4;
  for (uint8_t r = 0;r < (pixLen/2 + strip.numPixels())/2; r++){
    if ( r !=  ((pixLen / 2 + strip.numPixels())/2) ){
      strip.setPixelColor(r, Color(120,30,0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color(120,30,0));
      strip.setPixelColor(r - pixLen , Color(0,10,123));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color(0,10,123));
      strip.show();
      delay(spd);
    }
    else{
      delay(spd);
    }
  }
  for (uint8_t r = strip.numPixels()/2 +3;r < pixLen/2 + strip.numPixels(); r++){
  
      strip.setPixelColor(r, Color(120,30,0));  
      strip.setPixelColor(strip.numPixels()-1 - r, Color(120,30,0));
      strip.setPixelColor(r - pixLen , Color(0,10,123));
      strip.setPixelColor(strip.numPixels()-1 - r + pixLen, Color(0,10,123));
      strip.show();
      delay(spd);
    }
}

void speedStrip(uint32_t lidarInput){
  
  uint16_t lastLight = (lidarInput - lidarInput%100)/50;
  int i;
  int j = strip.numPixels();
  if(lastLight > 0){
  
  for(i = 0; i < j; i++){
    
    if(i <= lastLight){
      
      strip.setPixelColor(i, Color(120,30,0));
      
      strip.setPixelColor(j - (i + 1), Color(120,30,0));
      
    }else if(i >= j - (lastLight + 1)){}else{
      
      strip.setPixelColor(i, Color(0,10,123));
      
      }
    }
  }else{
      for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(0,10,123));
  }
  }
  
  strip.show();
}

void loop() {
uint8_t passSpeed = (180 - fakeSpeed)/2;
uint8_t pixelLength = fakeSpeed / 10; 
uint32_t regDis;

    measuredDis = lidarGetRange();
    
    distance = (measuredDis * filterVal) + (measuredDis * (1 - filterVal));
    
    if(distance >= 0){
      if(distance > (prevDis + 500)){
        serialPrintRange(prevDis + 250);
        prevDis = distance - 500;
        regDis = distance - 500;
      }else{
        serialPrintRange(distance);
        prevDis = distance;
        regDis = distance;
      }}else{
        regDis = prevDis;
        serialPrintRange(prevDis);
      }
  
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
      colorWipe(Color(0,10,123),0);
      //colorWipe(Color(120,30,0),10);
      break;
    }
    case 4: {//4 strip
      cycle(passSpeed, pixelLength);
      break;    
    }
    case 5: {
     meet(0);
     break;
    }
    case 6:{
      strip.setPixelColor(strip.numPixels()-1, Color(100,100,101));
      strip.show();
      break;
    }
    case 7:{
      speedStrip(regDis);
      break;
      }
    default: break;  
  }
}
