/* 
* RotaryEncoder.h
*
* Created: 2-7-2023 00:59:38
* Author: Jordan
*/


#ifndef __ROTARYENCODER_H__
#define __ROTARYENCODER_H__

#include "PinDriver.h"
#include "Interruptible.h"

/**
 * Make a rotary encoder.
 */
class RotaryEncoder: public Interruptible{
private:
	PinDriver rotary_a;
	PinDriver rotary_b;
	IRQn a_interrupt;
	IRQn b_interrupt;
	
	int last_state = 0;
	
	int rotation_ = 0;

//functions
public:

	/**
	 * Make a rotary encoder.
	 *
	 * \param a PinDriver for a signal.
	 * \param a_i a-signal receives interrupt from IRQn
	 * \param b PinDriver for b signal.
	 * \param b_i b-signal receives interrupt from IRQn
	 */
	RotaryEncoder(	PinDriver a, IRQn a_i,
					PinDriver b, IRQn b_i);
	
	/**
	 * If rotary encoder is registered in a interrupt this function gets called
	 *
	 * \param interrupt_number called from IRQn.
	 * \param flag_ interupt flag
	 */
	void callReference(IRQn interrupt_number, uint32_t flag_) override;
	
	/**
	 * Get current rotation value.
	 *
	 * Rotation increments on a-signal and decrements on b-signal.
	 *	
	 * \return int rotation value
	 */
	int getRotation();
	
	/**
	 * Get previous state.
	 *
	 * State is an encoded value. This is the last state of
	 * pin a and b when called from interrupt.
	 *	
	 * \return int encoded last_state
	 */
	int getLastState();
	
	/**
	 * Reset counted rotations.
	 */
	void reset();
	
	~RotaryEncoder();

private:
	RotaryEncoder( const RotaryEncoder &c_ );
	RotaryEncoder& operator=( const RotaryEncoder &c_ );

}; //RotaryEncoder

#endif //__ROTARYENCODER_H__
