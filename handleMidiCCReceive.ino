/*void handleMidiCCReceive() {
  handleAllCCreceive(MIDI.getData1(), MIDI.getData2());
}
*/

void handleAllCCreceive(byte CCnombre, byte CCvalueh) {
  if (CCnombre == 13) {
    cvOUT(CCvalueh*8);
  }
}

void handleReceivePB(byte bottom, byte top, byte jackiechan){
  int PBVAL = (top << 8) + bottom;
  sendUPB(PBVAL, jackiechan);
  sendHPB(PBVAL, jackiechan);
  //Serial.println(PBVAL);
  //Serial.println(PBVAL>>5);                     //rescale from 15bit to 10bit
  if(jackiechan == THEONE){                     //make a movie referance at the same time as checking if the channel is 16
    cvOUT(PBVAL>>5);                            //set the CVout to PBval and rescale it from 15bit to 10bit
  }
  //Serial.println(PBVAL);

  
  
}

