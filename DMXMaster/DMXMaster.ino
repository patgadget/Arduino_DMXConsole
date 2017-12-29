// My DMX Console Have a Duemilanove (set Arduino before Transfert)
#include <SoftwareSerial.h>

// DMX Console
// Analogue (5) Joystick X
// Analogue (4) Joystick Y
// Analogue (3) Pot #4
// Analogue (2) Pot #3
// Analogue (1) Pot #2
// Analogue (0) Pot #1


#define RxLCD 6
#define TxLCD 5
#define ledPin 13                // choose the pin for the LED
#define clockPin 7              //clock for serial to parallel to serial
#define latchPin 9              // after serial light latch them out
#define dataPin 8                // for LED
#define KeyOK 10
#define KeyESC 10
#define EncCHA 2                // choose the input pin (for a pushbutton)
#define EncCHB 3                // choose the input pin (for a pushbutton)
#define loadPin 4
#define sInDataPin 6
SoftwareSerial softserial(RxLCD, TxLCD);



//Define the LED value
#define TOP_LEFT_RED 0 //0x8000
#define TOP_LEFT_GREEN 1 //0x4000
#define TOP_RIGHT_RED 2 //0x2000
#define TOP_RIGHT_GREEN 3 //0x1000
#define BOT_LEFT_RED 4 //0x0800
#define BOT_LEFT_GREEN 5 //0x0400
#define BOT_RIGHT_RED 6 //0x0200
#define BOT_RIGHT_GREEN 7 //0x0100
#define ESC_RED 8 //0x0080
#define ESC_GREEN 9 //0x0040
#define OK_RED 10  //0x0020
#define OK_GREEN 11  //0x0010

//Define the input value
#define BOT_RIGHT 0
#define BOT_LEFT 1
#define TOP_RIGHT 2
#define TOP_LEFT 3
#define OK 6
#define ESC 7


#define myubrr (16000000L/16/250000-1)


int tempStateIn=0;
int lastState = 0;              //last state of led
int EncValue = 0;
int EncPulse = 3;             //0 = Nothing, 1= Forward, 2= reverse, 3= redraw
int EncDivider = 10;
int EncDividerCountUP = 0;
int EncDividerCountDW = 0;
int TopScreen =0;
int OldTopScreen=0;
int MenuIndex=1;
int DMXchannel=1;
int DMXintensity=1;
int FixtureNO=1;
int SequenceNO;
int SceneNO=1;
int ProgramNO=0;
int delayEntreSeq=500;
int contDelay;
int sequence1[15] = {5,6,7,1,7,2,10,3,7,3,10,5,6,0};
int indexSeq=0;
int full_dmx_channel=0;
int ModeNo=0; // 1 = DMX channel debug, 2= Fixture Mode, 3= Scene Mode, 4= Program Mode, 5 = ALL ON, 6 = ALL Off, 7 = Sequence mode
unsigned char randomDmx1Value=0;
unsigned char randomDmx2Value=0;
char randomDmx3Value=0;
int tempProgValue1;
int tempProgValue2;
int tempProgValue3;
long tempProgValueLong;
unsigned char dmx_data[50];
int readPot1;
int readPot2;
int readPot3;
int readPot4;
int readPot5;
int readPot6;

int Choix;
int TapAvg[10];
int ReadTimeTap;
int TapSyncTime;
int LastTapSyncTime;

int pinto=0;




/*************************************************************/
void setup()
{
  // Config the LED output for the serial to paralel chip
  pinMode(clockPin, OUTPUT);      
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin,OUTPUT);
  // Config the serial input switch
  pinMode(loadPin, OUTPUT);
  pinMode(sInDataPin,INPUT);

  //Config of the rotary encoder
  pinMode(EncCHA, INPUT);
  pinMode(EncCHB, INPUT);
  pinMode(1, OUTPUT);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(TxLCD, OUTPUT);
  pinMode(KeyOK, INPUT);
  pinMode(KeyESC, INPUT);
  digitalWrite(KeyOK, HIGH); // Turn ON the pull up resistor
  digitalWrite(KeyESC, HIGH);

  digitalWrite(TxLCD, HIGH);
  delay (100);
  softserial.begin(9600); // for LCD
  Serial.begin(250000); //For DMX out
  delay (100);
  //noInterrupts();
  softserial.write(22); // turn on LCD, No Cursor
  softserial.write(17); // turn on back light
  delay (50);
  softserial.write(12); // Clear Screen
  softserial.print(" DMX Console Master");
  delay (500);

  //  To Change speed to 250Kb and 2 stop bit\

  UBRR0H = (unsigned char)(myubrr>>8); //High part, only bit 0-3
  UBRR0L = (unsigned char)myubrr; //Low part
  //UCSR0B |= ((1<<RXEN0)|(1<<RXCIE0));//Enable Receiver and Interrupt RX
  UCSR0C |= ((3<<UCSZ00)|(1<<USBS0)); //N81 No parity/8 bits/2 Stop bit
  
  //UBRR0L = 3;
  //UCSR0C = UCSR0C | 0x08;

  randomSeed(analogRead(0));
  }






/*************************************************************/
void loop()
{
  int returnValue;

  if (full_dmx_channel == 0) //Make a break in serial
  {
    UCSR0B = UCSR0B & ~0x08;
    delayMicroseconds(120);
    pinMode(1, OUTPUT);
    digitalWrite(1,LOW);
    delayMicroseconds(12000);
    digitalWrite(1,HIGH);
    delayMicroseconds(12);
    UCSR0B = UCSR0B | 0x08;
    Serial.write(0x00);
   }
  if (full_dmx_channel>0) //Not a break then write the DMX Channel
    {
    Serial.write(0xFF);
    }
  full_dmx_channel++;
  if (full_dmx_channel>50)
    {
    full_dmx_channel = 0;
    readPot1=analogRead(0);
    readPot2=analogRead(1);
    readPot3=analogRead(2);
    readPot4=analogRead(3);
    readPot5=analogRead(4);
    readPot6=analogRead(5);
  }
}


