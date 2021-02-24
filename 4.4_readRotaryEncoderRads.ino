// Thomas Klinedinst   EENG 350   2 22 2021
// This code is used to accurately determine the angular position of a motor-driven wheel using 
// rotary encoders. The rotary encoder needs to be connected to the Arduino as follows:
//        + ---> 5V
//        GND -> GND
//        CLK -> 5          // CLK corresponds to encoder output B, the clk is specific to one model of encoder
//        DT --> 2          // DT corresponds to encoder output A, the DT is specific to one model of encoder
// The framework for this code was taken from the "Example" folder of the provided library, containing 
// an efficient method for reading encoders. This framework was modified to include calculations for 
// determining the angular position of the wheel based on counts as well as resetting the angular 
// position. 

#include <Encoder.h>
#define countsPerRotation 80

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 5);
//   avoid using pins with LEDs attached

float angularPosition;
float radPerCount;

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Angular Position:");

  Serial.println("Press r to reset the angular position");

  // Determining the scaling to convert position to angular position in radians
  // 80 counts per rotation = 80 counts / 2pi rad
  radPerCount = (float) 2 * (PI) / (float) 80;
  
}

long oldPosition  = -999;   // Included in the provided Basic example code

void loop() {
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
