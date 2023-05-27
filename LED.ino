///////////////////////////////////////// Cycle Leds (Program Mode) ///////////////////////////////////
void cycleLeds() {

  digitalWrite(whiteLed, LED_OFF);   
  delay(200);
  digitalWrite(whiteLed, LED_ON);  
  delay(200);
  
}
//////////////////////////////////////// Normal Mode Led  ///////////////////////////////////
void normalModeOn () {
  digitalWrite(whiteLed, LED_ON);  
  digitalWrite(redLed, LED_OFF);  
  digitalWrite(greenLed, LED_OFF);  
  
}
///////////////////////////////////////// Access Denied  ///////////////////////////////////
void denied() {
  tone(Buzzer, 2400, 400); 
  digitalWrite(greenLed, LED_OFF);  
  digitalWrite(whiteLed, LED_OFF);   
  digitalWrite(redLed, LED_ON);
  delay(200);
  noTone(Buzzer);
  delay(100);
  tone(Buzzer, 2400, 400); 
  delay(200);
  noTone(Buzzer);
  delay(1000);
}
/////////////////////////////////////////  Access Granted    ///////////////////////////////////
void granted ( uint16_t setDelay) {
  digitalWrite(whiteLed, LED_OFF);
  digitalWrite(greenLed, LED_ON);     
 
  
  if(digitalRead(Relay)==HIGH) 
  { 
  
  digitalWrite(Relay,LOW); 
  digitalWrite(starter,LOW); 
  delay(5000); 
  digitalWrite(starter,HIGH); 
  digitalWrite(Relay,HIGH);
  }
  else
  { 
    digitalWrite(Relay,LOW); 
  }
  
  digitalWrite(greenLed, LED_OFF);
}
///////////////////////////////////////// Write Success to EEPROM   ///////////////////////////////////
// Flashes the green LED 3 times to indicate a successful write to EEPROM
void successWrite() {
  digitalWrite(whiteLed, LED_OFF);   
  digitalWrite(redLed, LED_OFF); 
  digitalWrite(greenLed, LED_OFF);  
  delay(200);
  digitalWrite(greenLed, LED_ON);   
  delay(200);
  digitalWrite(greenLed, LED_OFF);  
  delay(200);
  tone(Buzzer, 2400, 200); 
  delay(200);
  noTone(Buzzer);
  digitalWrite(greenLed, LED_ON);   
  delay(200);
  digitalWrite(greenLed, LED_OFF);  
  tone(Buzzer, 2400, 200); 
  delay(200);
  noTone(Buzzer);
  delay(100);
  digitalWrite(greenLed, LED_ON);   
  delay(100);
}
///////////////////////////////////////// Success Remove UID From EEPROM  ///////////////////////////////////
// Flashes the white LED 3 times to indicate a success delete to EEPROM
void successDelete() {
 digitalWrite(whiteLed, LED_OFF);   
  digitalWrite(redLed, LED_OFF);  
  digitalWrite(greenLed, LED_OFF);  
  delay(200);
  digitalWrite(greenLed, LED_ON);   
  delay(200);
  digitalWrite(greenLed, LED_OFF);  
  delay(200);
  tone(Buzzer, 2400, 200); 
  delay(200);
  noTone(Buzzer);
  digitalWrite(greenLed, LED_ON);  
  delay(200);
  digitalWrite(greenLed, LED_OFF); 
  tone(Buzzer, 2400, 200); 
  delay(200);
  noTone(Buzzer);
  delay(100);
  tone(Buzzer, 2400, 200); 
  delay(200);
  noTone(Buzzer);
  delay(100);
  digitalWrite(greenLed, LED_ON);  
  delay(100);
  
}

///////////////////////////////////////// Write Failed to EEPROM   ///////////////////////////////////
// Flashes the red LED 3 times to indicate a failed write to EEPROM
void failedWrite() {
  tone(Buzzer, 2400, 200); 
  delay(200);
  digitalWrite(whiteLed, LED_OFF);   
  digitalWrite(redLed, LED_OFF);  
  digitalWrite(greenLed, LED_OFF);  
  delay(200);
  digitalWrite(redLed, LED_ON);  
  delay(200);
  digitalWrite(redLed, LED_OFF);  
  delay(200);
  digitalWrite(redLed, LED_ON);   
  delay(200);
  digitalWrite(redLed, LED_OFF);  
  delay(200);
  digitalWrite(redLed, LED_ON);   
  delay(200);
  noTone(Buzzer);
  delay(100);
}
void ShowReaderDetails() {
  // Get the MFRC522 software version
  // byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  // //Serial.print(F("MFRC522 Software Version: 0x"));
  // //Serial.print(v, HEX);
  // if (v == 0x91)
  //   //Serial.print(F(" = v1.0"));
  // else if (v == 0x92)
  //   //Serial.print(F(" = v2.0"));
  // else
    //Serial.print(F(" (unknown),probably a chinese clone?"));
  //Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  // if ((v == 0x00) || (v == 0xFF)) {
  //   //Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  //   //Serial.println(F("SYSTEM HALTED: Check connections."));
  //   // Visualize system is halted
    digitalWrite(greenLed, LED_OFF);  
    digitalWrite(whiteLed, LED_OFF);   
    digitalWrite(redLed, LED_ON);   
  //   while (true); // do not go further
  // }
}