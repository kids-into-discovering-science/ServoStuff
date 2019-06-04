// Core of the Servo program.
// Develop so:
// Do incremental movement, both directions.
// Add reporting back of position, Pot value.
// Add reporting back of Lim Switches

// Run on Teensy-LC

// "1 increment" is to be easily, poorly, defined.  It will be experimented with and possibly used as 
// the basis for turning longer amounts.
// To start with, 50 ms of 25% pulse width.

// Takes commands over SerialMon:
//     C or c == Clockwise 1 increment
//     W or w == Counterclockwise 1 increment
//     S or s == Stop
//     = == Increase speed 10%
//     - == Decrease speed 10%

//     2 == Go 2 increments in the last specified direction
//     3 == etc.

//After every command, replies over SerialMon:
//     Vlotage, vlot, after move (later to be translated to angle) and change in voltage from before move
//     Switch positions (CWlim, CCWlim, either false (no hit) or true (LIMIT HIT))

// ____________________________________________________________________

// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Motor 
int dir1PinA = 1;
int dir2PinA = 2;
int speedPinA = 3; // Needs to be a PWM pin to be able to control motor speed

int speed = 55; // 55% duty cycle
int oneIncr = 100; // one increment is 100 ms 

// Potentiometer
int potWiper = A9; //b n
int vlot = 0;  // That is, "volt", but not quite as it might be someone's reserved word.
               // With oneIcr = 100 ms and speed = 55:  
               // Fully W vlot is ~706, 699 -> 713.
               // Fully C is ~278, 268 -> 288.
               // So range is ~428 units of vlot.  
               // Range is a little less than 180 deg, call it 160 deg.
               // So 428/160 ~=2.5 units per degree.
               // Travels full range with about 150 Ws or Cs.  
               // So ~1 unit per degree.

// Switches
// boolean CWlim = false;
// boolean CCWlim = false;

// ____________________________________________________________________

void setup()
{ 
  Serial.begin(9600);  // initialize serial communication @ 9600 baud:
                       // Ahah!  In the IDE need to select Tools | Port | whichever looks right.

  pinMode(dir1PinA, OUTPUT); //Define L298N Dual H-Bridge Motor Controller Pins
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);

  analogWrite(speedPinA, 0);//Sets inital speed variable via PWM to zero (stopped)

  vlot = analogRead(potWiper); // Start with correct values
  digitalWrite(dir1PinA, LOW); // Set direction to 
  digitalWrite(dir2PinA, HIGH);
  Serial.print("vlot = "); // Starting position is here.
  Serial.println(vlot);
}

void loop()
{
  if (Serial.available() > 0)
  { int inByte = Serial.read();  // Initialize the Serial interface:
    if (inByte != 10)
    { Serial.print("Byte is ");
      Serial.println(inByte);
    }

    if (inByte == 67 or inByte == 99) // "C" or "c"--Increment Clockwise
    { digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
      Serial.print("Motor CW.  voltage changes from "); // Prints on the serial monitor
      vlot = analogRead(potWiper);
      Serial.print(vlot);
      Serial.print("V changing to ");
      analogWrite(speedPinA, speed);
      delay(oneIncr);
      analogWrite(speedPinA, 0);
      vlot = analogRead(potWiper);
      Serial.print(vlot);
      Serial.println("V.   "); // Creates a blank line printed on the serial monitor2
    }

    if (inByte == 87 or inByte == 119) // "W" or "w"--Increment Counterclockwise
    { analogWrite(speedPinA, speed);
      digitalWrite(dir1PinA, HIGH);
      digitalWrite(dir2PinA, LOW);
      Serial.println("Motor CCW"); 
      Serial.println("   "); 
      analogWrite(speedPinA, speed);
      delay(oneIncr);
      analogWrite(speedPinA, 0);
    }

    else if (inByte == 50 or inByte == 83 or inByte == 115 or inByte == 48) // "2" or "S" or "s" or zero--Motor  Stop (Freespin)
    { analogWrite(speedPinA, 0);
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, LOW);
      Serial.println("Motor Stop");
      Serial.println("   ");
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

    else if (inByte == 10) // LF; do nothing at all, including report back to serial port
    {
    }

    else
    { Serial.println("Default case; do nothing.");
      Serial.println("  ");

    }
  }
  else
  {   vlot = analogRead(potWiper);
      Serial.println(vlot);
      delay(1000);
  }
}
