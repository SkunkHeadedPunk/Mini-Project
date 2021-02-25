// Thomas Klinedinst      EENG 350      2 24 2021
// This program contains an integration of the code for section 4.3 (spinning the motor) and 
// 4.6 (reading the rotary encoder and converting to radians). The following are the descriptions
// of each section of the program. The program is roughly divided based on horizontal lines in the 
// code. 

// Program 4.3 is designed to spin motors using an Arduino with a Pololu motor driver shield. 
// This program contains a conversion from the pulse width of a PWM signal to the analog value
// written to the PWM signal. 
// To use this program, connect the motor driver shield to the arduino as indicated in the diagram
// on section 3.c of the Pololu Dual MC33926 Motor Driver Shield User's Guide
// 
// Program 4.6 is used to accurately determine the angular position of a motor-driven wheel using 
// rotary encoders. The rotary encoder needs to be connected to the Arduino as follows:
//        + ---> 5V
//        GND -> GND
//        B ---> 5
//        A ---> 2
// The framework for this code was taken from the "Example" folder of the provided library, containing 
// an efficient method for reading encoders. This framework was modified to include calculations for 
// determining the angular position of the wheel based on counts as well as resetting the angular 
// position. 
// 

//_________________________________________________________________________________________________ 
#include <Encoder.h>
#define countsPerRotation 3200    // Per the datasheet for the motor

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 5);
//   avoid using pins with LEDs attached

float angularPosition;
float radPerCount; 

#define motorVoltagePin9  9
#define motorVoltagePin10 10 
#define voltageSignPin7   7   // Corresponds to direction for motor 1
#define voltageSignPin8   8   // Corresponds to direction for motor 2
#define pinD2             4

int aWrite9;   // Variables for the values that are in the analogWrite command for pins 9,10
int aWrite10;
int desiredPulseWidth9  = 400; // Desired pulse width for pin 9, in micro seconds 
int desiredPulseWidth10 = 400; // Desired pulse width for pin 10, in micro seconds

void setup() {
  // put your setup code here, to run once:
  pinMode (pinD2, OUTPUT);
  pinMode (voltageSignPin7, OUTPUT);
  pinMode (voltageSignPin8, OUTPUT);
  pinMode (motorVoltagePin9, OUTPUT);
  pinMode (motorVoltagePin10, OUTPUT);

  digitalWrite(pinD2, HIGH);    // Enables connection to motor

//_________________________________________________________________________________
    Serial.begin(9600);
  Serial.println("Encoder Angular Position:");

  Serial.println("Press r to reset the angular position");

  // Determining the scaling to convert position to angular position in radians
  // 80 counts per rotation = 80 counts / 2pi rad
  radPerCount = (float) 2 * (PI) / (float) countsPerRotation; 
  
}
//________________________________________________________________________
long oldPosition  = -999;   // Included in the provided Basic example code
//________________________________________________________________________
void loop() {

  aWrite9  =  desiredPulseWidth9  / 8;              // Convert value written to analogWrite to pulse width
  aWrite10 =  desiredPulseWidth10 / 8; 

  if (aWrite9 > 255) { 
    aWrite9 = 255;                                  // makes sure that aWrite9 and 10 are between 0-255
  } 

  if (aWrite10 > 255) { 
    aWrite10 = 255;
  }
  
  analogWrite(motorVoltagePin9,aWrite9);            // Use analogWrite to supply PWM signal of desired width  
  analogWrite(motorVoltagePin10,aWrite10);                                                  
  digitalWrite(voltageSignPin7, 1);                 // Use pins 7/8 to set the sign of the voltage for each motor, 1 = CW, 0 = CCW
  digitalWrite(voltageSignPin8, 0); 

  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {   
    if (oldPosition != -999) {    // Makes sure angularPosition isn't calculated using the initial value for oldPosition
      angularPosition = angularPosition + (newPosition - oldPosition) * radPerCount; // Calculated based on position diff. 
    }                                                                                // to allow for easy reset w/o resetting
    oldPosition = newPosition;                                                       // position variables. 
    //Serial.println(newPosition);  
    Serial.println(angularPosition);
  }   
  
  // Mechanism for resetting the count to zero
  if (Serial.read() == 'r') {   
    angularPosition = 0; 
    Serial.println("Reset angular position");
  }
                                                    
}
