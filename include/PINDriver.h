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
class PINDriver{
	protected:
		Pio * PORT;
		int PIN;

	public:
	
		/**
		 * Setup a single pin.
		 *
		 * \param port Pio port that contains the pin.
		 * \param pin Pin number.
		 */
		PINDriver(Pio * port, int pin);
	
		
		/**
		 * Brief.
		 */
		inline void ControllerPIOEnable(){
			PORT->PIO_PER |= ( 1 << PIN);
		}

		/**
		 * Brief.
		 */
		inline void ControllerPIODisable(){
			PORT->PIO_PDR |= ( 1 << PIN);
		}

		/**
		 * Brief.
		 */
		inline void PeripheralABSelect(PIO_ABSR_SELECT abVal){
			if(abVal == PIO_ABSR_SELECT::PIO_ABSR_A){
				PORT->PIO_ABSR &= ~(1 << PIN);
			}
		
			if(abVal == PIO_ABSR_SELECT::PIO_ABSR_B){
				PORT->PIO_ABSR |= (1 << PIN);
			}
		}
	
		/**
		 * Brief.
		 */
		inline void SetOutput(PIO_PIN_STATE val){
			PORT->PIO_PER |= (1 << PIN); //Set GPIO use
			PORT->PIO_OER |= (1 << PIN); //Output Enable
			if(val == PIO_PIN_STATE::LOW){
				PORT->PIO_CODR |= (1 << PIN);
			}
		
			if(val == PIO_PIN_STATE::HIGH){
				PORT->PIO_SODR |= (1 << PIN);
			}
		}
		
		inline PIO_PIN_STATE GetState(){
			uint32_t pinStatus = (PORT->PIO_PDSR & (1 << PIN));
			if(pinStatus){
				return PIO_PIN_STATE::HIGH;
			}
			return PIO_PIN_STATE::LOW;
		}
		
		inline void SetInputFilter(PIO_INPUT_FILTER filter, uint16_t clkDiv = 0){
			if(filter == PIO_INPUT_FILTER::GLITCH){
				PORT->PIO_SCIFSR |= (1 << PIN);
			}
			if(filter == PIO_INPUT_FILTER::DEBOUNCE){
				PORT->PIO_DIFSR |= (1 << PIN);
				PORT->PIO_SCDR = (clkDiv & 0x3FFF); //Remove top 2 bits
			}
			
		}
		
		inline void DisableInputFilter(){
			PORT->PIO_IFDR |= (1 << PIN);
		}
		
		inline void InterruptEnable(){
			PORT->PIO_IER |= (1 << PIN);
		}
	
		inline void InterruptDisable(){
			PORT->PIO_IDR |= (1 << PIN);
		}
		
		inline void EnableAdditionalInterruptMode(PIO_LEVEL_SELECT level){
			
			PORT->PIO_AIMER |= (1 << PIN);
			
			switch (level){
				case PIO_LEVEL_SELECT::FALLING_EDGE:
					PORT->PIO_ESR |= (1 << PIN);
					PORT->PIO_FELLSR |= (1 << PIN);
					break;
				case PIO_LEVEL_SELECT::RISING_EDGE:
					PORT->PIO_ESR |= (1 << PIN);
					PORT->PIO_REHLSR |= (1 << PIN);
					break;
				case PIO_LEVEL_SELECT::LOW_LEVEL:
					PORT->PIO_LSR |= (1 << PIN);
					PORT->PIO_FELLSR |= (1 << PIN);
					break;
				case PIO_LEVEL_SELECT::HIGH_LEVEL:
					PORT->PIO_LSR |= (1 << PIN);
					PORT->PIO_REHLSR |= (1 << PIN);
					break;
			}
		}
		
		inline void DisableAdditionalInterruptMode(){
			PORT->PIO_AIMDR |= (1 << PIN);
			
			//Reset registers to default
			PORT->PIO_ESR |= (1 << PIN);
			PORT->PIO_FELLSR |= (1 << PIN);
		}
		
		inline void EnablePullUp(){
			PORT->PIO_PUER |= (1 << PIN);
		}
		
		inline void DisablePullUp(){
			PORT->PIO_PUDR |= (1 << PIN);
		}
		
		 //Peripheral AB Select PIO_ABSR
	
		~PINDriver();
		
	private:
		PINDriver( const PINDriver &c );
		PINDriver& operator=( const PINDriver &c );

}; //PINDriver

#endif //__PINDRIVER_H__
