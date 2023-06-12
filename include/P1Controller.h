/* 
* P1Controller.h
*
* Created: 4-6-2023 18:22:03
* Author: Jordan
*/


#ifndef __P1CONTROLLER_H__
#define __P1CONTROLLER_H__

#include "Helper.h"
#include <list>
#include <cstring>

#include <iomanip>
#include <string>
#include <stdlib.h>
#include <sstream>

//extern const char * testP1Telegram;

class P1Controller{
//variables
public:
protected:
private:

//functions
public:
	P1Controller();
	~P1Controller();
	
	//std::stringstream stream;
	
	//stream << "TEST";
	
	static int decodeP1(const char * msgStart); //should be null terminated string
	
	
	// [0] --- [1] --- [2] --- [n]
	// P1 --- M-bus0 --- Mbus1 --- Mbus n
	std::list<int> * nList;
	
protected:
private:
	P1Controller( const P1Controller &c );
	P1Controller& operator=( const P1Controller &c );

}; //P1Controller


//class COSEMBase{

	
	
//};

//class COSEM




#endif //__P1CONTROLLER_H__
