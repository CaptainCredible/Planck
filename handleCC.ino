void CC(int plexer, int rawVal, int current) {

  int rawDiff = rawVal - oldRaw[current];                                                                 //what is the delta to the latest "anchored" oldVal?
  if (rawDiff <= -8 || rawDiff >= 8) {                                                                    //only do anything if the raw value that we read has changed by more than 8
    oldRaw[current] = rawVal;                                                                             //reset the oldRaw "anchor"
    byte data_send = rawVal / 8;                                                                          //scale the rawVal down to nice and safe 7bit midi size

    if (inMin[current] < inMax[current]) {                                                                //if the input mapping is non-inverting
      data_send = constrain(data_send, inMin[current], inMax[current]);                                   //constrain the data_send so it doesn't exceed the mapping input and thus generate values outside the mapping output
    } else {                                                                                              //if the mapping is inverted then inMin is goiung to be bigger than inMax
      data_send = constrain(data_send, inMax[current], inMin[current]);                                   //therefore we need to switch them around in the constrain function
    }                                                                                                     //now we are sure that the mapped values will not exceed the outMin/outMax limits, thank god for that.


    data_send = map(data_send, inMin[current], inMax[current], outMin[current], outMax[current]);         //Map the data_send value as per requested by toolmans sysex editor

    //if we have an LFO
    if (MAP) {                                                                                          //check if the LFO mapping button is turned on
      cc_lfo = DATA1[current];                                                                          //if so, set the LFO CC number to the current CC number
      ch_lfo = midiCh[current];                                                                         //and set the LFO channel to the current channel
#ifdef DEBUG
      Serial.print("mapped");                                               //DEBUG
      Serial.print(DATA1[current]);                                         //report what was mapped
#endif
    }


    if (oldMIDIVal[current] != data_send) {                                                //has the MIDI value changed?
      if (lfoOnState && SHIFT && current < 4) {                                            //is the LFO on? do we want to ignore LFO knobs? HEY THIS WONT WORK IF WE REMAP THE CC to note!
        lfoControls[current] = rawVal;                                                     //harvest the Raw Values for use as higher rez LFO controls
      } else if (!PRESET2 && presetState) {                                                //is the preset switch flipped but no preset is stored?
        midiCHANNEL = (midiCh[current] + 1) % 16;                                          //send MIDI on other channel (not LFO though) 
      } else {                                                                             //if preset is normal
        midiCHANNEL = midiCh[current];                                                     //dont do shit
      }                                                                                     
      if (SHIFT && (!lfoOnState||current>3)) {                                                          //is shift pushed in and LFO is off or or its not an lfo control? 
        midiCHANNEL = (midiCHANNEL + 2) % 16;                                              //offset channel by 2 and roll around if it goes beyond 16
      }
      sendUCC(DATA1[current], data_send, midiCHANNEL);
      sendHCC(DATA1[current], data_send, midiCHANNEL);
    }
    oldMIDIVal[current] = data_send;                                                                    // set a new oldval to compare against next time


    //}
  }

}
