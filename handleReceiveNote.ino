byte ADSRselector = 0;

void handleReceiveNoteOn(byte note, byte channel, byte velocity) {
  //Serial.print(channel);
  //Serial.print(" - ");
  //Serial.print(note);
  //Serial.print(" - ");
  //Serial.println(velocity);
  switch (channel) {
    case 1:                                                
      digitalWriteFast(out1, HIGH);    //GATE 1 HIGH
      if (note > 47 && note < 95) {
        cvOUT(noteArray[note - 48]);   //NOTE PITCH CV
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
    case 5:
      ADSRselector = note % 4;
      bitSet(midiADSRgate, ADSRselector);
      bitSet(midiADSRrunning, ADSRselector);
      break;
      
      case 6:
      if (note > 47 && note < 95) {
        cvOUT(noteArray[note - 48]);   //NOTE PITCH CV
      }
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

    case 5:
      ADSRselector = note % 4;
      bitClear(midiADSRgate, ADSRselector);
      //midiADSRgate[ADSRselector] = false;
      break;

    default:
      break;
  }
}



