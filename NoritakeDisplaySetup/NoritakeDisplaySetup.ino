//-----------------------------------------------------------------------------
// @file: NoritakeDispalySetup.ino
// @brief: Testing and Setup of Noritake itron VFD
//
// @details: Arduino version
//
// @copyright: CONFIDENTIAL Property of Jess Fisher Copyright 2018
//
// @All rights reserved: Copying or other reproduction of this program except
// for archival purposes is prohibited without the prior written consent of
// Jess Fisher.
//
// Jess Fisher Copyright, Hoquiam, WA, USA
//
// @author:  Jess Fisher
// @date: 07/01/2018
// @version: 1.0
//
//-----------------------------------------------------------------------------
#include <Wire.h> 

#define DO0 2
#define DO1 3
#define DO2 4
#define DO3 5
#define DO4 6
#define DO5 7
#define DO6 8
#define DO7 9
#define RESETPIN  10
#define DBUSY 11
#define DWRITE 12
#define NUM_DPINS 8

unsigned char pins[NUM_DPINS] = {DO0,DO1,DO2,DO3,DO4,DO5,DO6,DO7};
unsigned char HW[20] = {"Recieve > "};
unsigned char x,y;
//-----------------------------------------------------------------------------
void WriteDisplayByte(unsigned char Data)
{
  unsigned char cnt=0, digit=1;

  while(digitalRead(DBUSY))cnt++;   // wait for not busy        
  digitalWrite(DWRITE,LOW);         // ensure Write pin is low
  for(int i=0; i<NUM_DPINS; i++)    // Setyup data pins
  {
    if(digit & Data) digitalWrite(pins[i],HIGH);
    else digitalWrite(pins[i],LOW);
    digit = digit << 1;
  }
  digitalWrite(DWRITE,HIGH);        // trigger display to read data
  delayMicroseconds(21);            // wait for at least 20uSec
}

//-----------------------------------------------------------------------------
void testDisplay(void)
{
  for(unsigned char c = 0x20; c < 0x7F; c++) WriteDisplayByte(c);
}
//-----------------------------------------------------------------------------
void ResetDisplay(void)
{
  unsigned int cnt = 0;
  
  Serial.println("System Init complete");
  digitalWrite(RESETPIN, LOW);
  delay(2);                       // pulse for a 2 mSec
  digitalWrite(RESETPIN, HIGH);
  delayMicroseconds(21);            // wait for at least 20uSec
  while(digitalRead(DBUSY))cnt++;   // Ensure Display has reset and is ready        
}
//-----------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);  // start serial for output
  Serial.println("System Init");

  Wire.begin(0x44);          // join i2c bus at slave address (address optional for master)
  Wire.onReceive(receiveEvent); // register recieve from master event
  Wire.onRequest(sendEvent);    // register send to master event
  Serial.println("I2C Init complete, waiting for slave data");


  for(int i=0; i<NUM_DPINS; i++) // Setyup data pins
  {
    pinMode(pins[i],OUTPUT);
    digitalWrite(pins[i],LOW);
  }
  pinMode(DWRITE, OUTPUT);
  digitalWrite(DWRITE, HIGH);
  
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH); 

  pinMode(DBUSY, INPUT);

  ResetDisplay();   // Init and clear Display)
  testDisplay(); 
}
//-----------------------------------------------------------------------------
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
 void sendEvent() 
{
  Serial.print("T>");
  Wire.write(x++);
  Serial.println(x,HEX); // print the data
}

//-----------------------------------------------------------------------------
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.print("R>");
  y = Wire.read(); // receive byte as int
  Serial.println(y,HEX); // print the data

}

//-----------------------------------------------------------------------------
// Main loop function for Arduino
void loop() 
{
  for(unsigned char c = 0; c < sizeof(HW); c++) WriteDisplayByte(HW[c]);
  WriteDisplayByte(y);  delay(1000);   // pause for reading
  WriteDisplayByte(0x0C);WriteDisplayByte(0x0B);  // clear display
  delay(100);
}
