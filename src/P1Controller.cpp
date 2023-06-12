/* 
* P1Controller.cpp
*
* Created: 4-6-2023 18:22:03
* Author: Jordan
*/


#include "P1Controller.h"

//const char * testP1Telegram = "/ISk5\2MT382-1000\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(101209113020W)\r\n0-0:96.1.1(4B384547303034303436333935353037)\r\n1-0:1.8.1(123456.789*kWh)\r\n1-0:1.8.2(123456.789*kWh)\r\n1-0:2.8.1(123456.789*kWh)\r\n1-0:2.8.2(123456.789*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(01.193*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00004)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\r\n1-0:32.32.0(00002)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00003)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)\r\n1-0:32.7.0(220.1*V)\r\n1-0:52.7.0(220.2*V)\r\n1-0:72.7.0(220.3*V)\r\n1-0:31.7.0(001*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(003*A)\r\n1-0:21.7.0(01.111*kW)\r\n1-0:41.7.0(02.222*kW)\r\n1-0:61.7.0(03.333*kW)\r\n1-0:22.7.0(04.444*kW)\r\n1-0:42.7.0(05.555*kW)\r\n1-0:62.7.0(06.666*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(3232323241424344313233343536373839)\r\n0-1:24.2.1(101209112500W)(12785.123*m3)\r\n!EF2F";

// default constructor
P1Controller::P1Controller()
{
	
	//nList = new std::list<int>();
	
} //P1Controller


int P1Controller::decodeP1(const char * msgStart){
	
	//Return -1 -> msg is not a P1 Telegram
	//Return -2 -> CRC error
	//Return -3 -> Generic parsing error
	//Return 0 -> Succes
	//Return 1 -> Return pointer available in recent msg
	
	
	const char * pCursor = msgStart;
	
	if(*pCursor == '/'){
		pCursor += 3;
		if(*pCursor == '5'){
			//Msg is P1
			++pCursor;
		}else{
			return -1;
		}	
	}else{
		return -1;
	}
	
	//Ident
	const char * identStart = pCursor;
	
	//Ident lenght
	int identLenght = 0;
	while(*pCursor != '\r'){
		++identLenght;
		++pCursor;
	}

	char * testIdent = new char[identLenght + 1];
	
	strncpy(testIdent, identStart, identLenght);
	testIdent[identLenght + 1] = '\0';
	
	Helper::Debug::DebugPrint("P1 Telegram Ident: \r\n");
	Helper::Debug::DebugPrint(testIdent);
	Helper::Debug::DebugPrint("\r\n");
	
	
	
	
	return 0;
}








// default destructor
P1Controller::~P1Controller()
{
	delete nList;
} //~P1Controller
