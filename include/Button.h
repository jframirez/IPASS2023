/* 
* Button.h
*
* Created: 1-7-2023 19:21:03
* Author: Jordan
*/


#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "PinDriver.h"
#include "InterruptRef.h"

class Button: public PinDriver, public InterruptRef{
//functions
public:
	Button(Pio * portButton, int pinButton);

	/* InterruptRef abstract override */
	void callReference(IRQn interruptNumber, uint32_t flag) override;
	
	bool wasPressed();
	void resetButton();
	
	~Button();
	
protected:
private:
	
	bool was_pressed = false;

	Button( const Button &c );
	Button& operator=( const Button &c );

}; //Button

#endif //__BUTTON_H__
