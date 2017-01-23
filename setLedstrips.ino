//DEACTIVATED

/*
void setLedStrips(int note, int velocity, boolean onOff) {
note = note - 120;
  if (note < 3 && note >=0) {                                                                        //ignore notes above 2 so we avoid writing outside the arrays
    if (onOff) {                                                                         //if its a noteOn
      // what note is it ?
      ledVal[note] = MIDI.getData2() * 2;                                                // set the red ledstrip to the velocity value
      ledFades[note] = ledVal[note];                                                     // set the fading value to the velocity
      isLedOn[note] = true;                                                              // flag this led as on
      if (isFading[note]) {                                                              //if the led is in mid fade
        isFading[note] = false;                                                          //flag it as no longer fading
        --aFadeIsOn;                                                                     //subtract one from aFadeIsOn
      }
      analogWrite(RGBpins[note], ledVal[note]);                                          //light it up
    } else {                                                                             //if its a noteOff
      if (isLedOn[note]) {                                                               //if this LED was on
        isLedOn[note] = false;                                                           //mark this led as of, this is only a safeguard against too many noteoffs
        if (masterDecayTime > 1) {                                                       //if fades are active
          isFading[note] = true;                                                         //set this led to fading
          ++aFadeIsOn;                                                                   //add one to aFadeIsOn
          ledTimers[note] = millis() + masterDecayTime;                                  //Set the timer
          polyDecayTime[note] = masterDecayTime;                                         //set the poly decaytime
          //ledFades[note] = ledVal[note];                                                 //set the initial value for the fade             NEEDS CHECKING UP
        } else {                                                                         //if we dont want the decay to run
          ledVal[note] = 0; //turn off the LED
          ledFades[note] = ledVal[note];   //set the initial value for the fade
          analogWrite(RGBpins[note], ledFades[note]);
        }
      }
    }
  }
}
*/
