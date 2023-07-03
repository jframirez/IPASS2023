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

void RotaryEncoder::callReference(IRQn interruptNumber, uint32_t flag){

	bool a = rotary_a.getStateBool();
	bool b = rotary_b.getStateBool();
	
	int current_state = 0;
	
	if(a){
		current_state |= (1 << 0);
	}
	
	if(b){
		current_state |= (1 << 1);
	}
	
	
	//Transistion from
	//10231 =  Left
	
	//20132 =  Right
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
	
// 	STATE current_state = NONE;
// 	
// 	if(a && b){
// 		current_state = AandB;
// 	}else if(a && !b){
// 		current_state = AnotB;
// 	}else if (!a && b){
// 		current_state = BnotA;
// 	}else{
// 		current_state = NONE;
// 	}
	
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
