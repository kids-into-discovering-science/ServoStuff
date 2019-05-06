// Core of the Servo program.
// Develop so:
// Do incremental movement, both directions.  DONE
// Add reporting back of position, Pot value.  DONE
// Add reporting back of LimSwitches.  DONE
// Prevent overrunning LimSwitches. DONE
// Do multiple incremental movement, both directions. DONE
// Run on Teensy-LC

// "1 increment" is to be easily, poorly, defined.  It will be experimented with and possibly used as 
// the basis for turning longer amounts.
// To start with, 50 ms of 25% pulse width.

// Takes commands over SerialMon:
//     C or c : Clockwise 1 increment
//     W or w : Counterclockwise 1 increment
//     S or s or 0 : Stop (don't need this, but hey)
//     = : Increase speed 10%  
//     - : Decrease speed 10%


//     2 : Go 2 increments in the last specified direction
//     3 : etc. thru 9
//     1 : Go 1 increment in the last specified direction


//After every command, replies over SerialMon:
//     Vlotage, vlot, after move (later to be translated to angle) and change in voltage from before move
//     Switch positions (CWlim, CCWlim, either false (no hit) or true (LIMIT HIT))

// ____________________________________________________________________

int dir1PinA = 2;  // Motor stuff.  Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.
int dir2PinA = 3;  
int speedPinA = 4; // Needs to be a PWM pin to be able to control motor speed

int speed = 55; // 55% duty cycle
int oneIncr = 100; // one increment is 100 ms 

int incs = 1; // Number of incremental movements to make

int potWiper = A6; // Potentiometer
int vlot = 0;  // That is, "volt", but not quite as it might be someone's reserved word.
               // With oneIcr = 100 ms and speed = 55:  
               // Fully W vlot is ~706, 699 -> 713.
               // Fully C is ~278, 268 -> 288.
               // So range is ~428 units of vlot.  
               // Range is a little less than 180 deg, call it 160 deg.
               // So 428/160 ~=2.5 units per degree.
               // Travels full range with about 150 Ws or Cs.  
               // So ~1 unit per degree.

const int numReadings = 5; // Will average 5 readings to smooth noise
int potReads[numReadings]; // vlot will be the average of these
int readIndex = 0;

int pause = 0; // Pause (nonzero) or don't pause (0) regular updates
int updateRate = 1000; // If no SerialMon input, update at this rate (ms)

int CWpin = 15;  // limit switches
int CCWpin = 16; 

boolean CWlim = false; // Switches
boolean CCWlim = false;

// ____________________________________________________________________

void setup()
{ 
  Serial.begin(9600);  // initialize serial communication @ 9600 baud:
                       // Ahah!  In the IDE need to select Tools | Port | whichever looks right.

  pinMode(dir1PinA, OUTPUT); //Define L298N Dual H-Bridge Motor Controller Pins
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);

  analogWrite(speedPinA, 0);//Sets inital speed variable via PWM to zero (stopped)

  vlot = analogRead(potWiper);                // Start with correct values
  for (int i = 0; i < numReadings-1; i++)
    { vlot = vlot + analogRead(potWiper);
    }
  vlot = vlot/5;

  digitalWrite(dir1PinA, LOW); // Set direction to CW
  digitalWrite(dir2PinA, HIGH);
  Serial.print("vlot = "); // Starting position is here
  Serial.println(vlot);

  pinMode(CWpin, INPUT_PULLUP);  // Set up and initialize the limit switches
  pinMode(CCWpin, INPUT_PULLUP);
  if (digitalRead(CWpin) == LOW) 
    {CWlim == true;}
  else {CWlim  == false;}; 
  if (digitalRead(CCWpin) == LOW) 
    {CCWlim == true;}
  else {CCWlim  == false;}
}
 
void loop()
{
  if (Serial.available() > 0)
  { int inByte = Serial.read();  //Wait for a byte
  
    CWlim = digitalRead(CWpin);
    CCWlim = digitalRead(CCWpin);
    
    if (inByte == 83 or inByte == 115 or inByte == 48) //  "S" or "s" or zero--Motor  Stop (Freespin)
    { analogWrite(speedPinA, 0);
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, LOW);
      Serial.println("Motor Stop");
    }

    else if (inByte == 43 or inByte == 61) // "+"or "="--Increase speed
    { Serial.print("Increase speed from ");
    
      Serial.print(speed);
      Serial.print(" to ");
      speed = speed * 1.1;
      Serial.println(speed);
    }

    else if ((inByte == 67 or inByte == 99)) // If not CW limitted, and "C" or "c"--Increment Clockwise
    { if (CWlim)
      { digitalWrite(dir1PinA, LOW);    // Set to Clockwise
        digitalWrite(dir2PinA, HIGH);
        Serial.print("Motor CW.  Pot voltage changes from "); // Prints on the serial monitor
        vlot = analogRead(potWiper); 
        for (int i = 0; i < numReadings-1; i++)
         { vlot = vlot + analogRead(potWiper);
         }
        vlot = vlot/5;
        Serial.print(vlot);
        Serial.print(" V to ");
        analogWrite(speedPinA, speed);  // Go one increment
        delay(oneIncr);
        analogWrite(speedPinA, 0); 
        vlot = analogRead(potWiper); 
        for (int i = 0; i < numReadings-1; i++)
         { vlot = vlot + analogRead(potWiper);
         }
        vlot = vlot/5;
        Serial.print(vlot);
        Serial.println("V.   "); 
      }
    else
      {  Serial.println("CW limit hit!!  Back up!");
      }  
    }

    else if ((inByte == 87 or inByte == 119)) // If not CW limitted, and "W" or "w"--Increment Counterclockwise
    {  if (CCWlim)
      { digitalWrite(dir1PinA, HIGH);
        digitalWrite(dir2PinA, LOW);
        Serial.print("Motor CCW.  Pot voltage changes from "); // Prints on the serial monitor
        vlot = analogRead(potWiper); 
        for (int i = 0; i < numReadings-1; i++)
         { vlot = vlot + analogRead(potWiper);
         }
        vlot = vlot/5;
        Serial.print(vlot);
        Serial.print(" V to ");
        analogWrite(speedPinA, speed);  // Go one increment
        delay(oneIncr);
        analogWrite(speedPinA, 0); 
        vlot = analogRead(potWiper); 
        for (int i = 0; i < numReadings-1; i++)
        
         { vlot = vlot + analogRead(potWiper);
         }
        vlot = vlot/5;
        Serial.print(vlot);
        Serial.println("V.   "); 
      }
      else
      { Serial.println("CCW limit hit!!  Back up!");
      }
    }

    else if (inByte == 43 or inByte == 61) // "+"or "="--Increase speed
    { Serial.print("Increase speed from ");
      Serial.print(speed);
      Serial.print(" to ");
      speed = speed * 1.1;
      Serial.println(speed);
    }

    else if (inByte == 45 ) // "-" Decrease speed
    { Serial.print("Decrease speed from ");
      Serial.print(speed);
      Serial.print(" to ");
      speed = speed * 0.9;
      Serial.println(speed);
    }

    else if (inByte > 48 && inByte < 58 ) // if it's a number 1-9 keep going in same direction
    { if (CWlim && CCWlim)
      { incs = inByte - 48;
        Serial.print(incs);
        Serial.print(" increments in same direction.  Pot voltage changes from "); // Prints on the serial monitor
        vlot = analogRead(potWiper); 
        for (int i = 0; i < numReadings-1; i++)
         { vlot = vlot + analogRead(potWiper);
         }
        vlot = vlot/5;
        Serial.print(vlot);
        Serial.print(" V to ");
        analogWrite(speedPinA, speed);  // Go one increment
        delay(incs * oneIncr);
        analogWrite(speedPinA, 0); 
        vlot = analogRead(potWiper); 
        for (int i = 0; i < numReadings-1; i++)
        
         { vlot = vlot + analogRead(potWiper);
         }
        vlot = vlot/5;
        Serial.print(vlot);
        Serial.println("V.   "); 
      }
      else
      { Serial.print("Can't move, limit hit");  
        Serial.print(CWlim);
        Serial.println(CCWlim);
      }
    }


    else if (inByte == 10) // LF; do nothing at all, including report back to serial port
    {
    }

    else
    { Serial.print("Don't understand '");
      Serial.print (inByte);
      Serial.println("' so doing nothing.");
    }
  }
  else
  {   
      vlot = analogRead(potWiper); 
      for (int i = 0; i < numReadings-1; i++)
       { vlot = vlot + analogRead(potWiper);
       }
      vlot = vlot/5;
      Serial.print("Pot position:  ");
      Serial.print(vlot);
      CWlim = digitalRead(CWpin);
      CCWlim = digitalRead(CCWpin);
      Serial.print(", CW/CCW limits are ");
      Serial.print(CWlim);
      Serial.println(CCWlim);
      
      delay(updateRate);
  }
}
