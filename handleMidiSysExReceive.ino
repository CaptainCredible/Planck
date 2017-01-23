void handleMidiSysexReceive() {
  printSexArray();
  if ((sexArray[11] == 100) && (sexArray[12] == 110)) {                                       // does it have Toolmans secret sysex identifier?
    sysExFlag = true;                                                                        // set sysExFlag true so that program focuses on receiving and writing
    digitalWriteFast(ledPin, HIGH);                                                          // celebrate by turning a the LED on
    onTime = millis(); ledPinState = true;                                                   // remember when we turned the LED on so we know when its time to turn it off
    writeAdress = (((sexArray[1]) * 128) + sexArray[2]);                                     // use a kind of number system with base 128 to get eeprom address
    if (writeAdress  == 384) {                                                               // This means array[1] is 3 and array[2] is 0 the magic code to reset
      sysExFlag = false;
      if (digitalReadFast(presetSwitch)) {
        EEPROM.write(401, 123);                                                              //123 in 401 means PRESET2 exists
        PRESET2 = true;
        //Serial.println("PRESET 2 marked as written");        
      } else {
        EEPROM.write(400, 123);                                                               //flag that the preset1 is written
        PRESET1 = true;
        //Serial.println("PRESET 1 marked as written");
      }
      handlePresets();                                                                      // READ THE PRESETS AND STICK THEM IN RAM BEFORE WE EXIT MIDI IN FUNCTION
    }
    else if (writeAdress == 450) {                                                          // i reckon this is the write adress that comes up if we are setting the LFO
      EEPROM.write(450, sexArray[3]);                                           // write lfo CC to EEPROM
      EEPROM.write(451, sexArray[4]);                                           // write lfo channel to EEPROM
    } else {                                                                                // if its not the RESET or the LFO then its data to stick in our EEPROM
//Serial.print("EEPROM - - - - - - ");
      for (int b = 0; b < 8; b++) {                                                         // loop 8 times
        EEPROM.write(writeAdress + b + Preset, sexArray[b + 3]);                // put the data from the sysex message into EEPROM ignoring bytes 0,1 and 2
printEEPROM(writeAdress + b + Preset);
//Serial.print(" preset offset is ");
//Serial.print(Preset);
//Serial.print(" ");
      }
    }
  }
}



void printSexArray() {
  //Serial.println();
  //Serial.print("SexArray = ");
  for (int i = 0; i < 14; i++) {
    //Serial.print(sexArray[i]);
    //Serial.print(" ");
  }
  //Serial.println();
//Serial.print("EEPROM =   ");

}

void printEEPROM(int select) {
  //Serial.print(EEPROM.read(select));
  //Serial.print(" ");
}



