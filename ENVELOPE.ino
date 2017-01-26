byte oldADSRdata_send[4] = {0, 0, 0, 0};
long trigMillis[4] = {0, 0, 0, 0};
long ADSRtimer[4] = {0, 0, 0, 0};
bool ADSRtrig[4] = {false, false, false, false};
unsigned long ADSRVAL[4] = {0, 0, 0, 0};
unsigned long releasePoint[4] = {0, 0, 0, 0};
bool ADSRrunning[4] = {false, false, false, false};
bool ADSRgate[4] = {false, false, false, false};
bool oldADSRgate[4] = {false, false, false, false};
#define ADSRdepth 2147483648 //max 31 bit number ADSR output will be 10 bit in CV and 7 bit in MIDI
bool ADSRstep[4] = {false, false, false, false};  //false is attack true is decay


int A = 100;    //millis
unsigned long attackStep = 0;
int D = 100;   //millis
unsigned long decayStep = 0;
unsigned long S = ADSRdepth / 2;  //0-1024 for 10bit CV (>>3 for 7bit CC)
int R = 200;   //millis
unsigned long releaseStep = 0;
unsigned long millisTicker = 0;

void handleADSR() {
  //Serial.println(millis());
  if (SHIFT) {
    A = oldRaw[lfoWaveKnob] + 2;
    D = (oldRaw[lfoDepthKnob] << 1) + 2;
    S = oldRaw[lfoFreqKnob] + 2; //scale 10bit to 31bit
    S = S << 21;
    if (S < 16777217) {
      S = 0;
    } //pull S down to zero
    R = (oldRaw[lfoOffsetKnob] << 1) + 2;
  }

  for (int i = 0; i < 4; i++) {
    if (oldRaw[ARCADE1 + i] > 512 | bitRead(midiADSRgate,i)) {
      
      ADSRgate[i] = true;
    } else {
      ADSRgate[i] = false;
    }



    if (ADSRgate[i] && !oldADSRgate[i]) { //if we are detecting a transition from low to high
      ADSRrunning[i] = true;
      trigMillis[i] = millis();
      ADSRstep[i] = 0;
      //triggerADSR(i);         //ALL THIS DOES IS SPIT OUT DATA ABOUT PARAMETERS
     //// Serial.println("TRIGGERED!!!");
      oldADSRgate[i] = true;
    } else if (oldADSRgate[i] && !ADSRgate[i]) { //if we are detecting a transition from high to low
    // // Serial.println("released");
      oldADSRgate[i] = false;
      trigMillis[i] = millis();
      releasePoint[i] = ADSRVAL[i];
    }
    ADSR(i);
  }
}



void triggerADSR(int i) { //all this does is write some shit brah
 //// Serial.print("   ATTACK = ");
 //// Serial.print(A);
 //// Serial.print("   DECAY = ");
 //// Serial.print(D);
 //// Serial.print("   SUSTAIN = ");
 //// Serial.print(S);
 //// Serial.print("   RELEASE = ");
 // Serial.println(R);
}

void updateAnimationStepSizes(int i) {
  attackStep =  ADSRdepth / A; //calculate how far ADSR needs to step per frame
  // decayStep = (ADSRdepth - S) / D; //how far the decay needs to travel every step
  decayStep = ADSRdepth / D; //how far the decay needs to travel every step
  //releaseStep = S / R; //how far release needs to fall every step
  releaseStep = ADSRdepth / R; //how far release needs to fall every step

}


void ADSR(int i) {

  if (ADSRrunning[i]) {
    updateAnimationStepSizes(i);
    unsigned long millisNow = millis();
    if (millisNow > millisTicker) {
      millisTicker = millisNow;
      ADSRtimer[i] = millisTicker - trigMillis[i];
      //Serial.print(  ADSRtimer  );

      if (ADSRgate[i]) {


        ////////////ATTACK/////////

        if (!ADSRstep[i]) {                                   //If we are in ATTACK step
          //  // Serial.print("attackStep = ");
          //  // Serial.print(attackStep);
          //   // Serial.print("  ATTACK  ");
          //   // Serial.println(ADSRtimer[i]);
          //          ADSRVAL[i] += attackStep;
          ADSRVAL[i] = attackStep * ADSRtimer[i];
          //          ADSRVAL = constrain(ADSRVAL, 0, ADSRdepth - 1);
          if (ADSRVAL[i] > ADSRdepth - attackStep) {
            ADSRVAL[i] = ADSRdepth;
            ADSRstep[i] = true;                                              //flag transition do decay
            trigMillis[i] = millisTicker;                                   //reset timer
          }
          /////////// DECAY SUSTAIN ///////////
        } else if (ADSRstep[i]) {                           //If we are in DECAY step
          // // Serial.print("decayStep = ");
          //  // Serial.print(decayStep);
          //   // Serial.print("  DECAY   ");
          //   // Serial.println(ADSRtimer[i]);
          if (ADSRVAL[i] != S) {
            ADSRVAL[i] = ADSRdepth - (decayStep * ADSRtimer[i]); // pull ADSRVAL down to the sustain LEVEL
            if (ADSRVAL[i] < S && ADSRVAL > (ADSRdepth + 10)) { //if we have passed sustain level or rolled around
              ADSRVAL[i] = S;
            }
          }
        }

        ///////////////RELEASE///////
      }  else { //IF GATE IS RELEASED!
        //// Serial.print("releaseStep = ");
        //// Serial.print(releaseStep);
        //// Serial.print("  RELEASE ");

        ADSRVAL[i] = releasePoint[i] - (releaseStep * ADSRtimer[i]);
        //Serial.print(" ADSRVAL -> ");
        //Serial.print(ADSRVAL[i]);
        //Serial.println(" <- ADSRVAL ");
        if (ADSRVAL[i] > (ADSRdepth + 10)) { // if we have rolled around
          ADSRVAL[i] = 0;
          ADSRrunning[i] = false;
          bitClear(midiADSRrunning,i);                                                      //tell the world that if we were keeping the adsr on for midis sake we can turn it off now
          //    // Serial.println( " STOPPED " );
        }
      }
      if (envToCV) {
        int CV_OUT = constrain(ADSRVAL[3] >> 21,0,1023);
        
        //cvOUT(ADSRVAL[3] >> 21);
        cvOUT(CV_OUT);
      }
      byte data_send = ADSRVAL[i] >> 24;
      if (data_send != oldADSRdata_send[i]) {
        //int ledBrightCompare = ADSRVAL[i] >> 23;
        //if (ledBrightCompare > LedBright){
        LedBright = data_send << 1;
        analogWrite(ledPin, LedBright);//LedBright
       //// Serial.println(S);

        //}
        byte CCnumber = DATA1[ARCADE1 + i + 16]; //read CCnumber off the mythical plexor4 last 4 values
        byte midiChannel = midiCh[ARCADE1 + 16 + i]; //not to be confused with midiCHANNEL !!
        sendUCC(CCnumber, data_send, midiChannel);
        sendHCC(CCnumber, data_send, midiChannel);
        //Serial.print(ADSRtimer[i]);
        //Serial.print(" ADSR no.");
        //Serial.print(i);
        //Serial.print(" = ");
        //Serial.println(data_send);
        oldADSRdata_send[i] = data_send;
      }
    }
  }
}
