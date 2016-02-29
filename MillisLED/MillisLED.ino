#include "Adafruit_WS2801.h" 
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
// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

uint8_t iterator = 0; //used in all of the loops
uint8_t lastMode = 0; // 0 is nothing
uint8_t currentMode = 0; // I am lazy

uint8_t mode;//what would be passed in by

//globals for update check
unsigned long currentMillis = millis(); //time since start-up, lasts 50 days before overflow
unsigned long previousMillis = 0;  // will store last time LEDs were updated


// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(32, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//Adafruit_WS2801 strip = Adafruit_WS2801(25);

//HELPER FUNCTIONS
//#######################################################
//#######################################################

//general

bool updater(uint16_t interval, uint16_t restriction){
  currentMillis = millis();
  bool checker = currentMillis - previousMillis >= interval;
  
  if ((lastMode != currentMode) || (iterator == restriction) ){
    iterator = 0; 
    lastMode = currentMode;
  }
  
  if (checker == true) {
    // save the last time you blinked the LEDs
    previousMillis = currentMillis;
  }
  return checker;
}

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

void solid(uint32_t stripColor){
 byte i;
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, stripColor);
  }
  strip.show();
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  currentMode = 200; 
  updater(wait, strip.numPixels());
  strip.setPixelColor(iterator, c);
  strip.show();
  iterator += 1;
}


void cycle(uint8_t spd, uint8_t pixLen){ // mode 1 DONE 
  currentMode =  1;
  bool doStuff = updater(spd, strip.numPixels());
  if(doStuff==true){
    if (pixLen < 18){ 
      strip.setPixelColor(iterator - pixLen, Color(0, 0, 129));
      strip.setPixelColor(iterator , Color(130, 60, 0));//orange
      strip.setPixelColor((strip.numPixels()- pixLen) + iterator, Color(0, 0, 129));//clear out end
      strip.show();
      } 
    else {
      solid(Color(130, 60, 0));
    }  
  iterator += 1 ;
  }
}

 void rainbow(uint8_t wait) { // mode 4 KYLE PLS CLEAN ME I AM SO OUT OF ORDER
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
void rainbowCycle(uint8_t wait) {// mode 3
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


void meet(uint16_t spd){// mode 5
  uint8_t pixLen = 4;
  byte halfway = ((pixLen / 2 + strip.numPixels())/2);
  currentMode = 5;
  if(updater(spd, pixLen/2 + strip.numPixels() )){
   if (iterator <  halfway -1 ){
    strip.setPixelColor(iterator, Color(130, 60, 0));  
    strip.setPixelColor(strip.numPixels()-1 - iterator, Color(130, 60, 0));
    strip.setPixelColor(iterator - pixLen , Color(0, 0, 129));
    strip.setPixelColor(strip.numPixels()-1 - iterator + pixLen, Color(0, 0, 129));
    strip.show();
    iterator += 1;
    }
    
    if (iterator == halfway -1){
    delay(spd);   
    iterator += (1 + pixLen/2);
    }
    if ((iterator > (pixLen/2) + (strip.numPixels()/2)) &&  (iterator < pixLen/2 + strip.numPixels())){
  Serial.println(iterator);
      strip.setPixelColor(iterator, Color(130, 60, 0));  
      strip.setPixelColor(strip.numPixels()-1 - iterator, Color(130, 60, 0));
      strip.setPixelColor(iterator - pixLen , Color(0, 0, 129));
      strip.setPixelColor(strip.numPixels()-1 - iterator + pixLen, Color(0, 0, 129));
      strip.show();
      iterator += 1;
    }  
  }

}



void flash(byte colorSet){//mode 2
currentMode = 2;
 uint32_t color1 = 5432;
 uint32_t color2;
 byte colorSwitch1= colorSet/ 10;
 byte colorSwitch2= colorSet % 10;
 
 switch (colorSwitch1){//decides which colors are what
    
  case 0:{
    color1= Color(0, 0, 0);//0black 
    break;
  }
  
  case 1:{
    color1= Color(130, 30, 0);//1orange 
    break;
  }
  
  case 2:{
    color1= Color(0, 0, 129);//2blue 
    break;
  }
  
  case 3:{
    color1=Color(129, 120, 0);//3yellow
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
  default:break;
 }

 switch (colorSwitch2){//decides which colors are what
    
  case 0:{
    color2= Color(0, 0, 0);//0black 
    break;
  }
  
  case 1:{
    color2= Color(130, 30, 0);//1orange 
    break;
  }
  
  case 2:{
    color2= Color(0, 0, 129);//2blue
    break;
  }

  case 3:{
    color2= Color(129, 120, 0);//3yellow
    break;
  }

  case 4:{
    color2= Color(0, 129, 0);//4green
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
  default:break;
 }
 
  //actual led code
  if (updater(500, 2)){
    switch(iterator){
      case 0:{
        solid(color1);
        iterator += 1;
        break;
      }
      case 1:{
        solid(color2);
        iterator += 1;
        break;
      }
      default:break;
    }
  } 
}

//END HELPER FUNCTIONS
//###########################################################
//###########################################################
//###########################################################



//Setup and Loop

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  Serial.begin(9600);
  strip.begin();
  strip.show();
  
  //mode set
  mode = 4;


  solid(Color(0, 0, 129));

}


void loop() {
  byte flashColors;//used in mode 2
  
  byte pixelLength;//determines pixel lenth
  uint16_t passSpeed;//the delay value to be passed in 
  byte passMode;//determines what to use based on the passed in mode
 
 mode = 238;
  if (mode <= 99){
    passMode = 1; //speed strip code
    passSpeed = ((100 - mode)* 2);
  }
  if ((100 <= mode) && (mode < 200)){
    passMode = 2; //2 color flash
    flashColors = mode - 100;
  }  
  if ((200 <= mode) && (mode < 220)){
    passMode = 6; //orange and blue color wipe
    passSpeed= 224 - mode;
  }
  if ((220 <= mode)&& (mode < 230)){
    passMode = 3;//rainbow cycle
    passSpeed= 235 - mode;
  }
  if ((230 <= mode)&&(mode < 240)){
    passMode = 4;//rainbow
    passSpeed= 244 - mode;
  }
  if ((240 <= mode)&&(mode < 256)){
    passMode = 5;//meet
    passSpeed= (263 - mode) * 2;
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
    
    case 3: {//distributed rainbow
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
      colorWipe(Color(0 , 0, 129), passSpeed);
      colorWipe(Color(130, 30 , 0), passSpeed);
      break;
    }
    case 9: {
    //solid(Color(0, 0, 0));//0black
    // solid(Color(130, 30, 0));//1orange
    //solid(Color(0, 0, 129));//2blue
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


