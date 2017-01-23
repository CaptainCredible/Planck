//ADD CODE TO IGNORE MIDI CONTROLS IF LFO IS ON

void Note(int plexer, int rawVal, int current) {
  if (current > 3) {                                                                //if this is not an LFO control
    int  midiVal = rawVal >> 3;
    int rawDiff = rawVal - oldRaw[current];                                             //what is the delta to the latest "anchored" oldVal?
    if (rawDiff <= -8 || rawDiff >= 8) {                                                //if it has moved an entire 128th
      oldRaw[current] = rawVal;                                                     //cast out a new anchor
      if (midiVal > thresh[current]) {                                                  // check if the button is pressed
        if (noteIsOn[current] == 0) {                                                   //check if button was off
          if (!PRESET2 && presetState) {
            midiCHANNEL = midiCh[current] + 1;
          } else {
            midiCHANNEL = midiCh[current];
          }
          if (SHIFT && (!lfoOnState || current > 3)) {                                                      //is shift pushed in and LFO is off or or its not an lfo control?
            midiCHANNEL = (midiCHANNEL + 2) % 16;                                              //offset channel by 2 and roll around if it goes beyond 16
          }
          sendUNon(DATA1[current], DATA2[current], midiCHANNEL);
          sendHNon(DATA1[current], DATA2[current], midiCHANNEL);
          //write down time
          noteIsOn[current] = 1;                                                        //flag this note as on
        }
        //this is what it does if the button is on and it was on last iteration
        //nothing
      }
      if (midiVal <= thresh[current]) {                                                 //check if button is released
        if (noteIsOn[current] == 1) {                                               //check if button was on
          if (!PRESET2 && presetState) {
            midiCHANNEL = midiCh[current] + 1;
          } else {
            midiCHANNEL = midiCh[current];
          }
          
          if (SHIFT && (!lfoOnState || current > 3)) {                                                      //is shift pushed in and LFO is off or or its not an lfo control?
            midiCHANNEL = (midiCHANNEL + 2) % 16;                                              //offset channel by 2 and roll around if it goes beyond 16
          }

          sendUNoff(DATA1[current], DATA2[current], midiCHANNEL);
          sendHNoff(DATA1[current], DATA2[current], midiCHANNEL);
          noteIsOn[current] = 0;                                                        //flag the note as off
        }
        //this is what it does if the button is off and it was off last iteration
        //nothing

      }
    }
  }
}

