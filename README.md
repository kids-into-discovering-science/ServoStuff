# ServoStuff
Code to run on the servos' Teensy-LC

Right now it has code to build the eventual full functionality of the Servo.
To start with, we want it to increment CW or CCW in response to SerMon commands.

Next we'll want it to read the resistor and report its value back before and afte the move.
Also add reporting of the limit switch positions.

Later on we'll calibrate resistance to angle.  We should probably hard code it for each Servo.
