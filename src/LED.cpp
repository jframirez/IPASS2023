/* 
* LED.cpp
*
* Created: 4-5-2023 04:31:30
* Author: Jordan
*/


#include "../include/LED.h"

LED::LED(Pio * portLED, int pinLED, LEDTYPE lT):
	PINDriver(portLED, pinLED)
	{
		inverted = false;
		switch (lT){
			case LEDTYPE::DEFAULT:
			case LEDTYPE::INVERTED:
				inverted = true;
			case LEDTYPE::OPENCOLLECTOR:
				//Write MultiDrive
				PORT->PIO_MDER |= (1 << PIN);
			case LEDTYPE::OPENCOLLECTOR_INV:
				PORT->PIO_MDER |= (1 << PIN);
				inverted = true;
			default:
				
				PORT->PIO_PER |= (1 << PIN); //Set GPIO use
				PORT->PIO_OER |= (1 << PIN); //Output Enable
				PORT->PIO_CODR |= (1 << PIN); //Write LOW
				break;
		}
	
}

void LED::On(){
	if(inverted){
		PORT->PIO_CODR |= (1 << PIN);
		return;
	}
	PORT->PIO_SODR |= (1 << PIN);
}

void LED::Off(){	
	if(inverted){
		PORT->PIO_SODR |= (1 << PIN);
		return;
	}
	PORT->PIO_CODR |= (1 << PIN);
	
}

void LED::Toggle(){
	//Read current state.
	uint32_t pinStatus = (PORT->PIO_PDSR & (1 << PIN));
	if(pinStatus){
		PORT->PIO_CODR |= (1 << PIN);
	}else{
		PORT->PIO_SODR |= (1 << PIN);
	}
}

LED::~LED(){
	
} //~LED