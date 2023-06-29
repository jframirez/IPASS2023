/*
* P1Decoder.cpp
*
* Created: 4-6-2023 18:22:03
* Author: Jordan
*/


#include "P1Decoder.h"

//String encoded for PC testing purpose
//testP1Telegram is the example telegram from P1 compendium document,
//The example is wrong in this document, this is the corrected version.
const char * testP1Telegram = "/ISk5\\2MT382-1000\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(101209113020W)\r\n0-0:96.1.1(4B384547303034303436333935353037)\r\n1-0:1.8.1(123456.789*kWh)\r\n1-0:1.8.2(123456.789*kWh)\r\n1-0:2.8.1(123456.789*kWh)\r\n1-0:2.8.2(123456.789*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(01.193*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00004)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\r\n1-0:32.32.0(00002)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00003)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)\r\n1-0:32.7.0(220.1*V)\r\n1-0:52.7.0(220.2*V)\r\n1-0:72.7.0(220.3*V)\r\n1-0:31.7.0(001*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(003*A)\r\n1-0:21.7.0(01.111*kW)\r\n1-0:41.7.0(02.222*kW)\r\n1-0:61.7.0(03.333*kW)\r\n1-0:22.7.0(04.444*kW)\r\n1-0:42.7.0(05.555*kW)\r\n1-0:62.7.0(06.666*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(3232323241424344313233343536373839)\r\n0-1:24.2.1(101209112500W)(12785.123*m3)\r\n!E47C";

const char * testP1Telegram2 = "/KFM5KAIFA-METER\r\n\r\n1-3:0.2.8(42)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.8.1(000306.946*kWh)\r\n1-0:1.8.2(000210.088*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(02.793*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00001)\r\n0-0:96.7.9(00001)\r\n1-0:99.97.0(1)(0-0:96.7.19)(000101000006W)(2147483647*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00000)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(003*A)\r\n1-0:51.7.0(005*A)\r\n1-0:71.7.0(005*A)\r\n1-0:21.7.0(00.503*kW)\r\n1-0:41.7.0(01.100*kW)\r\n1-0:61.7.0(01.190*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303331303033333738373931363136)\r\n0-1:24.2.1(170124210000W)(00671.790*m3)\r\n!29ED";

const char * testP1Telegram3 = "/JFR5SIM-P1-METER\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.8.1(000411.141*kWh)\r\n!0431";

const char * testP1Telegram4 = "/JFR5SIM-P1-METER\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.7.0(000482.745*kWh)\r\n!1793";

const char * testP1TelegramCapture = "/ISK5\\2M550E-1012\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(230615210817S)\r\n0-0:96.1.1(4530303433303037343231343730323139)\r\n1-0:1.8.1(008378.196*kWh)\r\n1-0:1.8.2(004019.445*kWh)\r\n1-0:2.8.1(000499.523*kWh)\r\n1-0:2.8.2(001403.870*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(00.368*kW)\r\n1-0:2.7.0(00.128*kW)\r\n0-0:96.7.21(00008)\r\n0-0:96.7.9(00005)\r\n1-0:99.97.0(3)(0-0:96.7.19)(190514051940S)(0000000554*s)(200924114954S)(0000008885*s)(211208115451W)(0000011472*s)\r\n1-0:32.32.0(00007)\r\n1-0:32.36.0(00001)\r\n0-0:96.13.0()\r\n1-0:32.7.0(238.2*V)\r\n1-0:31.7.0(002*A)\r\n1-0:21.7.0(00.368*kW)\r\n1-0:22.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303339303031393335343838303139)\r\n0-1:24.2.1(230615210459S)(04490.555*m3)\r\n!8CFC";

const char * testP1TelegramCaptureError = "/ISK5\\2M550E-1012\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(230616144327S)\r\n0-0:96.1.1(4530303433303037343231343730323139)\r\n1-0:1.8.1(008382.642*kWh)\r\n1-0:1.8.2(004019.737*kWh)\r\n1-0:2.8.1(000499.523*kWh)\r\n1-0:2.8.2(001406.900*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(00.000*kW)\r\n1-0:2.7.0(00.499*kW)\r\n0-0:96.7.21(00008)\r\n0-0:96.7.9(00005)\r\n1-0:99.97.0(3)(0-0:96.7.19)(190514051940S)(0000000554*s)(200924114954S)(0000008885*s)(211208115451W)(0000011472*s)\r\n1-0:32.32.0(00007)\r\n1-0:32.36.0(00001)\r\n0-0:96.13.0()\r\n1-0:32.7.0(238.6*V)\r\n1-0:31.7.0(002*A)\r\n1-0:21.7.0(00.000*kW)\r\n1-0:22.7.0(00.493*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303339303031393335343838303139)\r\n0-1:24.2.1(230616144002S)(04490.851*m3)\r\n!B1F2\r\n";
// default constructor
P1Decoder::P1Decoder(){
	
} //P1Decoder

std::list<OBISChannel*> P1Decoder::getAllOBISChannels(){
	return OBISChannelList;
}

OBISChannel * P1Decoder::getOBISChannel(unsigned int n){
	//Add unq channel number
	for(const auto &Optr: OBISChannelList){
		if (Optr->getChannelNumber() == n){
			////std::cout << "OBISChannel Collision, return existing channel" << std::endl;
			return Optr;
		}
	}

	OBISChannel * Channel = new OBISChannel(n);
	OBISChannelList.push_back(Channel);

	return Channel;
}

void P1Decoder::removeOBISChannel(OBISChannel * n){
	for(const auto &Optr: OBISChannelList){
		if (Optr->getChannelNumber() == n->getChannelNumber()){
			//std::cout << "OBISObject Collision" << std::endl;
			OBISChannelList.remove(Optr);
			delete Optr;
			break;
		}
	}
}

void P1Decoder::clearAllOBISChannel(){
	std::list<OBISChannel*>::iterator it = OBISChannelList.begin();

	while(it != OBISChannelList.end()){
		OBISChannel * prevP = (*it);
		it = OBISChannelList.erase(it);
		delete prevP;
	}
}

std::string P1Decoder::getDeviceIdentifier(){
	return deviceIdentifier;
}

void P1Decoder::setDeviceIdentifier(std::string id){
	deviceIdentifier = id;
}


int P1Decoder::decodeP1(const char * msgStart, P1Decoder & p1C){
	
	//Return -1 -> msg is not a P1 Telegram
	//Return -2 -> CRC error
	//Return -3 -> Generic parsing error
	//Return 0 -> Succes
	//Return 1 -> Return pointer available in recent msg
	
	const char * pCursor = msgStart;

	if(*pCursor == '/'){ //CRC skip /
		//Chekc crc
		CrcReturn CRCout = CrcP1::CrcGen(msgStart, '!');
		if(CRCout.crcErrorCode == 0){
			if(pCursor[CRCout.crcByteLenght] == '!'){
				//Make sure 5th next symbol = \r else error
				if((pCursor[CRCout.crcByteLenght + 5] == '\0') | (pCursor[CRCout.crcByteLenght + 5] == '\r') | (pCursor[CRCout.crcByteLenght + 5] == '\n')){
					if(std::stoi(pCursor + (CRCout.crcByteLenght + 1), nullptr, 16) != CRCout.crcValue){
						return -1;
					}
					}else{
					return -2;
				}
			}
		}else{
			return -3;
		}

		pCursor += 4; // @TODO CRC

		if(*pCursor == '5'){
			//Msg is P1
			++pCursor; // @TODO CRC
			}else{
			return -4;
		}
	}else{
		return -5;
	}
	
	std::string deviceIdentifier = "";

	while(*pCursor != '\r'){
		p1C.deviceIdentifier += *pCursor;
		++pCursor;
	}

	p1C.setDeviceIdentifier(deviceIdentifier);
	
	//Ident is device name for P1 device

	pCursor += 4;

	int OBISChannelNumber = 0;

	//parse first exception line
	if((*pCursor == '1') && (*(pCursor + 1)  == '-') && (*(pCursor + 2)  == '3')){
		pCursor += 4;

		int obisCodeSec1 = OBISCodeSectionToInt(pCursor);
		int obisCodeSec2 = OBISCodeSectionToInt(pCursor);
		int obisCodeSec3 = OBISCodeSectionToInt(pCursor);
		
		//Channel is
		OBISChannelNumber = 0;

		OBISChannel * tester = p1C.getOBISChannel(OBISChannelNumber);

		OBISAddObjectToChannel(tester, pCursor, obisCodeSec1, obisCodeSec2, obisCodeSec3);

		}else{
		return -6;
	}

	//at this point regular OBIS object can be added
	OBISMoveCursorNextLine(pCursor);
	
	int countLimit = 0;
	
	while(*pCursor !='!' || *pCursor == '\0'){
		pCursor += 2;

		OBISChannel * channel = p1C.getOBISChannel(OBISCodeSectionToInt(pCursor));

		int obisCodeSec1 = OBISCodeSectionToInt(pCursor);
		int obisCodeSec2 = OBISCodeSectionToInt(pCursor);
		int obisCodeSec3 = OBISCodeSectionToInt(pCursor);
		
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

int P1Decoder::OBISCodeSectionToInt(const char * &startSec){
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
			
			int nLogCount = OBISCodeSectionToInt(pCursor);
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

				OBISObjectTST * objT = new OBISObjectTST(OBISType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2));
				pCursor +=3;
				OBISObjectFloatingDecimal<int> * objF = new OBISObjectFloatingDecimal<int>(OBISType::PowerFailureEventLogTime, OBISCodeSectionToInt(pCursor), "%i", "s");
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
