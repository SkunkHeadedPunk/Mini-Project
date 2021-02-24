// Thomas Klinedinst      EENG 350      2 24 2021
// This program is designed to spin motors using an Arduino with a Pololu motor driver shield. 
// This program contains a conversion from the pulse width of a PWM signal to the analog value
// written to the PWM signal. 
// To use this program, connect the motor driver shield to the arduino as indicated in the diagram
// on section 3.c of the Pololu Dual MC33926 Motor Driver Shield User's Guide


#define motorVoltagePin9  9
#define motorVoltagePin10 10 
#define voltageSignPin7   7   // Corresponds to direction for motor 1
#define voltageSignPin8   8   // Corresponds to direction for motor 2
#define pinD2             4

int aWrite9;   // Variables for the values that are in the analogWrite command for pins 9,10
int aWrite10;
int desiredPulseWidth9  = 250; // Desired pulse width for pin 9, in micro seconds 
int desiredPulseWidth10 = 250; // Desired pulse width for pin 10, in micro seconds

void setup() {
  // put your setup code here, to run once:
  pinMode (pinD2, OUTPUT);
  pinMode (voltageSignPin7, OUTPUT);
  pinMode (voltageSignPin8, OUTPUT);
  pinMode (motorVoltagePin9, OUTPUT);
  pinMode (motorVoltagePin10, OUTPUT);
  
}

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
  digitalWrite(voltageSignPin7, 0);                 // Use pins 7/8 to set the sign of the voltage 
  digitalWrite(voltageSignPin8, 0); 
                                                    
}
