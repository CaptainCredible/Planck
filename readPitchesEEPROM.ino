void readPitchesEEPROM() {
  for (int i = 0; i < 48; i++) {
noteArray[i] = (EEPROM.read(900+(i*2))<<8)+EEPROM.read(901+(i*2));
  
  }
}


