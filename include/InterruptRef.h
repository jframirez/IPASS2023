/* 
* InterruptRef.h
*
* Created: 1-7-2023 19:22:15
* Author: Jordan
*/


#ifndef __INTERRUPTREF_H__
#define __INTERRUPTREF_H__

#include <list>

class InterruptRef{
public:
	virtual ~InterruptRef(){};
	virtual void callReference(IRQn interruptNumber, uint32_t flag) = 0;
	
};

class Interrupt{
public:
	
	Interrupt(){};
	
	void registerInterrupt(InterruptRef * i){
		list_.push_back(i);
	}
	
	void deleteInterrupt(InterruptRef * i){
		list_.remove(i);
	}
	
	void updateInterrupt(IRQn interruptNumber, uint32_t flag){
		//std::list<InterruptRef *>::iterator it = list_.begin();
		for(auto &ptr: list_){
			ptr->callReference(interruptNumber, flag);
		}
	}
	
private:
	std::list<InterruptRef *> list_;	
};

#endif //__INTERRUPTREF_H__
