

void loop() {

  //delay(20);                          //debug, makes it easier to see plexer scans and delays
  if (!sysExFlag) {                                         //dont run any of this shit if we are busy receiving and storing sysex, eeprom is slooooooow


    handleSwitches();
    handleMultiplexers();                                           //handle the multiplexers

    /*
     // Serial.print("ARCADE1 = ");
     // Serial.println(oldRaw[ARCADE1]);
     // Serial.print("ARCADE2 = ");
     // Serial.println(oldRaw[ARCADE2]);
     // Serial.print("ARCADE3 = ");
     // Serial.println(oldRaw[ARCADE3]);
     // Serial.print("ARCADE4 = ");
     // Serial.println(oldRaw[ARCADE4]);
    */
    //    handleToggleSwitches();                                         //handle shift and lfo toggle buttons
    if (lfoOnState) {                                                 //if LFO switch is On and PRESET switch is off
      handleLFO();                                                      //turn on LFO
    }
  // Serial.println(ArcadeMute);
    
    
    
    
    if (ADSROnState || midiADSRrunning) {
      handleADSR();                                                     //if LFO on and Preset on, turn on ADSR
    //   Serial.print(midiADSRrunning);
    //   Serial.println(" handled ADSR ");
       
    }






    //   if (aFadeIsOn > 0) {                                            // is there a LED fading out
    //   handleFades();                                                // handle it
    //   }


    if (!lfoOnState && !ADSROnState) {                                                 // if LFO and ADSR are off
      if (((millis() - onTime) > 100) && (ledPinState)) {              // Handle midi activity LED
        digitalWriteFast(ledPin, LOW);                                 // Turn off led if its that time again
        ledPinState = false;
      }
    }
  }

  if (MIDI.read()) {                                                // did you hear something on the DIN ?
    handleHardwareMidiIn();                                                 // what are we gonna do about that then
  }



  //USBMIDIRECEIVE CODE
  midiEventPacket_t USBrx;
  do {
    USBrx = MidiUSB.read();                                         //Check the usb tubes?
    if (USBrx.header != 0) {                                        //is there data there?
      if (USBTHRU) {
        MidiUSB.sendMIDI(USBrx);                                      //send back the received MIDI command
      }
      // handleUSBMidiIn();                                            //fuck this is all going to have to happen in the loop :(

      switch (USBrx.header) {
        case 4:    //
        case 6:   //only run this code if we are dealing with a sysex message or an end of sysex message

          sexArray[sexArraySelector] = USBrx.byte1;
          //EEPROM.write(sexArraySelector, USBrx.byte1);
          sexArraySelector++;
          if ((USBrx.byte1 == 247) && (USBrx.header == 6)) {   //end of sysex message
            //printSexArray();
            handleMidiSysexReceive();
            sexArraySelector = 0;

            break;
          }

          sexArray[sexArraySelector] = USBrx.byte2;
          //EEPROM.write(sexArraySelector, USBrx.byte2);
          sexArraySelector++;
          if ((USBrx.byte2 == 247) && (USBrx.header == 6)) {
            //printSexArray();
            handleMidiSysexReceive();
            sexArraySelector = 0;
            break;
          }

          sexArray[sexArraySelector] = USBrx.byte3;
          //EEPROM.write(sexArraySelector, USBrx.byte3);
          sexArraySelector++;
          if ((USBrx.byte3 == 247) && (USBrx.header == 6)) {
            //printSexArray();
            handleMidiSysexReceive();
            sexArraySelector = 0;
            break;
          }
          break;

        case 9:                                 //its a noteOn
          // MIDI.sendNoteOn(USBrx.byte2, (USBrx.byte1 - 143), USBrx.byte3);
          if (USBtoMIDI) {
            MIDI.sendNoteOn(USBrx.byte2, USBrx.byte3, USBrx.byte1 - 143);
          }
          handleReceiveNoteOn(USBrx.byte2, (USBrx.byte1 - 143), USBrx.byte3); //note - channel - velocity

          //digitalWriteFast(ledPin, HIGH);                                                  //turn on LED
          //onTime = millis(); ledPinState = true;
          triggerLED();
          break;

        case 8:                                  //its a note off
          if (USBtoMIDI) {
            MIDI.sendNoteOff(USBrx.byte2, USBrx.byte3, USBrx.byte1 - 127);
          }
          handleReceiveNoteOff(USBrx.byte2, (USBrx.byte1 - 127)); //note - channel
          triggerLED();
          //digitalWriteFast(ledPin, HIGH);                                                  //turn on LED
          //onTime = millis(); ledPinState = true;
          break;

        case 11:                                 //CC
          if (USBtoMIDI) {
            MIDI.sendControlChange(USBrx.byte2, USBrx.byte3, USBrx.byte1 - 175);
          }
          handleAllCCreceive(USBrx.byte2, USBrx.byte3);
          triggerLED();
          //     digitalWriteFast(ledPin, HIGH);                                                  //turn on LED
          //     onTime = millis(); ledPinState = true;
          break;

        case 14:                              //PITCHBEND
          handleReceivePB(USBrx.byte2, USBrx.byte3, USBrx.byte1 - 223);
        default:
          break;

      }
      MidiUSB.flush();  //dump that data back out the usb tubes //unnecesary ?

    }
  } while (USBrx.header != 0);


  Serial.println(EEPROMCLEAR);
}
