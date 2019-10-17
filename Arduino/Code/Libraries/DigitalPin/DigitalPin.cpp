
#include "DigitalPin.h"

//******************************************************************************************************************************************************
//'member' definitions: (here we define the actual c++ code and memory allocation of the members)
//******************************************************************************************************************************************************

DigitalPin::DigitalPin(int pin) //this is the constructor
{
  _pin = pin; //here we store the pin number in the private variable _pin.
}

void DigitalPin::beginOUT() //this initializes the pin as output
{
  pinMode(_pin, OUTPUT);
}

void DigitalPin::beginIN() //this initializes the pin as input
{
  pinMode(_pin, INPUT);
}

void DigitalPin::beginPULLUP() //this initializes the pin as input_pullup
{
  pinMode(_pin, INPUT_PULLUP);
}

void DigitalPin::on() //this turns the pin on
{
  digitalWrite(_pin, HIGH);
  _status=1; //set the status property
}

void DigitalPin::off() //this turns the pin off
{
  digitalWrite(_pin, LOW);
  _status=0; //set the status property
}

void DigitalPin::level(int val)
{
  analogWrite(_pin, val);
}

bool DigitalPin::status() //this returns the current status of the pin.
//It could be used by some other component of the sketch that needs to know whether the pin is on or off.
//This is an example for a method for 'communication' between or with objects.
{
    return _status;
}

bool DigitalPin::readState()
{
	return digitalRead(_pin);
}

unsigned long DigitalPin::pulseInHigh() // returns the length of a HIGH pulse on this pin
{
	return pulseIn(_pin, HIGH);
}

unsigned long DigitalPin::pulseInLow() // returns the length of a LOW pulse on this pin
{
	return pulseIn(_pin, LOW);
}

unsigned long DigitalPin::pulseInHighTOut(unsigned long timeOut) // returns the length of a HIGH pulse on this pin
{
	return pulseIn(_pin, HIGH,  timeOut);
}

unsigned long DigitalPin::pulseInLowTOut(unsigned long timeOut) // returns the length of a LOW pulse on this pin
{
	return pulseIn(_pin, LOW, timeOut);
}