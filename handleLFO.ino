void handleLFO() {
  //THIS IS THE MULTIPLEXOR FOR LFO PARAMETERS




  if (SHIFT) {
    lfoDepth = lfoControls[lfoDepthKnob] / 8;
    if(newLfoFreq){                                         //make sure we only do this heavy floating point math if the frequency knob has moved!
    lfoFreq = lfoRawFreq * 0.0002;
    newLfoFreq = false;
    //Serial.print("DID MATH - ");
    //Serial.println(lfoFreq);
    }
    lfoWave = lfoControls[lfoWaveKnob] / 205;
    lfoOffset = lfoControls[lfoOffsetKnob] / 8;
  }



  //CODE FOR SINE WAVE
  if (lfoWave == 0) {                                       //is sine selected?
    lfoState = (sin(PiInc) * lfoDepth) + lfoOffset;         //calculate point in sine and write to lfostate
  }

  //CODE FOR SAWTOOTH
  if (lfoWave == 1) {                                       //is sawtooth selected?
    lfoState = (((PiInc / PI) - 1) * lfoDepth) + lfoOffset; //calculate point in sawtooth
  }

  // CODE FOR RAMP
  if (lfoWave == 2) {                                                     //is ramp selected
    lfoState = ((((PiInc / PI) - 1) * lfoDepth) * -1) + lfoOffset;        //calculate point in ramp
  }

  //CODE FOR SQUARE
  if (lfoWave == 3) {                                                     //3 means square
    if (PiInc > PI) {                                                     //calcerlert square
      lfoState = lfoDepth * -1 + lfoOffset;
    }
    else {
      lfoState = lfoDepth + lfoOffset;
    }
  }

  //CODE FOR S/H
  if (lfoWave == 4 ) { //|| lfoWave==5                                    //randumb sermple and herld
    if (PiInc < 5) {
      if (SHflag == false) {                                              //generate random number when pi inc goes back to 0
        lfoState = random(-lfoDepth, lfoDepth) + lfoOffset;
        SHflag = true;                                                    //flag the fact that we just made a new random value
      }
    }
    else {
      SHflag = false;                                                     //reset the flag for next round
    }
  }

  //THIS WORKS BUT it moves way too fast and the resolution is too low so it has to be done in
  //floating point part of the code so it can change more subtly per increment


  //SLEW
  if (slew > 1) {                                                                                   //is there any slewing to be done at all
    //slew =  slew / 16;                                                                              //slew is inverted and scaled down to midi world
    float diff = lfoState - OldLfoState;   //if we are going up we get a positive number
    if (diff > 0) {   //if we are going up
      slewstate = OldLfoInteger + slew; //step up from old value by slew amount
      if (slewstate > lfoInteger) {   //if we overshoot
        slewstate = lfoInteger;      //set slewstate to target
      }
    }
    if (diff < 0) {
      slewstate = OldLfoInteger - slew;
      if (slewstate < lfoInteger) {
        slewstate = lfoInteger;
      }
    }
    //slew = map(slew, 0, 1024,0,3); //undesireable technique, renders only 4 steps of slewing

    lfoInteger = slewstate;
  }


  lfoInteger = (int) lfoState;
  lfoInteger = constrain(lfoInteger, 0, 127);


  LedBright = lfoInteger * 2;
  analogWrite(ledPin, LedBright);//LedBright
  if (CVlfo) {
    cvOUT(LedBright * 4);
  }

  if ((lfoInteger != OldLfoInteger) && !CVlfo) {
    sendUCC(cc_lfo, lfoInteger, ch_lfo);
    sendHCC(cc_lfo, lfoInteger, ch_lfo);
    //MIDI.sendControlChange (cc_lfo, lfoInteger, ch_lfo);
    OldLfoInteger = lfoInteger;
  }

  //if (millis() - PiStep > PiREZ) {
    PiStep = millis();
    PiInc = PiInc + (lfoFreq);
    if (PiInc >= 2 * PI) {
      PiInc = 0;
      int MIDIpi = (int) PiInc;
    }
  //}



  // if (lfoOnSwitchState == LOW){
  //   analogWrite(lfoLedPin, 0 );
  // }

  // RESET LFO TO ORIGINAL
  if (MAP && SHIFT) {
    if ((PRESET1 && !presetState) || PRESET2 && presetState ) {                   //If there is a preset stored for the current state of presetswitch read it
      cc_lfo = constrain(EEPROM.read(450 + Preset), 0, 127); // address 450
      ch_lfo = constrain(EEPROM.read(451 + Preset), 1, 16);  // address 451
    }
  }
}

