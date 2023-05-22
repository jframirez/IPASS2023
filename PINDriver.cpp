/* 
* PINDriver.cpp
*
* Created: 9-5-2023 05:14:02
* Author: Jordan
*/


#include "PINDriver.h"

// default constructor
PINDriver::PINDriver(Pio * port, int pin):
	PORT(port),
	PIN(pin)
	{
	//PORT->PIO_PER |= (1 << PIN); //Set GPIO use
	//PORT->PIO_OER |= (1 << PIN); //Output Enable
	//PORT->PIO_CODR &= ~(1 << PIN); //Write LOW
} //PINDriver

/*
void PINDriver::ControllerPIOEnable(){
	PORT->PIO_PER |= ( 1 << PIN);
}

void PINDriver::ControllerPIODisable(){
	PORT->PIO_PDR |= ( 1 << PIN);
}

void PINDriver::PeripheralABSelect(PIO_ABSR_SELECT abVal){
	if(abVal == PIO_ABSR_SELECT::PIO_ABSR_A){
		PORT->PIO_ABSR &= ~(1 << PIN);
	}
	
	if(abVal == PIO_ABSR_SELECT::PIO_ABSR_B){
		PORT->PIO_ABSR |= (1 << PIN);
	}
}

void PINDriver::SetOutput(PIO_OUTPUT val){
	PORT->PIO_PER |= (1 << PIN); //Set GPIO use
	PORT->PIO_OER |= (1 << PIN); //Output Enable
	if(val == PIO_OUTPUT::LOW){
		PORT->PIO_CODR |= (1 << PIN);
	}
	
	if(val == PIO_OUTPUT::HIGH){
		PORT->PIO_SODR |= (1 << PIN);
	}
}
*/



// default destructor
PINDriver::~PINDriver()
{
} //~PINDriver
