/* 
* PINDriver.h
*
* Created: 9-5-2023 05:14:02
* Author: Jordan
*/


#ifndef __PINDRIVER_H__
#define __PINDRIVER_H__

#include "sam.h"

enum class PIO_ABSR_SELECT{
		PIO_ABSR_A = 0,
		PIO_ABSR_B = 1
};

enum class PIO_OUTPUT{
	LOW = 0,	
	HIGH = 1
};

class PINDriver
{
//variables
public:
protected:
private:
	Pio * PORT;
	int PIN;

//functions
public:
	PINDriver(Pio * port, int pin);
	
	//void PINSet
	
	inline void ControllerPIOEnable(){
		PORT->PIO_PER |= ( 1 << PIN);
	}

	inline void ControllerPIODisable(){
		PORT->PIO_PDR |= ( 1 << PIN);
	}

	inline void PeripheralABSelect(PIO_ABSR_SELECT abVal){
		if(abVal == PIO_ABSR_SELECT::PIO_ABSR_A){
			PORT->PIO_ABSR &= ~(1 << PIN);
		}
		
		if(abVal == PIO_ABSR_SELECT::PIO_ABSR_B){
			PORT->PIO_ABSR |= (1 << PIN);
		}
	}
	
	inline void SetOutput(PIO_OUTPUT val){
		PORT->PIO_PER |= (1 << PIN); //Set GPIO use
		PORT->PIO_OER |= (1 << PIN); //Output Enable
		if(val == PIO_OUTPUT::LOW){
			PORT->PIO_CODR |= (1 << PIN);
		}
		
		if(val == PIO_OUTPUT::HIGH){
			PORT->PIO_SODR |= (1 << PIN);
		}
	}
	
	 //Peripheral AB Select PIO_ABSR
	
	~PINDriver();
protected:
private:
	PINDriver( const PINDriver &c );
	PINDriver& operator=( const PINDriver &c );

}; //PINDriver

#endif //__PINDRIVER_H__
