//////////////////////////////////////////////////////////////
//
// Name: scoreBoard.pde
// Author: Ian Nice - ian@woscc.org.uk
// Version: 0.1 - 14/03/2013 - First version for testing
//          0.2 - 01/04/2013 - Updated to produce additional debug
//          0.3 - 06/04/2013 - Updated to simplify wiring and fix bug
//          0.4 - 07/04/2013 - Updated to use strings instead of integers
//          0.5 - 11/04/2013 - Changed to simplify wiring setup, now that i dont have limitation of Integer size.
//          0.6 - 02/07/2013 - Updated because ted doesnt like preceeding zeros :-)
//          0.7 - 20/02/2016 - Updated to work with Arduino IDE 1.6.7 - Thanks to James W @ Potton Town CC for helping debug the problems
//          0.8 - 20/01/2017 - Updated to reflect Baseball & Softball Scoring. 
//
// Acknowledgement:
//  shifter.h - http://www.proto-pic.com
//  CmdMessenger.h - https://github.com/dreamcat4/cmdmessenger
//  Streaming.h - http://arduiniana.org/libraries/streaming/
//  Base64.h - https://github.com/adamvr/arduino-base64
//  Where the idea began.......
//  http://www.fritz-hut.com/arduinopi-web-based-controller-for-arduino/
//
//////////////////////////////////////////////////////////////

// Command overview
// 4,ScoreVisitor,Innings,ScoreHome,Balls1,Balls2,Strikes,Outs# //display a score on the board
// 5# //test mode

// Wiring layout
// shifter set 1, pins 22 (SRCK), 26 (SERIN),30 (RCK) --> Vistors score, Inning, Home score, 5 characters (e.g. 02305)
// shifter set 2, pins 34 (SRCK), 38 (SERIN), 42 (RCK) --> Balls, Strikes, Outs, 3 characters (e.g. 322)


// Shifter library available from http://www.proto-pic.com/Resources/shifter.zip
#include <ShifterStr.h>

// CmdMessenger library available from https://github.com/dreamcat4/cmdmessenger
#include <CmdMessenger.h>

// Base64 library available from https://github.com/adamvr/arduino-base64
#include <Base64.h>

// Streaming4 library available from http://arduiniana.org/libraries/streaming/
#include <Streaming.h>

// set up the field seperators and command seperators.  
char field_separator = ',';
char command_separator = '#';

// set up the chharacter arrays we will use later
char ScoreVisitor[3] = { '\0' };
char Innings[3] = { '\0' };
char ScoreHome[3] = { '\0' };
char Balls1[2] = { '\0' };
char Balls2[2] = { '\0' };
char Strikes[2] = { '\0' };
char Outs[2] = { '\0' };

// set up the headings 
char ScoreVisitorHeading[] = "Visitor Score:";
char InningsHeading[] = "Inning:";
char ScoreHomeHeading[] = "Score Home Score:";
char Balls1Heading[] = "Balls1:";
char Balls2Heading[] = "Balls2:";
char StrikesHeading[] = "Strikes:";
char OutsHeading[] = "Outs:";
char ScoresTotal[] = "Visitor Score, Inning and Home Score:";
char BallsStrikesandOuts[] = "Balls1, Balls2, Strikes and Outs:";

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// dreamcat4 default
// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// We can define up to a default of 50 cmds total, including both directions (send + recieve)
// and including also the first 4 default command codes for the generic error handling.
// If you run out of message slots, then just increase the value of MAXCALLBACKS in CmdMessenger.h

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised

  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Must not delete this line
};

// set up 2 groups of shifters
Shifter shifterSet1(5, 22, 26, 30);  //Visitor Score, Inning, Home Score + Balls, Strikes and Outs controlled by pins 22,26 and 30
Shifter shifterSet2(4, 34, 38, 42);  //Balls (LED Groups 1 & 2), Strikes and Outs controlled by pins 34,38 and 42


void update_scoreboard()
{
  while ( cmdMessenger.available() )
  {
    
    // lets assume we always get the correctly formatted string
    // its lazy, but should be ok
    
    char buf[5] = { '\0' };
    char tempString[6] = { '\0' }; 
    
    // Visitors Score
    cmdMessenger.copyString(buf, 4);
    strcpy(ScoreVisitor,buf);
    cmdMessenger.sendCmd(kACK, ScoreVisitorHeading);
    cmdMessenger.sendCmd(kACK, ScoreVisitor);
    
    // Innings
    memset(buf, '\0', 4);
    cmdMessenger.copyString(buf, 4);
    strcpy(Innings,buf);
    cmdMessenger.sendCmd(kACK, InningsHeading);
    cmdMessenger.sendCmd(kACK, Innings);
    
    // Home Score
    memset(buf, '\0', 4);
    cmdMessenger.copyString(buf, 4);
    strcpy(ScoreHome,buf);
    cmdMessenger.sendCmd(kACK, ScoreHomeHeading);
    cmdMessenger.sendCmd(kACK, ScoreHome);
       
  
    
    // Balls(1)
    memset(buf, '\0', 3);
    cmdMessenger.copyString(buf, 3);
    strcpy(Balls1,buf);
    cmdMessenger.sendCmd(kACK, Balls1Heading);
    cmdMessenger.sendCmd(kACK, Balls1);
    
    // Balls(2)
    memset(buf, '\0', 3);
    cmdMessenger.copyString(buf, 3);
    strcpy(Balls2,buf);
    cmdMessenger.sendCmd(kACK, Balls2Heading);
    cmdMessenger.sendCmd(kACK, Balls2);
    
    // Strikes
    memset(buf, '\0', 3);
    cmdMessenger.copyString(buf, 3);
    strcpy(Strikes,buf);
    cmdMessenger.sendCmd(kACK, StrikesHeading);
    cmdMessenger.sendCmd(kACK, Strikes);
    
    // Outs
    memset(buf, '\0', 3);
    cmdMessenger.copyString(buf, 3);
    strcpy(Outs,buf);
    cmdMessenger.sendCmd(kACK, OutsHeading);
    cmdMessenger.sendCmd(kACK, Outs);


   //build the string for shifter set 1
    memset(tempString, '\0', 6);
    strcpy(tempString,ScoreVisitor);
    strcat(tempString,Innings);
    strcat(tempString,ScoreHome);
    shifterSet1.display(tempString);



  //build the string for shifter set 2
    memset(tempString, '\0', 6);
    strcpy(tempString,Balls1);
    strcat(tempString,Balls2);
    strcat(tempString,Strikes);
    strcat(tempString,Outs);
    shifterSet2.display(tempString);

    

    
  }
}

void test_mode()
{
    char buf[] = { "Test Mode" };
    cmdMessenger.sendCmd(kACK, buf);
    shifterSet1.display("121212");
    shifterSet2.display("2122");
}

// default dreamcat4 stuff

// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------

// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  update_scoreboard,            // 004
  test_mode,                    // 005
  NULL
};
// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')


// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup()
{
  // Listen on serial connection for messages from the pc
  Serial.begin(57600); // Arduino Uno, Mega, with AT8u2 USB

  //cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);

  arduino_ready();
  
  // Set the displays to 000 on boot
  //shifterSet1.display("000000000");
  //shifterSet2.display("000000");
  shifterSet1.display("-00-0000");
  shifterSet2.display("0000");
}

// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 2000; // 2 seconds
long previousMillis = 0;
int counter = 0;

void timeout()
{
  // blink
  if (counter % 2)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
  counter ++;
}  

void loop()
{
  
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();

  // handle timeout function, if any
  if (  millis() - previousMillis > timeoutInterval )
  {
    timeout();
    previousMillis = millis();
  }
  
}
