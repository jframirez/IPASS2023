/* 
* Interruptible.h
*
* Created: 1-7-2023 19:22:15
* Author: Jordan
*/


#ifndef __INTERRUPTREF_H__
#define __INTERRUPTREF_H__

#include <list>

/**
 * Abstract base class of a object that can be interrupted.
 */
class Interruptible{
public:
	virtual ~Interruptible(){};
		
	/**
	 * If Interruptible is registered in a Interrupt this function gets called
	 *
	 * \param interrupt_number called from IRQn.
	 * \param flag_ interupt flag
	 */
	virtual void callReference(IRQn interrupt_number, uint32_t flag_) = 0;
	
};

/**
 * Object that can store interruptable objects.
 */
class Interrupt{
public:
	
	Interrupt(){};
	
	/**
	 * Register an interruptible object.
	 *
	 * \param i_ object to register
	 */
	void registerInterrupt(Interruptible * i_){
		list_.push_back(i_);
	}
	
	/**
	 * Remove a registered interruptible object if it is registered.
	 *
	 * \param i_ object to remove
	 */
	void deleteInterrupt(Interruptible * i_){
		list_.remove(i_);
	}
	
	/**
	 * Update all registered objects.
	 *
	 * \param interrupt_number called from IRQn.
	 * \param flag_ interupt flag.
	 */
	void updateInterrupt(IRQn interrupt_number, uint32_t flag_){
		//std::list<Interruptible *>::iterator it = list_.begin();
		for(auto &ptr: list_){
			ptr->callReference(interrupt_number, flag_);
		}
	}
	
private:
	std::list<Interruptible *> list_;	
};

#endif //__INTERRUPTREF_H__
