void sendUCC(byte CCno, byte CCval, byte ch) {
  midiEventPacket_t event = {0x0B, 0xB0 | ch - 1, CCno, CCval};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
  digitalWriteFast(ledPin, HIGH);                                                                        //flash the activity led
  onTime = millis(); ledPinState = true;                                                                 // write down what time we turned the led on
}

void sendHCC(byte CCno, byte CCval, byte ch) {
  MIDI.sendControlChange (CCno, CCval, ch);
  digitalWriteFast(ledPin, HIGH);                                                                        //flash the activity led
  onTime = millis(); ledPinState = true;                                                                 // write down what time we turned the led on
}

void sendUNon(byte note, byte velocity, byte ch) {
  midiEventPacket_t noteOn = {0x09, 0x90 | ch - 1, note, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
  digitalWriteFast(ledPin, HIGH);                                                  //turn on LED
  onTime = millis(); ledPinState = true;
}

void sendHNon(byte note, byte velocity, byte ch) {
  MIDI.sendNoteOn (note, velocity, ch);            //Send noteOn
  digitalWriteFast(ledPin, HIGH);                                                  //turn on LED
  onTime = millis(); ledPinState = true;
}

void sendUNoff(byte note, byte velocity, byte ch) {
  midiEventPacket_t noteOff = {0x08, 0x80 | ch - 1, note, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
  digitalWriteFast(ledPin, HIGH);                                                  //turn on the led
  onTime = millis(); ledPinState = true;                                                            //write down when the led was turned on
}

void sendHNoff(byte note, byte velocity, byte ch) {
  midiEventPacket_t noteOff = {0x08, 0x80 | ch - 1, note, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
  digitalWriteFast(ledPin, HIGH);                                                  //turn on the led
  onTime = millis(); ledPinState = true;                                                            //write down when the led was turned on
}

void sendHPB(int bend, int channerl) {
  MIDI.sendPitchBend((bend / 2) - 8192, channerl);

}

void sendUPB(int bend, int channerl) {
  byte hi = bend >> 8;
  midiEventPacket_t event = {14, channerl + 223, bend, hi};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

