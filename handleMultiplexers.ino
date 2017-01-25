

void handleMultiplexers() {
  for (int i = 0; i < 8; i++) {//loop 8 times, one for each input on the plexers
    PORTB = (PORTB & B10001111) | (i << 4);                                       //select input 1-8 skipping bits 0 and 1 (rx and tx) and leaving all the other bits alone (outputting on pins 8 9 10)
    for (int currentPlexer = 0; currentPlexer < numberOfPlexers; currentPlexer++) { //iterate as many times as number of multiplexers and increment currentPlexer for each time
      arrayCursor = (currentPlexer * 8) + i;                                          //referance point to read the right values in the arrays

      analogRead(currentPlexer);                                                    //switch the internal ADMUX to wait for the value to settle
      int val = analogRead(currentPlexer);

      if (CalibrationMode) {  //what to do only if we are in calibrate mode
        handleCalibration(val);
      }


if(ArcadeMute && (arrayCursor >= ARCADE1 && arrayCursor <= ARCADE4)){
//  do nothing no sending any data
oldRaw[arrayCursor]=val;

      //DETECT SHIFT
    } else if (arrayCursor == 5) { //is it shift button?
        if (val > 500 && !SHIFT) {

          SHIFT = true;
          if (CalibrationMode) {
            CALIBRATE--;
            cvOUT(CALIBRATE);
#ifdef DEBUG
            Serial.println(CALIBRATE);
#endif
          }
        } else if (val <= 500 && SHIFT) {

          SHIFT = false;
        }


        //DETECT MAP
      } else if (arrayCursor == 4) { //is it MAP button
        if (val > 500 && !MAP) {      //was it just pushed?
          MAP = true;                  //we are mapping then

          if (CalibrationMode) {          //are we in calibration mode?
            CALIBRATE++;                    //up the CALIBRATE value by one
#ifdef DEBUG                                //DEBUG
            Serial.println(CALIBRATE);      //DEBUG
#endif                                        //DEBUG
            cvOUT(CALIBRATE);                 //set the cvOUT to the new CALIBRATE LEVEL
          }

          mapTimer = millis();                //Set a reference time for the timer  for cvLFO
        } else if (val <= 500 && MAP) {
          MAP = false;
        }

      } else if (arrayCursor == 3 && SHIFT && lfoOnState) {                         //IS IT LFO FREQ?

        if (lfoRawFreq != val) {
          lfoRawFreq = val;                                   //hold on the the raw value regardless of noise!!!
          newLfoFreq = true;                                  // make a note of the fact that we have a new LFO frequency and we'll want to do that floating point math on it later
        }

      } else if (isNote[arrayCursor] == 1) {                                            //is this pot/switch/whatever defined as a note?
        Note(plexerSelect, val, arrayCursor);                                         //treat it as a note, (also divide raw reading by 8 cos we dont really need more than that
      } else {                                                                        //not a note eh? well then its a CC i guess
        CC(plexerSelect, val, arrayCursor);                                           //treat it as a CC
      }
    }
  }


  if (millis() - mapTimer > 1000 && MAP && !CVlfo) {                                  //check if MAP has been held in for 1000 milliseconds
    CVlfo = true;                                                                     //set the LFO to CV out!

#ifdef DEBUG                                                                          //DEBUG
    Serial.println("CVLFO YES");                                                      //DEBUG
#endif                                                                                //DEBUG

  } else if (millis() - mapTimer < 1000 && MAP && CVlfo) {
    CVlfo = false;
#ifdef DEBUG
    Serial.println("CVLFO NO");
#endif
    //digitalWrite(out2, LOW);
    cvOUT(0);                                                                         //SET CV OUTPUT TO ZERO WHEN CVLFO IS TURNED OFF
  }

}
