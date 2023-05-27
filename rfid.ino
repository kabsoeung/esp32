#include <EEPROM.h>     // We are going to read and write PICC's UIDs from/to EEPROM
#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>  // Library for Mifare RC522 Devices
#define COMMON_ANODE
#ifdef COMMON_ANODE
#define LED_ON HIGH
#define LED_OFF LOW
#else
#endif
#define redLed 6    
#define greenLed 5
#define whiteLed 4
const int starter=15;
#define Relay 8     
#define EraseButton 7     
#define openButton 2     
#define Buzzer 3    

boolean match = false;          // initialize card match to false
boolean programMode = false;  // initialize programming mode to false
boolean replaceMaster = false;

uint8_t successRead;    // Variable integer to keep if we have Successful Read from Reader

byte storedCard[4];   // Stores an ID read from EEPROM
byte readCard[4];   // Stores scanned ID read from RFID Module
byte masterCard[4];   // Stores master card's ID read from EEPROM

// Create MFRC522 instance.
#define SS_PIN 10
#define RST_PIN 9
//MFRC522 Pins
//SS 10
//Reset 9
//sck 13
//miso 12 
//mosi 11
//vcc 3.3v
//GND
//RQ not use

MFRC522 mfrc522(SS_PIN, RST_PIN); 
///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Arduino Pin Configuration
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(whiteLed, OUTPUT);
  pinMode(EraseButton, INPUT_PULLUP);   // Enable pin's pull up resistor
  pinMode(Relay, OUTPUT);
  pinMode(starter, OUTPUT);
  pinMode(Buzzer, OUTPUT); 
  //Be careful how Relay circuit behave on while resetting or power-cycling your Arduino
  digitalWrite(Relay, HIGH);    // Make sure door is locked
  digitalWrite(starter, HIGH);
  digitalWrite(redLed, LED_OFF);  
  digitalWrite(greenLed, LED_OFF);  
  digitalWrite(whiteLed, LED_OFF); 

  //Protocol Configuration
  Serial.begin(115200);  // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware

  //Serial.println(F("DEVICE RESET"));   // For debugging purposes
  ShowReaderDetails();  // Show details of PCD - MFRC522 Card Reader details

  // If the Button (EraseButton) Pressed while setup run (powered on) it erases EEPROM
  if (digitalRead(EraseButton) == LOW) {  // When button pressed pin should get low, button connected to ground
      digitalWrite(redLed, LED_ON);  
      digitalWrite(greenLed, LED_ON);  
      digitalWrite(whiteLed, LED_ON);  // all Led stays on to inform user we are going to erase all records
    //Serial.println(F("Erase Button Pressed"));
    tone(Buzzer, 2400, 400); 
    delay(200);
    noTone(Buzzer);
    //Serial.println(F("You have 10 seconds to Cancel"));
    //Serial.println(F("This will be remove all records and cannot be undone"));
    delay(10000);                        // Give user 10s to cancel operation
    if (digitalRead(EraseButton) == LOW) {    // If button still pressed, erase EEPROM
      //Serial.println(F("Starting Erasing EEPROM"));
      for (int x = 0; x < EEPROM.length(); x = x + 1) {    //Loop end of EEPROM address "EEPROM.length()"
        if (EEPROM.read(x) == 0) {              //If EEPROM address 0
          // do nothing, already clear, go to the next address in order to save time and reduce writes to EEPROM
        }
        else {
          EEPROM.write(x, 0);       // if not write 0 to clear, it takes 3.3mS
        }
        //Serial.println(x);
      }
      //Serial.println(F("EEPROM Successfully Wiped"));
      digitalWrite(greenLed, LED_OFF);
      tone(Buzzer, 2400, 400); 
      delay(200);
      noTone(Buzzer);
      digitalWrite(redLed, LED_OFF);  // visualize a successful erase
      delay(200);
      digitalWrite(redLed, LED_ON);
      delay(200);
      digitalWrite(redLed, LED_OFF);
      delay(200);
      digitalWrite(redLed, LED_ON);
      delay(200);
      digitalWrite(redLed, LED_OFF);
      
    }
    else {
      //Serial.println(F("Erasing Cancelled")); // Show some feedback that the erase button did not pressed for 10 seconds
    
    }
  }
  // Check if master card defined, if not let user choose a master card
  // This also useful to just redefine the Master Card
  // You can keep other EEPROM records just write other than 168 to EEPROM address 1
  // EEPROM address 1 should hold value '168'
  if (EEPROM.read(1) != 168) { //"168" Actually any number is fine just for faster checking that mastercard is saved
    //Serial.println(F("No Master Card Defined"));
    //Serial.println(F("Scan A PICC to Define as Master Card"));
    do {
      successRead = getID();            // sets successRead to 1 when we get read from reader otherwise 0
      // Visualize Master Card need to be defined 
      digitalWrite(whiteLed, LED_OFF);   
      delay(200);
      digitalWrite(whiteLed, LED_ON); 
      delay(300);
      digitalWrite(whiteLed, LED_OFF);   
      delay(200);
      digitalWrite(whiteLed, LED_ON); 
      delay(300);
      
    
    }
    while (!successRead);                  // Program will not go further while you not get a successful read
    for ( uint8_t j = 0; j < 4; j++ ) {        // Loop 4 times
      EEPROM.write( 2 + j, readCard[j]);  // Write scanned PICC's UID to EEPROM, start from address 3 
    }
    
    EEPROM.write(1, 168);                  // Write to EEPROM we defined Master Card.
    //Serial.println(F("Master Card Defined"));
   
  }
  //Serial.println(F("-------------------"));
  //Serial.println(F("Master Card's UID"));
  for ( uint8_t i = 0; i < 4; i++ ) {          // Read Master Card's UID from EEPROM
    masterCard[i] = EEPROM.read(2 + i);    // Write it to masterCard[i]
    //Serial.print(masterCard[i],HEX);
    
  }
  //Serial.println("");
  //Serial.println(F("-------------------"));
  //Serial.println(F("Everything Ready"));
  //Serial.println(F("Waiting PICCs to be scanned"));
  cycleLeds();    // Everything ready lets give user some feedback by cycling leds
  
}


///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {

    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 0
    
    // When device is in use if erase button pressed for 10 seconds initialize Master Card wiping 
      
    if (digitalRead(EraseButton) == LOW) { // Check if Erase button is pressed
      // red and green Led stays on to inform user we are going to erase Master Card
      digitalWrite(redLed, LED_ON);  
      digitalWrite(greenLed, LED_ON);  
      digitalWrite(whiteLed, LED_OFF); 
      
      // Give some feedback 
      //Serial.println(F("Wipe Button Pressed"));
      tone(Buzzer, 2400, 400); 
      delay(200);
      noTone(Buzzer);
      //Serial.println(F("Master Card will be Erased! in 10 seconds"));
      delay(10000);  // Wait 10 seconds to see user still wants to erase
      if (digitalRead(EraseButton) == LOW) {
        EEPROM.write(1, 0);                  
        //Serial.println(F("Restart device to re-program Master Card"));
        tone(Buzzer, 2400, 400); 
        delay(200);
        noTone(Buzzer);
        while (1);
      }
    }
        if (digitalRead(openButton) == HIGH) { // Check if button is pressed
        tone(Buzzer, 2400, 400); 
        delay(200);
        noTone(Buzzer);
        granted(1000);
        }
    if (programMode) {
      cycleLeds();              // Program Mode cycles through Red Green Blue waiting to read a new card
    }
    else {
      normalModeOn();     // Normal mode, white Power LED is on, all others are off
    }
  
  }
  while (!successRead);   //the program will not go further while you are not getting a successful read
  if (programMode) {
    if ( isMaster(readCard) ) { //When in program mode check First If master card scanned again to exit program mode
      //Serial.println(F("Master Card Scanned"));
      //Serial.println(F("Exiting Program Mode"));
      //Serial.println(F("-----------------------------"));
      programMode = false;
      return;
    }
    else {
      if ( findID(readCard) ) { // If scanned card is known delete it
        //Serial.println(F("I know this PICC, removing..."));
        deleteID(readCard);
        //Serial.println("-----------------------------");
        //Serial.println(F("Scan a PICC to ADD or REMOVE to EEPROM"));
      }
      else {                    // If scanned card is not known add it
        //Serial.println(F("I do not know this PICC, adding..."));
        writeID(readCard);
        //Serial.println(F("-----------------------------"));
        //Serial.println(F("Scan a PICC to ADD or REMOVE to EEPROM"));
      }
    }
  }
  else {
    if ( isMaster(readCard)) {    // If scanned card's ID matches Master Card's ID - enter program mode
      programMode = true;
      //Serial.println(F("-----------------------------"));
      //Serial.println(F("Hello Master - Entered Program Mode"));
      uint8_t count = EEPROM.read(0);   // Read the first Byte of EEPROM that
      //Serial.print(F("I have "));     // stores the number of ID's in EEPROM
      //Serial.print(count);
      //Serial.print(F(" record(s) on EEPROM"));
      //Serial.println("");
      //Serial.println(F("Scan a PICC to ADD or REMOVE From EEPROM"));
      //Serial.println(F("Scan Master Card again to Exit Program Mode"));
      //Serial.println(F("-----------------------------"));
    }
    else {
         
      if ( findID(readCard) ) { // If not, see if the card is in the EEPROM
        //Serial.println(F("Welcome, You shall pass"));
        granted(5000);         // Open the door lock 
      }
      else {      // If not, show that the ID was not valid
        //Serial.println(F("You shall not pass"));
        denied();
      
      } 
        
    }
  }
}




