void handleToggleSwitches() {
  /*
    // SHIFT TOGGLE
    if ((digitalReadFast(ShiftSwitchPin) == HIGH)) {                                //if switch is pressed
      if ((lastToggleShift == false) && millis() - shiftBounce > DebounceT) {   //check if it previously ws off and if it was more than 50ms since last toggle
        presetState = !presetState;                                               //flip the state of shift flag
        //digitalWriteFast(ShiftLedPin, presetState);
        shiftBounce = millis();                                                 //reset the debounce timer
        lastToggleShift = true;                                                 //remember current state of the button
      }
    } else if (lastToggleShift && millis() - lfoBounce > DebounceT) {           //if button is off and previously was on
      shiftBounce = millis();                                                   //reset debounce timer
      lastToggleShift = false;
    }
  */

  // LFO TOGGLE
  /*
  if (lfoOnState) {
    if ((digitalReadFast(leftButton) == HIGH)) {                                  //if switch is pressed
      if ((lastToggleLfo == false) && millis() - lfoBounce > DebounceT) {       //check if it previously ws off and if it was more than 50ms since last toggle
        lfoOnState = !lfoOnState;                                               //flip the state of lfo flag
        digitalWriteFast(lfoLedPin, lfoOnState);
        lfoBounce = millis();                                                   //reset the debounce timer
        lastToggleLfo = true;                                                   //remember current state of the button
        //int test = 5*lfoOnState;                                              // a way to set mappings to zero?

        if (lfoOnState) {
          for (int i = 0; i < 13; i++) {
            outMin[i] = 0;
            outMax[i] = 0;
          }
        } else {
          if (PRESETS) {
            handlePresets();                                                      //when the LFO is turned off, read preset again to reset everything
          }
        }
      }
    } else if (lastToggleLfo && millis() - lfoBounce > DebounceT) {             //if button is off and previously was on
      lastToggleLfo = false;                                                    //remember lfo button state
      lfoBounce = millis();                                                     //reset debounce
      digitalWriteFast(ledPin, LOW);
      onTime = millis(); ledPinState = true;
    }
  }
*/
}
 
