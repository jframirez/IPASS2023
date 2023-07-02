/* 
* RotaryEncoder.cpp
*
* Created: 2-7-2023 00:59:38
* Author: Jordan
*/


#include "RotaryEncoder.h"

// default constructor
RotaryEncoder::RotaryEncoder(	PinDriver a, IRQn aI,
								PinDriver b, IRQn bI):
								rotaryA(a),
								rotaryB(b),
								aInterrupt(aI),
								bInterrupt(bI){
		bool aState = rotaryA.getStateBool();
		bool bState = rotaryB.getStateBool();
		
		int currentState = 0;
		
		if(aState){
			currentState |= (1 << 0);
		}
		
		if(bState){
			currentState |= (1 << 1);
		}
		
		last_state = 0;
		
// 		if(aState && bState){
// 			last_state = AandB;
// 		}else if(aState && !bState){
// 			last_state = AnotB;
// 		}else if (!aState && bState){
// 			last_state = BnotA;
// 		}
// 		
// 		last_state = NONE;
}

void RotaryEncoder::callReference(IRQn interruptNumber, uint32_t flag){

	bool a = rotaryA.getStateBool();
	bool b = rotaryB.getStateBool();
	
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
		rotation -= 1;
	}
	
	if(ts == 11 || ts == 2 || ts == 4 || ts == 13){
		//Right
		rotation += 1;
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
	return rotation;
}	

int RotaryEncoder::getState(){
	return last_state;
}

void RotaryEncoder::reset(){
	//last_state = 0;
	rotation = 0;
}

// default destructor
RotaryEncoder::~RotaryEncoder()
{
} //~RotaryEncoder
