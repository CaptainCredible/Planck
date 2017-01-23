void handleCalibration(int val) {

  if (calibnotestep > 47) {                                      //if we are at the end of calibration
    EEPROM.write(1000, 123);                                     //write calibrated flag
#ifdef DEBUG
    Serial.print("wrote magic");                                //debug
    Serial.println(EEPROM.read(1000));                           //debug check
#endif

    for (int var = 0; var < 144; var++) {                                         //scan through stored values
      cvOUT(noteArray[calibnotestep]);                          //
      calibnotestep--;                                          //
      if (calibnotestep < 0) {                                  //
        calibnotestep = 47;                                     //
      }                                                         //
      
      digitalWriteFast(ledPin, HIGH);
      delay(50);
      digitalWriteFast(ledPin, LOW);
      delay(50);
      //var++;
    }
    CalibrationMode = false;
    readPitchesEEPROM();
    
  }


  // DURING CALIBRATION //

  if (arrayCursor == 6) {    //check for "big step up" button
    if (val > 500) {
      CALIBRATE = CALIBRATE + 5;
      cvOUT(CALIBRATE);
      delay(200);
    }
  }

  if (arrayCursor == 7) {    //check for "big step down" button
    if (val > 500) {
      CALIBRATE = CALIBRATE - 5;
      cvOUT(CALIBRATE);
      delay(200);
    }
  }


  if (arrayCursor == 8 || arrayCursor == 9 ) {  // check for "store" buttons
    // Serial.println(val);
    if (val > 500) {
      noteArray[calibnotestep] = CALIBRATE;
#ifdef DEBUG
      Serial.print("Note number - ");
      Serial.print(calibnotestep);
      Serial.print("set as");
      Serial.println(CALIBRATE);
#endif
      EEPROM.write(900 + calibnotestep * 2, CALIBRATE >> 8);
      EEPROM.write(901 + calibnotestep * 2, 0xFF & CALIBRATE);
      calibnotestep++;
      for (int i = 0; i < 1024; i++) {
        cvOUT(i);
        delayMicroseconds(10);
      }
      digitalWriteFast(ledPin, HIGH);
      delay(500);
      cvOUT(CALIBRATE);


      digitalWriteFast(ledPin, LOW);

    }
  }
}
