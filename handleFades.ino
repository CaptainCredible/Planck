/*void handleFades() {                                                                  //handle ledstrips that are currently fading out
#ifdef LEDSTRIPS
  // check fades flag
  for (int i = 0; i < 3; i++) {                                                       // scan fades to find active ones
    if (isFading[i]) {                                                                // if RED fade is on
      ledFades[i] = map((ledTimers[i] - millis()), polyDecayTime[i], 0, ledVal[i], 0); // calculate the current ledFades value by mapping
      if ((ledFades[i] <= 0) || (millis() > ledTimers[i])) {                           // if the led has faded all the way out or the timer has passed the set time for this fade (surely i only need one of these?)
        isFading[i] = false;
        --aFadeIsOn;

      }
      //      ledVal[i] = ledFades[i];
      analogWrite(RGBpins[i], ledFades[i]);
    }
  }
#endif
}
*/

