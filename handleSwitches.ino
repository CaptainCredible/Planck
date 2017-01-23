void handleSwitches() {



  //PRESET SWITCH
  if (digitalReadFast(presetSwitch) == LOW && presetState) {
    presetState = false;
    Preset = 0;
    if (PRESET1) { //ignore this code if we aren't bothered about bloody presets
      //Serial.println("toggled to 1 and found preset");
      handlePresets();
    } else {
      //Serial.println("toggled to 1 and didnt find preset");
    }

    //digitalWriteFast(out1, presetState);

  } else if (digitalRead(presetSwitch) == HIGH && !presetState) {
    presetState = true;
    Preset = 500;                     // i could remove one of these two variables, presetState is boolean and Preset is the eeprom offset for the preset
    if (PRESET2) {                                                  //ignore this code if we aren't bothered about bloody presets
      //Serial.println("toggled to 2 and found preset");
      handlePresets();
    } else {
      //Serial.println("toggled to 2 and didnt find preset");
    }
    //digitalWriteFast(out1, presetState);
  }

//LFO ON SWITCH

if(lfoOnState && !digitalReadFast(lfoOnSwitch)){
  digitalWrite(ledPin, 0);
}
lfoOnState = (digitalReadFast(lfoOnSwitch));


  
  //LFO MAP PIN
  //MAP = !digitalReadFast(rightButton);           //MAP button is on multiplexor2 in box

  //SHIFT BUTTON
  //SHIFT = !digitalReadFast(leftButton);          //SHIFT button is on multiplexor2 in box



  //digitalWrite(out1, !digitalReadFast(shiftButton));
}

