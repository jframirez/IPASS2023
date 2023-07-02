/* 
* PinDriver.cpp
*
* Created: 9-5-2023 05:14:02
* Author: Jordan
*/


#include "../include/PinDriver.h"

// default constructor
PinDriver::PinDriver(Pio * port, int pin):
	port(port),
	pin(pin)
	{
}

PinDriver::PinDriver( const PinDriver &c ){
	this->port = c.port;
	this->pin = c.pin;
}

void PinDriver::enableAdditionalInterruptMode(PIO_LEVEL_SELECT level){
	
	port->PIO_AIMER |= (1 << pin);
	
	switch (level){
		case PIO_LEVEL_SELECT::FALLING_EDGE:
		port->PIO_ESR |= (1 << pin);
		port->PIO_FELLSR |= (1 << pin);
		break;
		case PIO_LEVEL_SELECT::RISING_EDGE:
		port->PIO_ESR |= (1 << pin);
		port->PIO_REHLSR |= (1 << pin);
		break;
		case PIO_LEVEL_SELECT::LOW_LEVEL:
		port->PIO_LSR |= (1 << pin);
		port->PIO_FELLSR |= (1 << pin);
		break;
		case PIO_LEVEL_SELECT::HIGH_LEVEL:
		port->PIO_LSR |= (1 << pin);
		port->PIO_REHLSR |= (1 << pin);
		break;
	}
}

// default destructor
PinDriver::~PinDriver(){
} //~PinDriver
