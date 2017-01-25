void handlePresets() {
#ifdef DEBUGMODE
  //nothing
#else
#endif
  triggerLED();
  //digitalWriteFast(ledPin, HIGH);                       //blink the led
  //onTime = (millis() + 500000);                     //for a LONG TIME
  if (presetState) {                                 //if shift button is toggled / Preset 2 is selected
    Preset = 500;                                   //make an EEPROM adress offset of 500 for reading preset2

  } else {                                          //if Preset1 is selected
    Preset = 0;                                     //make no EEPROM adress offset

  }


  for (int plexelect = 0; plexelect < numberOfPlexers; plexelect++) {     //go through EEPROM reading code once for each multiplexer

    int adressOffset =      plexelect * 40;                                 //offset the EEPROM address by 40 for each multiplexer
    int mapAdressOffset =   plexelect * 32;                                 //turns out the mapping data is stored with a 32 byte interval per multiplexer, so here is a seperate offsett for that
    int arrayOffset =       plexelect * 8;                                  //here is the offset to make sure we are writing to the correct index of the array in RAM

    for (int z = 0; z < 8; z++) {
      DATA1  [z + arrayOffset] = constrain(EEPROM.read(z + Preset       + adressOffset),    0, 127);   //address p1 0-7          p2 40-47      p3 80-87      p4 120-127
      isNote [z + arrayOffset] = constrain(EEPROM.read(z + Preset   + 8 + adressOffset),    0, 1);     //address p1 8-15         p2 48-55      p3 88-95      p4 128-135
      midiCh [z + arrayOffset] = constrain(EEPROM.read(z + Preset  + 16 + adressOffset),    1, 16);    //address p1 16-23        p2 56-63      p3 96-103     p4 136-143
      thresh [z + arrayOffset] = constrain(EEPROM.read(z + Preset  + 24 + adressOffset),    0, 127);   //address p1 24-31        p2 64-71      p3 104-111    p4 144-151
      DATA2  [z + arrayOffset] = constrain(EEPROM.read(z + Preset  + 32 + adressOffset),    0, 127);   //address p1 32-39        p2 72-79      p3 112-119    p4 152-159
      inMin  [z + arrayOffset] = constrain(EEPROM.read(z + Preset + 160 + mapAdressOffset), 0, 127);   //address p1 160-167      p2 192-199    p3 224-231    p4 256-263
      inMax  [z + arrayOffset] = constrain(EEPROM.read(z + Preset + 168 + mapAdressOffset), 0, 127);   //address p1 168-175      p2 200-207    p3 232-239    p4 264-271
      outMin [z + arrayOffset] = constrain(EEPROM.read(z + Preset + 176 + mapAdressOffset), 0, 127);   //address p1 176-183      p2 208-215    p3 240-247    p4 272-279
      outMax [z + arrayOffset] = constrain(EEPROM.read(z + Preset + 184 + mapAdressOffset), 0, 127);   //address p1 184-191      p2 216-223    p3 248-255    p4 280-287
    }
  }



  //LFO//
  cc_lfo = constrain(EEPROM.read(450 + Preset), 0, 127); // address 450
  ch_lfo = constrain(EEPROM.read(451 + Preset), 1, 16);  // address 451

  //Serial.print("handled PRESET - ");
  //Serial.println(Preset);

}


