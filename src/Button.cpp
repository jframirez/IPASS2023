/* 
* Button.cpp
*
* Created: 1-7-2023 19:21:03
* Author: Jordan
*/


#include "Button.h"
#include "Helper.h"

Button::Button(Pio * portButton, int pinButton):
	PinDriver(portButton, pinButton)
	{
}

void Button::callReference(IRQn interruptNumber, uint32_t flag){
	if(flag & (1 << pin)){
		was_pressed = true;
	}
}

bool Button::wasPressed(){
	return was_pressed;
}

void Button::resetButton(){
	was_pressed = false;
}

// default destructor
Button::~Button()
{
} //~Button
