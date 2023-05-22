/* 
* LED.h
*
* Created: 4-5-2023 04:31:30
* Author: Jordan
*/


#ifndef __LED_H__
#define __LED_H__

#include "sam.h"

class LED
{
//variables
public:
protected:
private:
	Pio * PORT;
	int PIN;
	bool inverted = false;
//functions
public:
	LED(Pio * portLED, int pinLED);
	LED(Pio * portLED, int pinLED, bool isInverted);
	~LED();
	void ON();
	void OFF();
protected:
private:
	LED( const LED &c );
	LED& operator=( const LED &c );

}; //LED

#endif //__LED_H__
