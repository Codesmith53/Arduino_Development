
// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006
// This example code is in the public domain.


#include <Wire.h> 

unsigned char cnt=0,CountTimer;
unsigned char x = 0xAA,y;    // receive-transmit data loopback
unsigned long counter;

void setup()
{
	Serial.begin(115200);  // start serial for output
	Serial.println("System Init");

	Wire.begin(0x44);          // join i2c bus at slave address (address optional for master)
	Wire.onReceive(receiveEvent); // register recieve from master event
	Wire.onRequest(sendEvent);    // register send to master event
	Serial.println("I2C Init complete, waiting for slave data");

}

void loop()
{
  // Something to do!
  int cnt = 0;
  if(++cnt >= 10) cnt = 0;
}

//////////////////// I2C Tx testing /////////////////////
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
 void sendEvent() 
{
  Serial.print("T>");
  Wire.write(x++);
  Serial.println(x,HEX); // print the data
}

//////////////////// I2C Rec testing /////////////////////
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.print("R>");
	y = Wire.read(); // receive byte as int
	Serial.println(y,HEX); // print the data

}


