/* 
* LED.cpp
*
* Created: 4-5-2023 04:31:30
* Author: Jordan
*/


#include "../include/Led.h"

Led::Led(Pio * portLed, int pinLed, LEDTYPE ledType):
	PinDriver(portLed, pinLed)
	{
		inverted = false;
		switch (ledType){
			case LEDTYPE::DEFAULT:
			case LEDTYPE::INVERTED:
				inverted = true;
			case LEDTYPE::OPENCOLLECTOR:
				//Write MultiDrive
				port->PIO_MDER |= (1 << pin);
			case LEDTYPE::OPENCOLLECTOR_INV:
				port->PIO_MDER |= (1 << pin);
				inverted = true;
			default:	
				port->PIO_PER |= (1 << pin); //Set GPIO use
				port->PIO_OER |= (1 << pin); //Output Enable
				port->PIO_CODR |= (1 << pin); //Write LOW
				break;
		}
	
}

void Led::on(){
	if(inverted){
		port->PIO_CODR |= (1 << pin);
		return;
	}
	port->PIO_SODR |= (1 << pin);
}

void Led::off(){	
	if(inverted){
		port->PIO_SODR |= (1 << pin);
		return;
	}
	port->PIO_CODR |= (1 << pin);
	
}

void Led::toggle(){
	//Read current state.
	uint32_t pinStatus = (port->PIO_PDSR & (1 << pin));
	if(pinStatus){
		port->PIO_CODR |= (1 << pin);
	}else{
		port->PIO_SODR |= (1 << pin);
	}
}

Led::~Led(){
	
} //~Led