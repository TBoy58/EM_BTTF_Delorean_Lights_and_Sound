#ifndef DigitalPin_h
#define DigitalPin_h

#include "Arduino.h"

//******************************************************************************************************************************************************
//class definition (this is a 'pre-declaration'. It tells the compiler about the existence of the 'members' (i.e. functions ('methods') and variables ('properties')) of the class

//******************************************************************************************************************************************************

class DigitalPin
// DigitalPin - A bit of a misnomer as it defines all pin types - Tboy58
{
  public://access specifier - this tells whether the 'members' are visible from the outside ('public") or only from the inside ('private')
    /*
    This is the 'constructor', which is automatically called whenever a new object of this class is created, allowing the class to initialize member variables or allocate storage.
    This constructor function is declared just like a regular member function, but with a name that matches the class name and without any return type; not even void.
    */
    DigitalPin(int pin);
    //these are the public class 'members'. The actual code for these members is given below...here we just tell the compiler about their existence, and what values are being returned
    void beginOUT();
	void beginIN();
	void beginPULLUP();
    void on();
    void off();
	void level(int val);
    bool status();
	bool readState();
	unsigned long pulseInHigh();
	unsigned long pulseInLow();
	unsigned long pulseInHighTOut(unsigned long timeOut);
	unsigned long pulseInLowTOut(unsigned long timeOut);
	
  private://these class members can only be used from within the class, i.e. they cannot be directly accessed from the outside. It is good practice to make all variables ('properties') private
          //and access them via public functions. That way they cannot be accidentally be changed via buggy code. This example only shows 'properties', but there could also be private 'methods'
    int _pin;
    bool _status;
};

#endif