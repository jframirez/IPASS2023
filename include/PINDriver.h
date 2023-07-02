/* 
* PinDriver.h
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


enum class PIO_PIN_STATE{
	LOW = 0,	
	HIGH = 1
};

// bool operator(const T& lhs, const T& rhs ILI_COLORS lhs, uint shift){
// 	return ((uint16_t)lhs >> shift);
// }

enum class PIO_INPUT_FILTER{
	GLITCH = 0,
	DEBOUNCE = 1
};

enum class PIO_LEVEL_SELECT{
	FALLING_EDGE = 0,
	RISING_EDGE = 1,
	LOW_LEVEL = 2,
	HIGH_LEVEL = 3
};

/**
 * Setup a pin.
 *
 * Setup a pin on the uC. This allow to setup the basic functions
 * on the pin that fall under Parallel Input/Output Controller (PIO)
 *
 */
class PinDriver{
protected:
	Pio * port; /**< port where pin is located */
	int pin; /**< pin number */

public:
	
	/**
	* Setup a single pin.
	*
	* \param port Pio port that contains the pin.
	* \param pin Pin number.
	*/
	PinDriver(Pio * port, int pin);
	
	/**
	 * Copy constructor.
	 */
	PinDriver( const PinDriver &c );
		
	/**
	* Enable PIO on pin.
	*/
	inline void controllerPioEnable(){
		port->PIO_PER |= ( 1 << pin);
	}

	/**
	* Disable PIO on pin.
	*/
	inline void controllerPioDisable(){
		port->PIO_PDR |= ( 1 << pin);
	}

	/**
	* Set peripheral function of pin
	*
	* \param abVal PIO_ABSR_SELECT select peripheral type
	* \see PIO_ABSR_SELECT
	*/
	inline void peripheralABSelect(PIO_ABSR_SELECT abVal){
		if(abVal == PIO_ABSR_SELECT::PIO_ABSR_A){
			port->PIO_ABSR &= ~(1 << pin);
		}
		
		if(abVal == PIO_ABSR_SELECT::PIO_ABSR_B){
			port->PIO_ABSR |= (1 << pin);
		}
	}
	
	/**
	* Set pin to output and set the output value
	*
	* \param val set pin to PIO_PIN_STATE
	* \see PIO_PIN_STATE
	*/
	inline void setOutput(PIO_PIN_STATE val){
		port->PIO_PER |= (1 << pin); //Set GPIO use
		port->PIO_OER |= (1 << pin); //Output Enable
		if(val == PIO_PIN_STATE::LOW){
			port->PIO_CODR |= (1 << pin);
		}
		
		if(val == PIO_PIN_STATE::HIGH){
			port->PIO_SODR |= (1 << pin);
		}
	}
	
	inline void setOutput(bool val){
		port->PIO_PER |= (1 << pin); //Set GPIO use
		port->PIO_OER |= (1 << pin); //Output Enable
		
		if(val){
			port->PIO_SODR |= (1 << pin);
		}else{
			port->PIO_CODR |= (1 << pin);
		}
	}
		
	/**
	* Get current value of pin.
	*
	* \return PIO_PIN_STATE value of pin
	* \see PIO_PIN_STATE
	*/
	inline PIO_PIN_STATE getState(){
		uint32_t pinStatus = (port->PIO_PDSR & (1 << pin));
		if(pinStatus){
			return PIO_PIN_STATE::HIGH;
		}
		return PIO_PIN_STATE::LOW;
	}
	
	inline bool getStateBool(){
		uint32_t pinStatus = (port->PIO_PDSR & (1 << pin));
		if(pinStatus){
			return true;
		}
		return false;
	}
		
	/**
	* Set input filter type for pin.
	*
	* If filter is of type PIO_INPUT_FILTER::DEBOUNCE
	* the clkDiv parameter gets used to set the clock divider
	* value for debouncing.
	*
	* \param filter an integer argument.
	* \param clkDiv if filter is PIO_INPUT_FILTER::DEBOUNCE set this value to clock divider.
	* \see PIO_INPUT_FILTER
	*/
	inline void setInputFilter(PIO_INPUT_FILTER filter, uint16_t clkDiv = 0){
		if(filter == PIO_INPUT_FILTER::GLITCH){
			port->PIO_SCIFSR |= (1 << pin);
		}
		if(filter == PIO_INPUT_FILTER::DEBOUNCE){
			port->PIO_DIFSR |= (1 << pin);
			port->PIO_SCDR = (clkDiv & 0x3FFF); //Remove top 2 bits
		}
			
	}
		
	/**
	* Disable input filter on pin.
	*/
	inline void disableInputFilter(){
		port->PIO_IFDR |= (1 << pin);
	}
		
	/**
	* Enable interrupt on pin.
	*/
	inline void interruptEnable(){
		port->PIO_IER |= (1 << pin);
	}
		
	/**
	* Disable interrupt on pin.
	*/
	inline void interruptDisable(){
		port->PIO_IDR |= (1 << pin);
	}
		
	/**
	* Set the type of additional interrupt with PIO_LEVEL_SELECT.
	* 
	* \param level set the type the additional interrupt uses.
	* \see PIO_LEVEL_SELECT
	*/
	void enableAdditionalInterruptMode(PIO_LEVEL_SELECT level);
		
	/**
	* Disable additional interrupt mode by resetting
	* the registers to default values.
	*/
	inline void disableAdditionalInterruptMode(){
		//Reset registers to default
		port->PIO_ESR |= (1 << pin);
		port->PIO_FELLSR |= (1 << pin);
		
		port->PIO_AIMDR |= (1 << pin);
	}
		
	/**
	* Enable internal pull-up on pin.
	*/
	inline void enablePullUp(){
		port->PIO_PUER |= (1 << pin);
	}
		
	/**
	* Disable internal pull-up on pin.
	*/
	inline void disablePullUp(){
		port->PIO_PUDR |= (1 << pin);
	}
	
	int getPinNumber(){
		return pin;
	}
	
	inline Pio * getPort(){
		return port;
	}
	
	/**
	* Default destructor.
	*/
	~PinDriver();
		
private:
	
	PinDriver& operator=( const PinDriver &c );

}; //PinDriver

#endif //__PINDRIVER_H__
