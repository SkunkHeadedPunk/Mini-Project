# Mini-Project
Seed Lab Mini Project - Group 007

_Purpose of Repository_
This repository contains programs which detect the quadrant of an Aruco marker appearing on a PiCamera stream, and turns a wheel accordingly.
The geometrical quadrants correspond to the following angles on the wheel:
I: 90 degrees
II: 180 degrees
III: 270 degrees
IV: 360 (or 0) degrees



_File Organization_
Files are organized by task point numbers as defined in MiniProject.handout.pdf, and named with a short description of their purpose.

4.2_ArucoDetection: Python SHIFT file implementing Aruco marker (and it's respective quadrant) location, also includes (but does not implement) functions to write/read to/from Arduino.

4.3_Spin_Motors.ino: Arduino program for spinning motors using the Pololu motor driver shield and PWM signals. 

4.4_readRotaryEncoderRads.ino: Arduino program for reading the rotary encoder's angular position, using the library from the .zip folder.  

4.3_4.4_CombinedSpinAndRead: Arduino program integrating programs 4.3 and 4.4, used to spin the motor and read its current angular position to the serial monitor.
 
4.6 ControllerMatlab.html. This is the matlab file that contains the simulated PI controller 

4.7 sketch_feb25b.ino This is the final code to run the arduino with the encoader PID controller and motor controller

Encoder-1.4.1.zip: Arduino Library containing an accurate way to read encoders. Including this library is necessary to properly read the rotary   encoders using the Arduino.   

Arduino Motor Driver Shield Datasheet: Datasheet and user guide for the Pololu Dual MC33926 Motor Driver Shield used to control motors using the Arduino.

FinalArduinoCode.ino: Final implementation of the motor control. Takes a quadrant value from the Pi and moves the motor to the desired location.

FinalPiCode.py: Final implementation of Pi code. Uses computer vision techniques to determine a quadrant location of the Aruco marker, then sends the quadrant value to the Arduino.

marker_0.png: The Aruco marker detected by the computer vision module to allow the user to tell the wheel which position to spin to. 
