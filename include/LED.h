/* 
* LED.h
*
* Created: 4-5-2023 04:31:30
* Author: Jordan
*/


#ifndef __LED_H__
#define __LED_H__

#include "sam.h"
#include "PinDriver.h"

/**
 * Implementation fo PinDriver spefically for Leds.
 */
class Led : public PinDriver{
	public:
		
		/**
		 * enum to hold led types.
		 */
		enum class LEDTYPE{
			DEFAULT = 0,		///< Drive high to turn on led.
			INVERTED,			///< Drive low to turn on led.
			OPENCOLLECTOR,		///< Sink current.
			OPENCOLLECTOR_INV,	///< Sink current, led is inverted.
		};
		
		/**
		 * Create a Led with a LED::LEDTYPE.
		 *
		 * \param portLed holds a Pio port.
		 * \param pinLed pin number in Pio.
		 * \param ledType holds LED::LEDTYPE , default is LED::LEDTYPE::DEFAULT
		 * \see LED::LEDTYPE.
		 */
		Led(Pio * portLed, int pinLed, Led::LEDTYPE ledType = LEDTYPE::DEFAULT);
		
		/**
		 * Default destructor.
		 */
		~Led();
		
		/**
		 * Turn the led on.
		 */
		void On();
		
		/**
		 * Turn the led off.
		 */
		void Off();
		
		/**
		 * Toggle current LED state.
		 */
		void Toggle();
		
		
	private:
		bool inverted;
				
		Led( const Led &c );
		Led& operator=( const Led &c );

}; //LED

#endif //__LED_H__
