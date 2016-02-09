/* 
 * Communication Protocol:
 * Slave acknowledges SS (Slave Select) by sending -1 slave response code and receiving 
 * master command code. On subsequent SS slave sends master command code or -1 slave 
 * response code indicating an unrecognized command has been received. Additional SS's 
 * may be needed to transfer additional data to/from Slave depending on the master command

 * SPI pin numbers:
 * SCK   52  // Serial Clock.
 * MISO  50  // Master In Slave Out.
 * MOSI  51  // Master Out Slave In.
 * SS    53  // Slave Select
 */
// Include SPI (Serial Peripheral Interface) library. Does not support SPI Slave.
#include <SPI.h>
boolean SSlast = LOW;         // SS last flag.
const byte led = 12;           // Slave LED digital I/O pin.
boolean ledState = HIGH;      // LED state flag.
const byte cmdLEDState = 2;   // 

//Initialize SPI slave.
void SlaveInit(void) {
  // Initialize SPI pins.
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
//  pinMode(MISO, INPUT);
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
// The setup() function runs after reset.
void setup() {
  // Initialize serial for troubleshooting.
  Serial.begin(9600);
  // Initialize slave LED pin.
  pinMode(led, OUTPUT);
  digitalWrite(led, ledState);
  // Initialize SPI Slave.
  SlaveInit();
  Serial.println("Slave Initialized");
}
// The loop function runs continuously after setup().
void loop() {
  // Slave Enabled?
  if (!digitalRead(SS)) {
    // Yes, first time?
    if (SSlast != LOW) {
      // Yes, take MISO pin.
     //pinMode(MISO, OUTPUT);
      Serial.println("***Slave Enabled.");
      // Write -1 slave response code and receive master command code
      byte rx = SPItransfer(255);
      Serial.println("Initial -1 slave response code sent");
      Serial.println("rx:" + String(rx) + ".");

      // cmdLEDState?
      if (rx == cmdLEDState) {
        // Acknowledge cmdLEDState.
        byte rx = SPItransfer(cmdLEDState);
        Serial.println("cmdLEDState Acknowledged.");
        Serial.println("rx:" + String(rx) + ".");
        rx = SPItransfer(ledState);
        Serial.println("ledState:" + String(ledState) + " Sent.");
        Serial.println("rx:" + String(rx) + ".");  
        ledState = !ledState;
        digitalWrite(led, ledState);   
      }
      else {
        // Unrecognized command.
        byte rx = SPItransfer(255);
        Serial.println("Unrecognized Command -1 slave response code sent.");
        Serial.println("rx:" + String(rx) + ".");
      }
      // Update SSlast.
   // SSlast= LOW;
    }
  }
  else {
    // No, first time?
    if (SSlast != HIGH) {
      // Yes, release MISO pin.
      pinMode(MISO, INPUT);
      Serial.println("Slave Disabled.");
      // Update SSlast.
      SSlast = HIGH;
    }
  }
}

