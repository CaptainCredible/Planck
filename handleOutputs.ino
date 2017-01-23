void cvOUT(int CV) {
  TC4H = CV >> 8;
  OCR4D = 0xFF & CV;
}


//USBRECEIVE CODE NEEDS TO MIGRATE HERE TO MERGE WITH HARDWARE MIDI CODE!!!!
void handleOutputs(byte note, byte velocity, bool onOff){
  
}

