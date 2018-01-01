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
SoftwareSerial softserial(RxLCD, TxLCD);

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





/*************************************************************/
void setup()
{
  pinMode(1, OUTPUT);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(TxLCD, OUTPUT);

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
  softserial.print(" DMX Console Master ");
  UCSR0A &= (~0x02); // Cancelling Doubling Bit

  if ( UCSR0A & 0x02)
    softserial.print("Doubling ON");
  else
    softserial.print("NO Doubling");
  
  
  delay (500);

  //  To Change speed to 250Kb and 2 stop bit\
  UBRR0H = (unsigned char)(myubrr>>8); //High part, only bit 0-3
  UBRR0L = (unsigned char)myubrr; //Low part
  UCSR0C |= ((3<<UCSZ00)|(1<<USBS0)); //N81 No parity/8 bits/2 Stop bit
  }






/*************************************************************/
void loop()
{
  if (full_dmx_channel == 0) //Make a break in serial
  {
    UCSR0B = UCSR0B & ~0x08; // Disable USART Transmit
    //delayMicroseconds(120);
    pinMode(1, OUTPUT);
    //digitalWrite(1,HIGH);
    //delayMicroseconds(120);
    digitalWrite(1,LOW); // Low = Break
    delayMicroseconds(120);
    digitalWrite(1,HIGH);
    UCSR0B = UCSR0B | 0x08; // Enable USART Transmit
    Serial.write(0x00);
   }
  if (full_dmx_channel>0) //Not a break then write the DMX Channel
  //if (full_dmx_channel == 1) //Not a break then write the DMX Channel
    {
    Serial.write(readPot1/4);
    //Serial.write(0x0F);
    //Serial.write(0x00);
    //Serial.write(0xFF);
    }
  full_dmx_channel++;
  if (full_dmx_channel>5)
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
