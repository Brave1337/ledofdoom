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
      // Write -1 slave response code and receive master command code
  byte rx = SPItransfer(255);
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
}


