int trigMillis = 0;
bool ADSRtrig = false;
int ADSRVAL = 0;
bool ADSRrunning;
long ADSRtimer;
int A = 10;    //millis
int D = 100;   //millis
int S = 50;    //0-1024 for 10bit CV (>>3 for 7bit CC)
int R = 100;   //millis
bool releaseBleedOut = false;

void triggerADSR() {
  ADSRrunning = true;
}

byte ADSR() {
  if (ADSRrunning) {
    ADSRtimer = millis() - trigMillis;
    if (ADSRtimer < A) {

      //here should be code to rise to 1024 within time limit

    } else if (ADSRtimer < D) {

      //here should be code to fall to sustain value

    } 

  }
  Serial.println(ADSRVAL);
}

