// My DMX Console Have a Duemilanove (set Arduino before Transfert)
#include <SoftwareSerial.h>

// DMX Console
// Analogue (5) Joystick X
// Analogue (4) Joystick Y
// Analogue (3) Pot #4
// Analogue (2) Pot #3
// Analogue (1) Pot #2
// Analogue (0) Pot #1

#define TxDMX 1
#define RxLCD 6
#define TxLCD 5
SoftwareSerial softserial(RxLCD, TxLCD);

int full_dmx_channel=0;
unsigned char dmx_data[50];
int readPot1;
int readPot2;
int readPot3;
int readPot4;
int readPot5;
int readPot6;


/*************************************************************/
void setup()
{
  // DMX Setup
  pinMode(TxDMX, OUTPUT);
  Serial.begin(250000, SERIAL_8N2); //For DMX out
  
  // LCD Setup
    pinMode(TxLCD, OUTPUT);
  digitalWrite(TxLCD, HIGH);
  delay (100);
  softserial.begin(9600); // for LCD
  delay (100);
  softserial.write(22); // turn on LCD, No Cursor
  softserial.write(17); // turn on back light
  delay (50);
  softserial.write(12); // Clear Screen
  softserial.print(" DMX Console Master ");
  }






/*************************************************************/
void loop()
{
  if (full_dmx_channel == 0) //First then Make a break in serial
  {
    /* To Reset the Sequence in a DMX Protocole, a Break is Send to tell all the
     *  receiver that this is the starting point.
     *  https://en.wikipedia.org/wiki/DMX512#Protocol
     */
    UCSR0B = UCSR0B & ~0x08; // Disable USART Transmit
    //pinMode(TxDMX, OUTPUT);
    digitalWrite(TxDMX,LOW); // Low = Break
    delayMicroseconds(120);
    digitalWrite(1,HIGH); // Back to High before giving control back to USART
    UCSR0B = UCSR0B | 0x08; // Enable USART Transmit
    Serial.write(0x00); // Start Code 0x00 is standart for light
   }
  if (full_dmx_channel>0) //Not a break then write the DMX Channel
    {
    Serial.write(dmx_data[full_dmx_channel]);
    }
  full_dmx_channel++;
  if (full_dmx_channel>7)
    {
    full_dmx_channel = 0;
    readPot1=analogRead(0);
    readPot2=analogRead(1);
    readPot3=analogRead(2);
    readPot4=analogRead(3);
    readPot5=analogRead(4);
    readPot6=analogRead(5);
    dmx_data[1]=readPot1/4;
    dmx_data[2]=readPot2/4;
    dmx_data[3]=readPot3/4;
    dmx_data[4]=readPot4/4;
    dmx_data[5]=readPot5/4;
    dmx_data[6]=readPot6/4;
  }
}
