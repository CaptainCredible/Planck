void handleReceiveNoteOn(byte note, byte channel, byte velocity) {
//Serial.print(channel);
//Serial.print(" - ");
//Serial.print(note);
//Serial.print(" - ");
//Serial.println(velocity);
switch (channel) {
    case 1:                                                //dont know why we have to offset by 16, I probably fucked up somewhere else in the code and now i cant be arsed to find the mistake
      digitalWriteFast(out1, HIGH);
      if(CALIBRATED){                                                     //if it's calibrated we can pull the correct duty cycles out of the arraty

      } else {                                                            //if it's not calibrated we can just throw the MIDI pitch out of the back
      if(note>47 && note < 95){
      cvOUT(noteArray[note-48]);

      }
      }
      break;
    case 2:
      digitalWriteFast(out1, HIGH);
      cvOUT(velocity << 3);
      //Serial.println(velocity<<3);
      break;

    case 3:
      digitalWriteFast(out1, HIGH);
      break;

    case 4:
      cvOUT(1024);
      break;
    default:
      break;
  }
}

void handleReceiveNoteOff(byte note, byte channel) {
  switch (channel) {
    case 1:
      digitalWriteFast(out1, LOW);
      break;

    case 2:
      digitalWriteFast(out1, LOW);
      break;

    case 3:
      digitalWriteFast(out1, LOW);
      break;

    case 4:
      cvOUT(0);

    default:
      break;
  }
}



