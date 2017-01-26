#define DEBUG
/*
  PLANCK CODE, By Daniel Lacey-McDermott a.k.a. Captain Credible, the international Space Hero
  -programmable via sysex over DIN or USB
  -THRU all notes and CC from DIN to USB
*/



#include <MIDIUSB.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <EEPROM.h>
#include <digitalWriteFast.h>


#define ARCADE1 12   //guessed
#define ARCADE2 13
#define ARCADE3 14
#define ARCADE4 15

#define SHIFTBUTTONNO 8 //8 is NOT CORRECT
#define MAPBUTTONNO 9  //9 is NOT CORRECT
#define THEONE 16  //jesus christ, why did I have to call it the one? I cant remember what the fuck this is!

#define HIGHSPEED                                           //set to high speed
#define DISABLERUNNINGSTATUS                                //running status causes all kinds of trouble, fuck that shit
#define TURNTHRUOFF                                       //turn off midi thru
#define DEBUGMODE                                           // simplified operation for faster development and debug
#define LEDSTRIPS                                           //use LEDSTRIPS

bool EEPROMCLEAR = false;
const boolean LFO = false; //enable or disable LFO
//boolean PRESETS = false; //enable or disable presets
boolean PRESET1 = false;//should be true if there is 123 in EEPROM 400
boolean PRESET2 = false;//should be true if there is 123 in EEPROM 401
boolean envToCV = false;
byte midiADSRgate = 0;
byte midiADSRrunning = 0;

/////////////////
//ROUTING////////
/////////////////
boolean HARDWARETHRU = true;                                   //Route input DIN back out through output
boolean MIDItoUSB = true;                                       //Route DIN to USB
boolean USBTHRU = true;                                        //Route USBinput back out through usb
boolean USBtoMIDI = true;                                       //Route USB input to DIN MIDI

#define numberOfPlexers  2                                      //how many multiplexer do you have connected bitch?

/////////////////////
///CV CALIBRATION ///
/////////////////////

int CALIBRATE = 0;
bool CalibrationMode = false;
int calibnotestep = 0;
bool CALIBRATED = false;
bool newLfoFreq = false;

bool ArcadeMute = false;

long int shiftBounce = 0;           //debounce timer to avoid shift switch glitch
long int lfoBounce = 0;             // debounce timer to avoid lfo switch glitch
bool lastToggleShift = false;       // boolean to remember shift switch state
bool lastToggleLfo = false;         // boolean to remember lfo switch state
bool lfoOnState = false;            //boolean to flag lfo on/off state
bool ADSROnState = false;
bool lfoSwitchState = false;
bool presetState = false;            //boolean to flag shift on/off state /presets
bool SHIFT = false;
bool ledPinState = false;        // keep track of whether we need to spend precious time digitalWriting the ledPin
bool MAP = false;
long mapTimer = 0;                  //keep track of how lont the map button has been pressed in!
bool CVlfo = false;
long PiStep = 0;                    //limit the rate of Pi Increment
int PiREZ = 1;




//pins:
#define leftSwitch 7
#define rightSwitch 4
#define presetSwitch 7
#define leftButton 14
#define rightButton 16

#define lfoDepthKnob 0
#define lfoWaveKnob 1
#define lfoOffsetKnob 2
#define lfoFreqKnob 3
#define lfoOnSwitch 4
int DebounceT = 0;
const byte lfoLedPin = 7;
//const byte mapPin = 15;   //must be set to pullup  NOT EXIST ANY MORE!!
const byte ledPin = 3;    //PLANCK 3
#define out1 5
#define out2 6

//ELIMINATE THESE: REMOVE THE DEBOUNCING TOGGLE BUTTONS CODE
//const byte LfoSwitchPin = 7;         //pin for reading the lfo button
//const byte ShiftButtonPin = 16;
//const byte ShiftSwitchPin = 10;      //pin for reding the shift button
//const byte ShiftLedPin = 7 ;          //pin for the shift led
//const byte DebounceT = 100;          //debounce timer setting
//const byte presetLed = 8;

//LEDSTRIP
#ifdef LEDSTRIPS
const int RGBpins[3] = {9, 10, 11};                                      // pins that the ledstrip is connected to, need to be PWM pins

unsigned int       ledVal[3] = {0, 0, 0};                                // current value for the red green and blue led
unsigned int       ledFades[3] = {0, 0, 0};                              // fade value for red green blue
unsigned long      ledTimers[3] = {0, 0, 0};                             // timers to keep track of fades for red green and blue
unsigned int       masterDecayTime = 0;                                // current decay time
unsigned int       polyDecayTime[3] = {0, 0, 0};                         // decay time as measured when the leds note was released
boolean            isLedOn[3] = {false, false, false};                   // flag to see if a led is on
boolean            isFading[3] = {false, false, false};                  // flag to see if a led is fading
int                aFadeIsOn = 0;
#endif

int noteArray[48] {0, 15, 33, 51, 71, 91, 110, 129, 149, 167, 186, 205, 223, 242, 262, 282, 302, 317, 337, 356, 374, 393, 412, 431, 450, 469, 488, 507, 526, 545, 564, 583, 602, 621, 640, 660, 679, 697, 715, 734, 755, 774, 793, 811, 831, 851, 871}; //EEPROM ADRESSES 900 - 948 + some others for the other bits of the int

int sexArraySelector = 0;
byte sexArray[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int oldRaw[32] = {                            // to check how far the pot has wandered since last midi update
  -99, -99, -99, -99, -99, -99, -99, -99,    //plexer1
  -99, -99, -99, -99, -99, -99, -99, -99,   //plexer2
  -99, -99, -99, -99, -99, -99, -99, -99,  //plexer3
  -99, -99, -99, -99, -99, -99, -99, -99, //plexer4
};

int oldMIDIVal[32] = {                        // to check if the midi val has changed
  -99, -99, -99, -99, -99, -99, -99, -99,    //plexer1 (because of mapping it might be the same even though rawVal has wandered)
  -99, -99, -99, -99, -99, -99, -99, -99,   //plexer2
  -99, -99, -99, -99, -99, -99, -99, -99,  //plexer3
  -99, -99, -99, -99, -99, -99, -99, -99, //plexer4
};


//Settings for plexers
byte DATA1[32] = {                        // CC number / note number
  1, 2, 3, 4, 40, 41, 42, 46,                //plexer1  eeprom addr 0-7
  9, 10, 11, 12, 39, 38, 36, 37,        //plexer2   eeprom addr 40 - 47  last four are arcade buttons
  17, 18, 19, 20, 21, 22, 23, 24,      //plexer3    eeprom addr 80-87
  25, 26, 27, 28, 100, 101, 102, 103      //plexer4     eeprom addr 120-127   last four are used as envelope CCs
};

byte isNote [32] = {              //note or CC
  0, 0, 0, 0, 1, 1, 1, 1,        //plexer1          eeprom addr 8-15
  0, 0, 0, 0, 1, 1, 1, 1,       //plexer2           eeprom addr 48-55
  0, 0, 0, 0, 0, 0, 0, 0,      //plexer3            eeprom addr 88-95
  0, 0, 0, 0, 0, 0, 0, 0      //plexer4             eeprom addr 128-135
};

byte noteIsOn [32] = {          //DOESN'T NEED STORING IN EEPROM
  0, 0, 0, 0, 0, 0, 0, 0,      //plexer1
  0, 0, 0, 0, 0, 0, 0, 0,     //plexer2
  0, 0, 0, 0, 0, 0, 0, 0,    //plexer3
  0, 0, 0, 0, 0, 0, 0, 0    //plexer4
};

byte midiCh[32] = {            //midi channel
  1, 1, 1, 1, 1, 1, 1, 1,     //plexer1             eeprom addr 16-23
  1, 1, 1, 1, 1, 1, 1, 1,    //plexer2              eeprom addr 56-63
  1, 1, 1, 1, 1, 1, 1, 1,   //plexer3               eeprom addr 96-103
  1, 1, 1, 1, 1, 1, 1, 1   //plexer4                eeprom addr 136-143
};

byte midiCHANNEL = 0;      //placeholder for midichannel manipulation by shift switch

byte thresh [32] = { //noteOn threshold
  64, 64, 64, 64, 64, 64, 64, 64,    //plexer1        eeprom  addr 24 -31
  64, 64, 64, 64, 64, 64, 64, 64,   //plexer2         eeprom  addr 64 -71
  64, 64, 64, 64, 64, 64, 64, 64,  //plexer3          eeprom  addr 104 -111
  64, 64, 64, 64, 64, 64, 64, 64  //plexer4           eeprom  addr 144 -151
};

byte DATA2[32] = { //velocity for noteOns
  127, 127, 127, 127, 127, 127, 127, 127,    //plexer1 eeprom addr 32 - 39
  127, 127, 127, 127, 127, 127, 127, 127,   //plexer2  eeprom addr 72 - 79
  127, 127, 127, 127, 127, 127, 127, 127,  //plexer3   eeprom addr 112 - 119
  127, 127, 127, 127, 127, 127, 127, 127  //plexer4    eeprom addr 152 - 159
};

//MAPPING
byte inMin[32] = {
  0, 0, 0, 0, 0, 0, 0, 0, //plexer1 eeprom addr 160 - 167
  0, 0, 0, 0, 63, 63, 63, 63, //plexer2 eeprom addr 192 - 199 LAST FOUT ARE ARCADE BUTTONS
  0, 0, 0, 0, 0, 0, 0, 0, //plexer3 eeprom addr 224 - 231
  0, 0, 0, 0, 0, 0, 0, 0  //plexer4 eeprom addr 256 - 263
};

byte inMax[32] = {
  127, 127, 127, 127, 127, 127, 127, 127, //plexer1 eeprom addr 168 - 175
  127, 127, 127, 127, 64, 64, 64, 64, //plexer2 eeprom addr 200 - 207  LAST FOUR ARE ARCADE BUTTONS
  127, 127, 127, 127, 127, 127, 127, 127, //plexer3 eeprom addr 232 - 239
  127, 127, 127, 127, 127, 127, 127, 127  //plexer4 eeprom addr 264 - 271
};

byte outMin[32] = {
  0, 0, 0, 0, 0, 0, 0, 0, //plexer1 eeprom addr 176 - 183
  0, 0, 0, 0, 0, 0, 0, 0, //plexer2 eeprom addr 208 - 215
  0, 0, 0, 0, 0, 0, 0, 0, //plexer3 eeprom addr 240 - 247
  0, 0, 0, 0, 0, 0, 0, 0, //plexer4 eeprom addr 272 - 279
};

byte outMax[32] = {
  127, 127, 127, 127, 127, 127, 127, 127,  //plexer1 eeprom addr 184 - 191
  127, 127, 127, 127, 127, 127, 127, 127, //plexer2 eeprom addr 216 - 223     /LAST FOUT ARE ARCADE BUTTONS
  127, 127, 127, 127, 127, 127, 127, 127,  //plexer3 eeprom addr 248 - 255
  127, 127, 127, 127, 127, 127, 127, 127   //plexer4 eeprom addr 280 - 287
};

float lfoControls[4] = {64, 0, 64, 0.01};

byte arrayCursor = 0; //to make it easy to access the different plexors in the arrays

byte USBreceivedCC = 0;
byte USBreceivedVal = 0;

//lfo handling
byte cc_lfo = 3; //addr 450
byte ch_lfo = 1; //addr 451
int LedBright = 0;
float lfoState = 0;
float OldLfoState = 0;
float PiInc = 0;
int lfoRawFreq = 100;  //equates to approx lfoFreq of 0.015
float lfoFreq = 0.01;
int lfoDepth = 64;
int lfoOffset = 64;
int lfoInteger = 0;
int OldLfoInteger = 0;
int lfoWave = 0;
boolean SHflag = false;
byte adress = 0;
int slewstate = 0;
int slew = 0;

//Preset / EEPROM Handling
boolean sysExFlag = false;
boolean oldPresetState;
int Preset = 0;
int writeAdress;
byte SEXarray[8];


//multiplexer handling
byte plexerSelect = 0;
unsigned long onTime = 0;

struct MySettings : public midi::DefaultSettings                                 //code to change if running status is disabled
{
  static const bool UseRunningStatus = false;                                     // Messes with my a lot of stuff!
};
//here is the code that is executed if runningstatus is to be disabled
#ifdef DISABLERUNNINGSTATUS
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial1, MIDI, MySettings);            //Start a custom instance on hardware serial port (the only one the atmega328 has), call it MIDI and apply the code in struct MySettings
#else                                                                             // here is the code that runs if we want running status  
MIDI_CREATE_DEFAULT_INSTANCE();                                                   //just create a default instance
#endif



void setup() {
  // Magic sauce to do 10-bit register writing.
  // Write TC4H first also when writing to white OCR4A.
  TC4H = 0x03;
  TCCR4D = 0;
  TCCR4C |= 0x09; // Activate channel D
  OCR4C = 0xFF;
  // Configuration of Timer 4 Registers.
  TCCR4A = 0b10000010;
  TCCR4B = 0b00000001;
  //PLL FREQ REGISTER (CAREFUL NOT TO LOSE USB!)
  PLLFRQ = (PLLFRQ & 0xCF) | 0x30;


  // running with high speed clock (set prescale to 8) This speeds up the clock for the analog read
  // the switches need 0.1uF to ground when running this fast (or do they?)
#ifdef HIGHSPEED                                                    // 111=/128 110=/64 101=/32 100=/16  011=/8 (preferred speed)  010=/4  001=/2 

  //PRESCALER OF 8

  bitClear(ADCSRA, ADPS2) ;
  bitSet(ADCSRA, ADPS1) ;
  bitSet(ADCSRA, ADPS0) ;

#endif



  pinMode(ledPin, OUTPUT);
  pinMode(lfoLedPin, OUTPUT);
  pinMode(lfoOnSwitch , INPUT_PULLUP);
  pinMode(presetSwitch, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);

  pinMode(8, OUTPUT);                                               // PORTB 0
  pinMode(9, OUTPUT);                                               // PORTB 1
  pinMode(10, OUTPUT);                                              // PORTB 2

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);

  MIDI.begin(MIDI_CHANNEL_OMNI);                                    // initialize MIDI baudrate, listen to all channels


  //digitalWrite(out1, HIGH);
#ifdef TURNTHRUOFF
  MIDI.turnThruOff();
#endif

  handleMultiplexers();
  if( (oldRaw[ARCADE1]>100) &&(oldRaw[ARCADE2]>100) &&(oldRaw[ARCADE3]>100) &&(oldRaw[ARCADE4]>100) && SHIFT){
    EEPROMCLEAR = true;                                                                                 //DELETE EVERYTHING IN EEPROM!!!
  }
  if (EEPROMCLEAR) {
digitalWrite(ledPin,HIGH);
    for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.write(i, 0);
    }
    digitalWrite(ledPin,LOW);
  }


  if (EEPROM.read(400) == 123) {                    //check if presets have been (succesfully) written to EEPROM (the last thing Toolman Sysex editor does is write 123 to adress 400)
    PRESET1 = true;                                 //use the fucking presets
  } else {
    PRESET1 = false;                                //ignore the meaningless gobbledygook that might be in EEPROM
  }

  if (EEPROM.read(401) == 123) {                    //check if presets have been (succesfully) written to EEPROM (the last thing Toolman Sysex editor does is write 123 to adress 400)
    PRESET2 = true;                                 //use the fucking presets
  } else {
    PRESET2 = false;                                //ignore the meaningless gobbledygook that might be in EEPROM
  }

  if ( (PRESET1 && (digitalReadFast(presetSwitch) == LOW)) || (PRESET2 && (digitalReadFast(presetSwitch) == HIGH)) ) {                                                    // Are we bothered with Presets today? set PRESETS to true further up
    //pinMode(presetSwitch, INPUT);                                      // this is a switch
    if (digitalReadFast(presetSwitch) == HIGH) {
      Preset = 500;                                                   //basically preset2 is just an offset of 500 for the EEPROM adresses
    } else {
      Preset = 0;
    }
    handlePresets();                                                //read presets from EEPROM
  }
#ifdef DEBUG
  Serial.begin(115200);
#endif
  digitalWrite(ledPin, HIGH);
  //delay(5000);
  handleMultiplexers();
  //// Serial.println("CALIB");
  //// Serial.println(oldMIDIVal[SHIFTBUTTONNO]);
  //// Serial.println(oldMIDIVal[MAPBUTTONNO]);
  digitalWrite(ledPin, LOW);
  if ( oldMIDIVal[8] > 64 && oldMIDIVal[9] > 64) {
    CalibrationMode = true;
    CALIBRATE = 1024;
    while (CALIBRATE > 0) {
      CALIBRATE--;
      digitalWriteFast(ledPin, CALIBRATE % 20);
      cvOUT(CALIBRATE);
      delay(3);
    }

  }


  //delay(5000);
  if (EEPROM.read(1000) == 123) {                                              //if we find our magic value then we are calibrated
    CALIBRATED = true;
    digitalWrite(ledPin, HIGH);
    delay(50);

#ifdef DEBUG
    for (int i = 0; i < 47; i++) {                                            //DEBUG CODE
      //// Serial.println(noteArray[i]);                                           //CHECK NOTE ARRAY
    }
#endif

    readPitchesEEPROM();                                                      //load up the stored pwm values from the EEPROM

#ifdef DEBUG
    for (int i = 0; i < 47; i++) {
      //// Serial.println(noteArray[i]);
    }
#endif

    digitalWrite(ledPin, LOW);
  } else {
    //// Serial.println("NO STORED VALUES, USING DEFAULT");
  }
}
