/* 
* RotaryEncoder.cpp
*
* Created: 2-7-2023 00:59:38
* Author: Jordan
*/


#include "RotaryEncoder.h"

// default constructor
RotaryEncoder::RotaryEncoder(	PinDriver a, IRQn a_i,
								PinDriver b, IRQn b_i):
								rotary_a(a),
								rotary_b(b),
								a_interrupt(a_i),
								b_interrupt(b_i){
			
	rotary_a.controllerPioEnable();
	rotary_b.controllerPioEnable();
	
	rotary_a.setInputFilter(PIO_INPUT_FILTER::GLITCH);
	rotary_b.setInputFilter(PIO_INPUT_FILTER::GLITCH);
	
	rotary_a.interruptEnable();
	rotary_b.interruptEnable();
	
	rotary_a.enablePullUp();
	rotary_b.enablePullUp();
	
	rotary_a.disableAdditionalInterruptMode(); //Default triggers on both edges
	rotary_b.disableAdditionalInterruptMode(); //Default triggers on both edges
	
	bool a_state = rotary_a.getStateBool();
	bool b_state = rotary_b.getStateBool();
	
	int current_state = 0;
	
	if(a_state){
		current_state |= (1 << 0);
	}
	
	if(b_state){
		current_state |= (1 << 1);
	}
	
	last_state = 0;
}

void RotaryEncoder::callReference(IRQn interrupt_number, uint32_t flag_){
	
	if((a_interrupt != interrupt_number) || (b_interrupt != interrupt_number)){
		//Interrupt is not for this object
		return;
	}
	
	bool a = rotary_a.getStateBool();
	bool b = rotary_b.getStateBool();
	
	int current_state = 0;
	
	if(a){
		current_state |= (1 << 0);
	}
	
	if(b){
		current_state |= (1 << 1);
	}

	int ts = (current_state << 2) | (last_state); // total state
	
	if(ts == 7 || ts == 1 || ts == 8 || ts == 14){
		//Left
		rotation_ -= 1;
	}
	
	if(ts == 11 || ts == 2 || ts == 4 || ts == 13){
		//Right
		rotation_ += 1;
	}
	
		
	last_state = current_state;
	
};


int RotaryEncoder::getRotation(){
	return rotation_;
}	

int RotaryEncoder::getLastState(){
	return last_state;
}

void RotaryEncoder::reset(){
	rotation_ = 0;
}

// default destructor
RotaryEncoder::~RotaryEncoder(){
}
