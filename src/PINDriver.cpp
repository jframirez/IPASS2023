/* 
* PinDriver.cpp
*
* Created: 9-5-2023 05:14:02
* Author: Jordan
*/


#include "../include/PinDriver.h"

// default constructor
PinDriver::PinDriver(Pio * port, int pin):
	port(port),
	pin(pin)
	{
} //PinDriver




// default destructor
PinDriver::~PinDriver(){
} //~PinDriver
