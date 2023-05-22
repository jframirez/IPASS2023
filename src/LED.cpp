/* 
* LED.cpp
*
* Created: 4-5-2023 04:31:30
* Author: Jordan
*/


#include "../include/LED.h"

// default constructor

/*
*
*/
LED::LED(Pio * portLED, int pinLED): 
	PORT(portLED), 
	PIN(pinLED)
	{
		PORT->PIO_PER |= (1 << PIN); //Set GPIO use
		PORT->PIO_OER |= (1 << PIN); //Output Enable
		PORT->PIO_CODR &= ~(1 << PIN); //Write LOW	
} //LED

/*
*
*/
LED::LED(Pio * portLED, int pinLED, bool isInv):
	PORT(portLED),
	PIN(pinLED),
	inverted(isInv)
{
	PORT->PIO_PER |= (1 << PIN); //Set GPIO use
	PORT->PIO_OER |= (1 << PIN); //Output Enable
	PORT->PIO_CODR |= (1 << PIN);
} //LED

/*
*
*/
void LED::ON(){
	if(inverted){
		PORT->PIO_CODR |= (1 << PIN);
		return;
	}
	PORT->PIO_SODR |= (1 << PIN);
}

/*
*
*/
void LED::OFF(){	
	if(inverted){
		PORT->PIO_SODR |= (1 << PIN);
		return;
	}
	PORT->PIO_CODR |= (1 << PIN);
	
}

// default destructor
LED::~LED(){
	
} //~LED