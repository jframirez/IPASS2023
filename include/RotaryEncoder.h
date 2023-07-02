/* 
* RotaryEncoder.h
*
* Created: 2-7-2023 00:59:38
* Author: Jordan
*/


#ifndef __ROTARYENCODER_H__
#define __ROTARYENCODER_H__

#include "PinDriver.h"
#include "InterruptRef.h"

enum class ROTATION{
	NONE = 0,
	LEFT = 1,
	RIGHT = 2
};

class RotaryEncoder: public InterruptRef{
private:
	PinDriver rotaryA;
	PinDriver rotaryB;
	IRQn aInterrupt;
	IRQn bInterrupt;
	
	int last_state = 0;
	
	int rotation = 0;

//functions
public:
	RotaryEncoder(	PinDriver a, IRQn aI,
					PinDriver b, IRQn bI);
	
	void callReference(IRQn interruptNumber, uint32_t flag) override;
	
	int getRotation();
	
	int getState();
	
	void reset();
	
	~RotaryEncoder();

private:
	RotaryEncoder( const RotaryEncoder &c );
	RotaryEncoder& operator=( const RotaryEncoder &c );

}; //RotaryEncoder

#endif //__ROTARYENCODER_H__
