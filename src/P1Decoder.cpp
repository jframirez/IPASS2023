/*
* P1Decoder.cpp
*
* Created: 4-6-2023 18:22:03
* Author: Jordan
*/


#include "P1Decoder.h"

//const char * testP1Telegram = "/ISk5\2MT382-1000\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(101209113020W)\r\n0-0:96.1.1(4B384547303034303436333935353037)\r\n1-0:1.8.1(123456.789*kWh)\r\n1-0:1.8.2(123456.789*kWh)\r\n1-0:2.8.1(123456.789*kWh)\r\n1-0:2.8.2(123456.789*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(01.193*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00004)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\r\n1-0:32.32.0(00002)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00003)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)\r\n1-0:32.7.0(220.1*V)\r\n1-0:52.7.0(220.2*V)\r\n1-0:72.7.0(220.3*V)\r\n1-0:31.7.0(001*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(003*A)\r\n1-0:21.7.0(01.111*kW)\r\n1-0:41.7.0(02.222*kW)\r\n1-0:61.7.0(03.333*kW)\r\n1-0:22.7.0(04.444*kW)\r\n1-0:42.7.0(05.555*kW)\r\n1-0:62.7.0(06.666*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(3232323241424344313233343536373839)\r\n0-1:24.2.1(101209112500W)(12785.123*m3)\r\n!EF2F";

//String encoded for PC testing purpose
const char * testP1Telegram = "/ISk5\\2MT382-1000\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(101209113020W)\r\n0-0:96.1.1(4B384547303034303436333935353037)\r\n1-0:1.8.1(123456.789*kWh)\r\n1-0:1.8.2(123456.789*kWh)\r\n1-0:2.8.1(123456.789*kWh)\r\n1-0:2.8.2(123456.789*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(01.193*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00004)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\r\n1-0:32.32.0(00002)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00003)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)\r\n1-0:32.7.0(220.1*V)\r\n1-0:52.7.0(220.2*V)\r\n1-0:72.7.0(220.3*V)\r\n1-0:31.7.0(001*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(003*A)\r\n1-0:21.7.0(01.111*kW)\r\n1-0:41.7.0(02.222*kW)\r\n1-0:61.7.0(03.333*kW)\r\n1-0:22.7.0(04.444*kW)\r\n1-0:42.7.0(05.555*kW)\r\n1-0:62.7.0(06.666*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(3232323241424344313233343536373839)\r\n0-1:24.2.1(101209112500W)(12785.123*m3)\r\n!E47C";

const char * testP1Telegram2 = "/KFM5KAIFA-METER\r\n\r\n1-3:0.2.8(42)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.8.1(000306.946*kWh)\r\n1-0:1.8.2(000210.088*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(02.793*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00001)\r\n0-0:96.7.9(00001)\r\n1-0:99.97.0(1)(0-0:96.7.19)(000101000006W)(2147483647*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00000)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(003*A)\r\n1-0:51.7.0(005*A)\r\n1-0:71.7.0(005*A)\r\n1-0:21.7.0(00.503*kW)\r\n1-0:41.7.0(01.100*kW)\r\n1-0:61.7.0(01.190*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303331303033333738373931363136)\r\n0-1:24.2.1(170124210000W)(00671.790*m3)\r\n!29ED";

// default constructor
P1Decoder::P1Decoder(){
	
} //P1Decoder


int P1Decoder::decodeP1(const char * msgStart, std::list<OBISChannel*>&channelList, P1Decoder & p1C){
	
	//Return -1 -> msg is not a P1 Telegram
	//Return -2 -> CRC error
	//Return -3 -> Generic parsing error
	//Return 0 -> Succes
	//Return 1 -> Return pointer available in recent msg
	
	const char * pCursor = msgStart;

	if(*pCursor == '/'){ //CRC skip /
		//Chekc crc
		CRCreturn CRCout = CRCp1::crcGen(msgStart, '!');
		if(CRCout.crcErrorCode == 0){
			if(pCursor[CRCout.crcByteLenght] == '!'){
				//Make sure 5th next symbol = \r else error
				if(pCursor[CRCout.crcByteLenght + 5] == '\0'){
					//std::cout << "CHECKING CRC CODE" << std::endl;
					//std::cout << "CRC CODE : " << CRCout.crcValue << std::endl;
					if(std::stoi(pCursor + (CRCout.crcByteLenght + 1), nullptr, 16) == CRCout.crcValue){
						//CRC ok
						//std::cout << "CRC ok" << std::endl;
						}else{
						//std::cout << "CRC ERROR !" << std::endl;
						return -1;
					}
					}else{
					//std::cout << "CRC/MSG ERROR END OF MSG MALFORMED" << std::endl;
					return -1;
				}
			}
			}else{
			//std::cout << "CRC/MSG ERROR" << std::endl;
			return -1;
		}

		pCursor += 4; // @TODO CRC
		//std::cout << *pCursor << std::endl;
		if(*pCursor == '5'){
			//Msg is P1
			++pCursor; // @TODO CRC
			}else{
			//std::cout << "FIRST RETURN ERROR" << std::endl;
			return -1;
		}
		}else{
		//std::cout << "SECOND RETURN ERROR - NOT A P1 MSG" << std::endl;
		return -1;
	}
	
	//Ident
	const char * identStart = pCursor;
	//std::cout << "P1 Telegram Ident start char : " << *identStart << std::endl;
	
	//Ident lenght
	int identLenght = 0;
	while(*pCursor != '\r'){
		++identLenght;
		++pCursor; // @TODO CRC
	}
	char * testIdent = new char[identLenght];
	
	strncpy(testIdent, identStart, identLenght);
	testIdent[identLenght] = '\0';

	//Ident is device name on channel 0
	
	//std::cout << "P1 Telegram Ident: " << testIdent << std::endl;
	//std::cout << "P1 Telegram Ident lenght: " << identLenght << std::endl;

	//std::cout << "pCurser is : " << *pCursor << std::endl;
	
	//crlfcrlf
	pCursor += 4;

	int OBISChannelNumber = 0;

	//parse first exception line
	if((*pCursor == '1') && (*(pCursor + 1)  == '-') && (*(pCursor + 2)  == '3')){
		//std::cout << "P1 Special telegram version: " << std::endl;
		pCursor += 4;
		//std::cout << "pCursor: " << *pCursor << std::endl;
		//OBIS parse 255.255.255
		//int oC = 0;
		
		//const char * OBISstart = pCursor;

		int obisCodeSec1 = OBIScodeSectionToInt(pCursor);
		int obisCodeSec2 = OBIScodeSectionToInt(pCursor);
		int obisCodeSec3 = OBIScodeSectionToInt(pCursor);

		//uint32_t switchCode = (obisCodeSec1 << 16) | (obisCodeSec2 << 8) | (obisCodeSec3 << 0);
		
		//Channel is
		OBISChannelNumber = 0;

		//std::cout << "OBIS Channel is: " << OBISChannelNumber << std::endl;//For 1-3 hardcoded channel should be 0
		//Categorie is
		//channelList.remove(0);
		//OBISChannel *tester = new OBISChannel(0);
		//channelList.push_back(tester);

		OBISChannel * tester = p1C.getOBISChannel(OBISChannelNumber);

		OBISAddObjectToChannel(tester, pCursor, obisCodeSec1, obisCodeSec2, obisCodeSec3);

		}else{
		return -1;
	}

	//at this point regular OBIS object can be added
	OBISMoveCursorNextLine(pCursor);
	
	int countLimit = 0;
	
	while(*pCursor !='!' || *pCursor == '\0'){
		//get channel
		pCursor += 2;

		// for (int i = 0; i < 20; i++){
		//     if(*pCursor != '\0'){
		//         //std::cout << *(pCursor + i) ;
		//     }
		// }
		// //std::cout << std::endl;

		OBISChannel * channel = p1C.getOBISChannel(OBIScodeSectionToInt(pCursor));

		////std::cout << "Current Channel in use: " << tester->getChannelNumber() << std::endl;
		int obisCodeSec1 = OBIScodeSectionToInt(pCursor);
		int obisCodeSec2 = OBIScodeSectionToInt(pCursor);
		int obisCodeSec3 = OBIScodeSectionToInt(pCursor);
		
		OBISAddObjectToChannel(channel, pCursor, obisCodeSec1, obisCodeSec2, obisCodeSec3);

		OBISMoveCursorNextLine(pCursor);
		
		countLimit++;
		
		if(countLimit >= 40){
			break;
		}
	}


	//std::cout << "P1 After Telegram Ident: " << std::endl;
	for (int i = 0; i < 20; i++){
		if(*pCursor != '\0'){
			//std::cout << *pCursor++ ;
		}
	}
	//std::cout << std::endl;
	
	//First message is an exception to the DSMR protocol
	//1-3:0.2.8(50) ... not on channel three bus general information on the DSMR protocol, 50 denoting version 5

	return 0;
}

int P1Decoder::OBISMoveCursorNextLine(const char * &startSec){
	while ((*startSec != '\n') && (*startSec != '!') && (*startSec != '\0')){
		++startSec;
	}

	if(*startSec == '\n'){
		++startSec;
	}

	return 0;
}

int P1Decoder::OBIScodeSectionToInt(const char * &startSec){
	//end of obis sector is '.' or '('
	int val = 0;

	//while((*startSec != '.') && (*startSec != '(')){
	while((*startSec >= '0') && (*startSec <= '9')){   //This will delemit on non digit
		val = val * 10 + (*startSec - '0');
		++startSec;
	}

	++startSec; //skip past delimination char

	return val;
}

int P1Decoder::OBISStringToIntLenght(const char * &startSec, unsigned int n){
	//end of obis sector is '.' or '('
	int val = 0;

	unsigned int count = 0;

	while((*startSec >= '0') && (*startSec <= '9')){
		if(count >=n ){
			break;
		}
		++count;

		val = val * 10 + (*startSec - '0');
		++startSec;
	}

	return val;
}

constexpr int32_t OBISBitMask(int i1, int i2, int i3){
	return (i1 << 16) | (i2 << 8) | (i3 << 0);
}

int P1Decoder::OBISAddObjectToChannel(OBISChannel * oChannel, const char * &pCursor, int obisCodeSec1, int obisCodeSec2, int obisCodeSec3){
	//should be 0.2.8
	int32_t bitMask = (obisCodeSec1 << 16) | (obisCodeSec2 << 8) | (obisCodeSec3 << 0);
	//std::cout << "BITMASK: " << bitMask << std::endl;

	OBISObject * obj;

	switch (bitMask){
		case OBISBitMask(0,2,8):{
			//std::cout << "TESTING 0.2.8" << std::endl;
			obj = new OBISObjectFloatingDecimal<int>(OBISType::Version, std::stold(pCursor, nullptr), "%i");
			oChannel->addOBISObject(obj);
			//pCursor += 2;
			break;
		}
		case OBISBitMask(1,0,0):{
			//std::cout << "TESTING 1.0.0" << std::endl;
			obj = new OBISObjectTST(OBISType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2));
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(1,7,0):{
			//std::cout << "TESTING 1.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::PDelivered, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(1,8,1):{
			//std::cout << "TESTING 1.8.1" << std::endl;

			obj = new OBISObjectFloatingDecimal<double>(OBISType::EDeliveredToClientT1, std::stold(pCursor, nullptr), "%0.3f", "kWh"); //should be long double, long double to_string support is broken in my compiler
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(1,8,2):{
			//std::cout << "TESTING 1.8.2" << std::endl;

			obj = new OBISObjectFloatingDecimal<double>(OBISType::EDeliveredToClientT2, std::stold(pCursor, nullptr), "%0.3f", "kWh");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(2,7,0):{
			//std::cout << "TESTING 2.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::PReceived, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(2,8,1):{
			//std::cout << "TESTING 2.8.1" << std::endl;

			obj = new OBISObjectFloatingDecimal<double>(OBISType::EDeliveredByClientT1, std::stold(pCursor, nullptr), "%0.3f", "kWh");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(2,8,2):{
			//std::cout << "TESTING 2.8.2" << std::endl;

			obj = new OBISObjectFloatingDecimal<double>(OBISType::EDeliveredByClientT2, std::stold(pCursor, nullptr), "%0.3f", "kWh");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(21,7,0):{
			//std::cout << "TESTING 21.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::InsActivePowerL1P, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(22,7,0):{
			//std::cout << "TESTING 22.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::InsActivePowerL1N, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(24,1,0):{
			//std::cout << "TESTING 24.1.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned int>(OBISType::DeviceType, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(24,2,1):{
			//std::cout << "TESTING 24.2.1" << std::endl;
			//24.2.1(170124210000W)(00671.790*m3)

			obj = new OBISPowerFailureEventLog(OBISType::GasDeliveredLog);
			
			if((*pCursor == '\n') && (*pCursor == '\0')){
				//ERROR STATE IN PARSE
				//std::cout << "ERROR IN LOG MSG PARSE" << std::endl;
				break;
			}

			OBISObjectTST * objT = new OBISObjectTST(OBISType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2));
			pCursor +=3;
			OBISObjectFloatingDecimal<double> * objF = new OBISObjectFloatingDecimal<double>(OBISType::GasDelivered, std::stold(pCursor, nullptr), "%0.3f", "m3");
			static_cast<OBISPowerFailureEventLog*>(obj)->addOBISLog(objT, objF);
			////std::cout << obj->print() << obj2->print() << std::endl;

			oChannel->addOBISObject(obj);

			break;
		}
		case OBISBitMask(31,7,0):{
			//std::cout << "TESTING 31.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::ICurrentInL1, std::stold(pCursor, nullptr), "%u", "A");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(32,7,0):{
			//std::cout << "TESTING 32.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::IVoltInL1, std::stold(pCursor, nullptr), "%0.1f", "V");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(32,32,0):{
			//std::cout << "TESTING 32.32.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NVoltageSagsPL1, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(32,36,0):{
			//std::cout << "TESTING 32.36.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NVoltageSwellsPL1, std::stold(pCursor, nullptr),"%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(41,7,0):{
			//std::cout << "TESTING 41.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::InsActivePowerL2P, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(42,7,0):{
			//std::cout << "TESTING 42.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::InsActivePowerL2N, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(51,7,0):{
			//std::cout << "TESTING 51.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::ICurrentInL2, std::stold(pCursor, nullptr), "%u", "A");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(52,7,0):{
			//std::cout << "TESTING 52.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::IVoltInL2, std::stold(pCursor, nullptr), "%0.1f", "V");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(52,32,0):{
			//std::cout << "TESTING 52.32.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NVoltageSagsPL2, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(52,36,0):{
			//std::cout << "TESTING 52.36.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NVoltageSwellsPL2, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(61,7,0):{
			//std::cout << "TESTING 61.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::InsActivePowerL3P, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(62,7,0):{
			//std::cout << "TESTING 62.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::InsActivePowerL3N, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(71,7,0):{
			//std::cout << "TESTING 71.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::ICurrentInL3, std::stold(pCursor, nullptr), "%u", "A");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(72,7,0):{
			//std::cout << "TESTING 72.7.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<double>(OBISType::IVoltInL3, std::stold(pCursor, nullptr), "%0.1f", "V");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(72,32,0):{
			//std::cout << "TESTING 72.32.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NVoltageSagsPL3, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(72,36,0):{
			//std::cout << "TESTING 72.36.0" << std::endl;
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NVoltageSwellsPL3, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(96,1,0):{
			//std::cout << "TESTING 96.1.0" << std::endl;

			char hexBuffer[3];
			char cBuf;
			std::string convertedBuf;

			while((*pCursor != ')') && (*pCursor != '\0')){
				std::strncpy(hexBuffer, pCursor, 2);
				hexBuffer[2] = '\0';
				cBuf = std::stoul(hexBuffer, nullptr, 16);
				convertedBuf += cBuf;
				pCursor += 2;
			}
			
			obj = new OBISObjectString(OBISType::EquipmentIdentifierGas, convertedBuf);

			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(96,1,1):{

			char hexBuffer[3];
			char cBuf;
			std::string convertedBuf;

			while((*pCursor != ')') && (*pCursor != '\0')){
				std::strncpy(hexBuffer, pCursor, 2);
				hexBuffer[2] = '\0';
				cBuf = std::stoul(hexBuffer, nullptr, 16);
				convertedBuf += cBuf;
				pCursor += 2;
			}
			
			obj = new OBISObjectString(OBISType::Equipment_identifier, convertedBuf);

			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(96,7,9):{
			//std::cout << "TESTING 96.7.9" << std::endl;
			//Number of long power failures in any phase
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NLongPowerFailuresInPhase, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(96,7,21):{
			//std::cout << "TESTING 96.7.21" << std::endl;
			//Number of power failures in any phase
			obj = new OBISObjectFloatingDecimal<unsigned long>(OBISType::NPowerFailuresInPhase, std::stold(pCursor, nullptr), "%u");
			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(96,13,0):{
			//std::cout << "TESTING 96.13.0" << std::endl;
			char hexBuffer[3];
			char cBuf;
			std::string convertedBuf;

			while((*pCursor != ')') && (*pCursor != '\0')){
				std::strncpy(hexBuffer, pCursor, 2);
				hexBuffer[2] = '\0';
				cBuf = std::stoul(hexBuffer, nullptr, 16);
				convertedBuf += cBuf;
				pCursor += 2;
			}
			
			obj = new OBISObjectString(OBISType::TextMessage, convertedBuf);


			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(96,14,0):{
			// char hexBuffer[3];
			// char cBuf;
			// std::string convertedBuf;

			// while((*pCursor != ')') && (*pCursor != '\0')){
			//     std::strncpy(hexBuffer, pCursor, 2);
			//     hexBuffer[2] = '\0';
			//     cBuf = std::stoul(hexBuffer, nullptr, 16);
			//     convertedBuf += cBuf;
			//     pCursor += 2;
			// }

			//This is implemented wrongfully implemented in P1, just convert to int
			
			//OBISObject * obj = new OBISObjectString(OBISType::TariffIndicator, convertedBuf);

			obj = new OBISObjectFloatingDecimal<int>(OBISType::TariffIndicator, strtol(pCursor, nullptr, 10), "%i");


			oChannel->addOBISObject(obj);
			break;
		}
		case OBISBitMask(99,97,0):{
			obj = new OBISPowerFailureEventLog(OBISType::PowerFailureEventLog);
			
			int nLogCount = OBIScodeSectionToInt(pCursor);
			++pCursor;

			for (int i = 0; i < nLogCount; i++){
				//move pCursor to next section
				while ((*pCursor != ')') && (*pCursor != '\n') && (*pCursor != '\0')){
					++pCursor;
				}
				
				if((*pCursor == '\n') && (*pCursor == '\0')){
					//ERROR STATE IN PARSE
					break;
				}
				
				pCursor +=2;

				
				//std::cout << "ADDING NEW LOG ITEM: " << std::endl;

				OBISObjectTST * objT = new OBISObjectTST(OBISType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2));
				pCursor +=3;
				OBISObjectFloatingDecimal<int> * objF = new OBISObjectFloatingDecimal<int>(OBISType::PowerFailureEventLogTime, OBIScodeSectionToInt(pCursor), "%i", "s");
				static_cast<OBISPowerFailureEventLog*>(obj)->addOBISLog(objT, objF);
			}

			oChannel->addOBISObject(obj);

			break;
		}
		default:
		break;
	}
	return 0;
}

// default destructor
P1Decoder::~P1Decoder(){

	if(!OBISChannelList.empty()){
		std::list<OBISChannel*>::iterator it;
		for(it = OBISChannelList.begin(); it != OBISChannelList.end(); ++it){
			delete *it;
		}
	}
} //~P1Decoder
