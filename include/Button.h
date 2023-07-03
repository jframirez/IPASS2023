/* 
* Button.h
*
* Created: 1-7-2023 19:21:03
* Author: Jordan
*/


#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "PinDriver.h"
#include "Interruptible.h"

/**
 * Button that is interruptable.
 */
class Button: public PinDriver{
public:

	/**
	 * Make a button.
	 *
	 * \param port_button Pio pointer to port
	 * \param pin_button pin in Pio port
	 */
	Button(Pio * port_button, int pin_button);

	/**
	 * If button is registered in a interrupt this function gets called
	 *
	 * \param interrupt_number called from IRQn.
	 * \param flag_ interupt flag
	 */
	void callReference(IRQn interrupt_number, uint32_t flag_) override;
	
	/**
	 * If the button is pressed
	 *
	 * \return bool was the button pressed
	 */
	bool wasPressed();
	
	/**
	 * Reset internal button state.
	 */
	void resetButton();
	
	~Button();
	
protected:
private:
	
	bool was_pressed = false;

	Button( const Button &c );
	Button& operator=( const Button &c );

}; //Button

#endif //__BUTTON_H__
