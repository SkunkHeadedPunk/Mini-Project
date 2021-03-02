// Logan Meyer      EENG 350      2 25 2021
// This program contains an integration of the code for section 4.3 (spinning the motor) and 
// 4.6 (reading the rotary encoder and converting to radians), as well as the control 
// systems. The following are the descriptions of each section of the program. 
// The program is roughly divided based on horizontal lines in the code. 

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

// The control system shown here uses a PI controller in order to control and adjust the position of the
// wheel based on the inputs of the desired angular position from the Raspberry pi and the current
// position from the localization modules. 
// 

//_________________________________________________________________________________________________ 
#include <Encoder.h>
#include <Wire.h>
#define countsPerRotation 3200    // Per the datasheet for the motor

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 5);
//   avoid using pins with LEDs attached

float angularPosition;
float radPerCount; 
int sign = 1;


#define motorVoltagePin9  9
#define motorVoltagePin10 10 
#define voltageSignPin7   7   // Corresponds to direction for motor 1
#define voltageSignPin8   8   // Corresponds to direction for motor 2
#define pinD2             4   
#define ADDRESS           0x04    

int aWrite9;   // Variables for the values that are in the analogWrite command for pins 9,10
int aWrite10;
int desiredPulseWidth9  = 800; // Desired pulse width for pin 9, in micro seconds 
int desiredPulseWidth10 = 800; // Desired pulse width for pin 10, in micro seconds

//

/////////////////////
int incomingQuad = 0;
float locationtogo = 0; //fixme to call location go to pi/2 now

float Kp = 0.64;  // Unitless quanitity (proportional)
float Ki = 0.001; // Unit: Volts/ Second
//int Kd = 0; // not needed but may
//float Step = 7;
int delayValue = 30;
//// timing ang control
int I = 0;
int e_past = 0;
int Ts = 0;
int Tc = millis(); 
//below here is location variables
float r = locationtogo; //FIXME                                                         this is the location we want it to go to .      boom
float y = angularPosition;





void setup() {
  
  // define callbacks for i2c communication
  Wire.begin(ADDRESS);
  Wire.onReceive(receiveData);
  
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
  if(locationtogo > y){
    sign = 1;
  }
  if(locationtogo < y){
    sign = 0;
  }
  locationtogo = 2*(3.14159/2); //*incomingQuad * ;
  static float e;
  static float D;


  aWrite9  =  desiredPulseWidth9  / 8;              // Convert value written to analogWrite to pulse width
  aWrite10 =  desiredPulseWidth10 / 8; 
  static float u = aWrite9;
 
  if (aWrite9 > 255) { 
    aWrite9 = 255;                                  // makes sure that aWrite9 and 10 are between 0-255
  } 

  if (aWrite10 > 255) { 
    aWrite10 = 255;
  }

  aWrite9 = aWrite9*u;
  
  analogWrite(motorVoltagePin9,aWrite9);            // Use analogWrite to supply PWM signal of desired width  
  analogWrite(motorVoltagePin10,aWrite10);                                                  
  digitalWrite(voltageSignPin7, sign);                 // Use pins 7/8 to set the sign of the voltage for each motor, 1 = CW, 0 = CCW
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
   


 //eror determining part of code
 r = locationtogo;
 y = angularPosition;
 e = r-y;
//////
 
//checks for error
 if (Ts>0){
  D = (e - e_past)/Ts;
  e_past = e;
 }
 else{
  D = 0;  //maybe we need to pop a kd value in there if its not working well so here it is
 }

//now calculate I
I = I + Ts*e;

//now get controller output
u = Kp*e + Ki*I;
if((((locationtogo-y))<0.2) || ((y-locationtogo)>0.2)){
  u = u - Ki*I;
}
//Serial.println(u);
//aWrite9 = aWrite9*u;

//Serial.println(u);
Ts = millis()-Tc; //FIXME
Tc = millis(); //FIXME
delay(delayValue - Ts);                                           
}

//Callback for received data
void receiveData(int byteCount) {
  while (Wire.available()) {
    incomingQuad = Wire.read();
  }
}
