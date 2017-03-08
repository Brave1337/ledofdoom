const byte LED_CLOCK_PIN  = 7;
const byte LED_DATA_PIN   = 6;
Adafruit_WS2801 strip = Adafruit_WS2801(32, LED_DATA_PIN, LED_CLOCK_PIN);

const byte OPERATION_SWITCH_MODE  = 0;
const byte OPERATION_SET_COLOR    = 1;

const byte MODE_SOLID   = 0;
const byte MODE_RAINBOW = 1;

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
const uint32_t COLOR_BLACK = Color(0, 0, 0);//0black
const uint32_t COLOR_ORANGE = Color(140, 55, 0);//1orange
const uint32_t COLOR_BLUE = Color(0, 0, 129);//2blue
const uint32_t COLOR_YELLOW = Color(129, 120, 0);//3yellow
const uint32_t COLOR_GREEN = Color(0, 129, 0);//4green
const uint32_t COLOR_RED = Color(127, 0, 0);//5red
const uint32_t COLOR_CYAN = Color(0, 129, 64);//6cyan
const uint32_t COLOR_WHITE = Color(100, 129, 129);//7 White
const uint32_t COLOR_PURPLE = Color(67, 0, 129);//8purp
const uint32_t COLOR_PINK = Color(129, 0, 67);//9 pionk
const uint32_t colorTable[9] = {COLOR_ORANGE, COLOR_BLUE, COLOR_YELLOW, COLOR_GREEN, COLOR_RED, COLOR_CYAN, COLOR_WHITE, COLOR_PURPLE, COLOR_PINK};

byte mode = MODE_SOLID;
uint32_t curColor = 0x008FFF;

void loop() {
  // send data only when you receive data:
  if (Serial.available() >= 2) {
    // read the incoming operation and arg bytes:
    byte op = Serial.read();
    byte arg = Serial.read();

    Serial.print("Recieved ");
    Serial.print(op);
    Serial.print(", ");
    Serial.println(arg);

    if (op == OPERATION_SWITCH_MODE) {
      mode = arg;
    } else if (op == OPERATION_SET_COLOR) {
      Serial.print("Setting color to ");
      Serial.println(colorTable[arg], HEX);
      curColor = colorTable[arg];
    } else {
      Serial.print("Unknown operation: ");
      Serial.println(op);
    }
  if (mode == MODE_SOLID){
    solid(colorTable[arg]) 
  } else if (mode == MODE_RAINBOW){
    rainbowCycle(28);
  }
    Serial.flush();
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


void meet(uint16_t spd){
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
  }
  strip.show();
}
