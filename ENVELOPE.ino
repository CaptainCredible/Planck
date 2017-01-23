int trigMillis = 0;
bool ADSRtrig = false;
unsigned long ADSRVAL = 0;
bool ADSRrunning;
long ADSRtimer;
bool ADSRgate = false;
bool oldADSRgate = false;
#define ADSRdepth 2147483648 //max 31 bit number ADSR output will be 10 bit in CV and 7 bit in MIDI
byte ADSRstep = 0; // 0 = A   1 = D   2 = S   3 = R


int A = 100;    //millis
unsigned long attackStep = 0;


int D = 100;   //millis
unsigned long decayStep = 0;


unsigned long S = ADSRdepth / 2;  //0-1024 for 10bit CV (>>3 for 7bit CC)


int R = 200;   //millis
unsigned long releaseStep = 0;

unsigned long releasePoint = 0;



unsigned long millisTicker = 0;

void handleADSR() {
  if (oldRaw[ARCADE1] > 512) {
    ADSRgate = true;
  } else {
    ADSRgate = false;
  }


  if (ADSRgate && !oldADSRgate) { //if we are detecting a transition from low to high
    triggerADSR();
    Serial.println("TRIGGERED!!!");
    oldADSRgate = true;
  } else if (oldADSRgate && !ADSRgate) {
    Serial.println("released");
    releasePoint = ADSRVAL;
    oldADSRgate = false;
  }
  ADSR();
}



void triggerADSR() {
  ADSRrunning = true;
  updateAnimationStepSizes();
  trigMillis = millis();
  ADSRstep = 0;
  Serial.print("   ATTACK = ");
  Serial.print(A);
  Serial.print("   DECAY = ");
  Serial.print(D);
  Serial.print("   SUSTAIN = ");
  Serial.print(S);
  Serial.print("   RELEASE = ");
  Serial.println(R);
}

void updateAnimationStepSizes() {
  attackStep =  ADSRdepth / A; //calculate how far ADSR needs to step per frame
  decayStep = (ADSRdepth - S) / D; //how far the decay needs to travel every step
  releaseStep = S / R; //how far release needs to fall every step


}


byte ADSR() {
  A = oldRaw[lfoWaveKnob] + 1;
  D = (oldRaw[lfoDepthKnob] << 1) + 1;
  S = oldRaw[lfoFreqKnob] + 1; //scale 10bit to 31bit
  S = S << 21 + 1;
  R = oldRaw[lfoOffsetKnob] + 1 << 1;



  if (ADSRrunning) {
    updateAnimationStepSizes();
    if (millis() > millisTicker) {
      millisTicker = millis();
      ADSRtimer = millisTicker - trigMillis;
      //     Serial.print(  ADSRtimer  );

      if (ADSRgate) {


        ////////////ATTACK/////////

        if (ADSRstep == 0) {                                   //If we are in ATTACK step
          Serial.print("attackStep = ");
          Serial.print(attackStep);
          Serial.print("  ATTACK  ");
          ADSRVAL += attackStep;
          //          ADSRVAL = constrain(ADSRVAL, 0, ADSRdepth - 1);
          if (ADSRVAL > ADSRdepth) {
            ADSRVAL = ADSRdepth;
            ADSRstep = 1;                                              //flag transition do decay
          }
          /////////// DECAY SUSTAIN ///////////
        } else if (ADSRstep == 1) {                           //If we are in DECAY step
          Serial.print("decayStep = ");
          Serial.print(decayStep);
          Serial.print("  DECAY   ");
          if (ADSRVAL != S) {
            ADSRVAL -= decayStep; // pull ADSRVAL down to the sustain LEVEL
            if (ADSRVAL < S + decayStep) { //if we are going to roll past 0
              ADSRVAL = S;
            }
            //ADSRVAL = constrain(ADSRVAL, S, ADSRdepth);      //make sure we dont pass the sustain level   //if statement should effectively constrain this beyotch
          }
        }

        ///////////////RELEASE///////
      }  else { //IF GATE IS RELEASED!
        Serial.print("releaseStep = ");
        Serial.print(releaseStep);
        Serial.print("  RELEASE ");
        if (ADSRVAL < (releaseStep)) { // if we have got all the way back to 0
          ADSRVAL = 0;
          ADSRrunning = false;
          Serial.println( " STOPPED " );
        } else {
          ADSRVAL = ADSRVAL - releaseStep;
        }

        //code for return to 0
      }
      cvOUT(ADSRVAL >> 21);

      Serial.println(ADSRVAL);
    }
  }
}
