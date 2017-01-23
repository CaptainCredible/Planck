void handleHardwareMidiIn() {
  switch (MIDI.getType()) {                                                                     // Get the type of the message we caught

    case midi::SystemExclusive:;
      for (int i = 0; i < 14; i++) {
        sexArray[i] = MIDI.getSysExArray()[i];
      }
      handleMidiSysexReceive();
      break;

    case midi::NoteOn:;                                                                     //was it a NOTE ON?
      if (MIDItoUSB) {
        sendUNon(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());                         //SEND IT BACK OUT USB AND MIDI
      }
      if (HARDWARETHRU) {
        sendUNon(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());                         //SEND IT BACK OUT USB AND MIDI
      }
      handleReceiveNoteOn(MIDI.getData1(), MIDI.getChannel(), MIDI.getData2());             //Send this info to the function responsible for conrolling CVout
      break;

    case midi::NoteOff:;
      if (MIDItoUSB) {
        sendUNoff(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
      }
      if (HARDWARETHRU) {
        sendHNoff(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
      }
      handleReceiveNoteOff(MIDI.getData1(), MIDI.getChannel());
      break;

    case midi::ControlChange:;
      if (MIDItoUSB) {
        sendUCC(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
      }
      if (HARDWARETHRU) {
        sendHCC(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
        handleAllCCreceive(MIDI.getData1(), MIDI.getData2());
      }
      handleAllCCreceive(MIDI.getData1(), MIDI.getData2());
      break;

    case midi::PitchBend:;
      handleReceivePB(MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
      break;
    default:                                                                                 // this is what happens when we receive any other midi than what we are looking for

      break;

  }
#ifdef TURNTHRUOFF

#else
  digitalWriteFast(ledPin, HIGH);                                                                        //flash the activity led
  onTime = millis(); ledPinState = true;                                                                                  // write down what time we turned the led on
#endif
}




