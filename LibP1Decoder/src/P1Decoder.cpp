/*
* P1Decoder.cpp
*
* Created: 4-6-2023 18:22:03
* Author: Jordan
*/


#include "P1Decoder.h"


// default constructor
P1Decoder::P1Decoder(){
	
} //P1Decoder

std::list<CosemChannel*> P1Decoder::getAllCosemChannels(){
	return CosemChannelList;
}

CosemChannel * P1Decoder::addCosemChannel(unsigned int n){
	//Add unq channel number
	for(const auto &Optr: CosemChannelList){
		if (Optr->getChannelNumber() == n){
			return Optr;
		}
	}

	CosemChannel * Channel = new CosemChannel(n);
	CosemChannelList.push_back(Channel);

	return Channel;
}

CosemChannel * P1Decoder::getCosemChannel(unsigned int n){
	for(const auto &oPtr: CosemChannelList){
		if (oPtr->getChannelNumber() == n){
			return oPtr;
		}
	}
	return nullptr;
}

std::string P1Decoder::getCosemStringFromChannel(unsigned int channelNumber, ObisType type){
	CosemChannel *const & channel = getCosemChannel(channelNumber);

	if(channel != nullptr){
		for(auto &oPtr: channel->getCosemObjectList()){
			if(oPtr->getType() == type){
				return oPtr->print();
			}
		}
	}
	return std::string("NaN");
}

std::string P1Decoder::getDeltaString(unsigned int channelNumber, ObisType type1, ObisType type2){
	CosemObject * deltaP1 = nullptr;
	CosemObject * deltaP2 = nullptr;

	CosemChannel *const & channel1 = getCosemChannel(channelNumber);

	if(channel1){
		for(auto &oPtr: channel1->getCosemObjectList()){
			if(oPtr->getType() == type1){
				deltaP1 = oPtr;
			}
			if(oPtr->getType() == type2){
				deltaP2 = oPtr;
			}
		}
		
		if(deltaP1){
			return deltaP1->getDelta(deltaP2);
		}
	}
	
	return std::string("NaN");
	
}

unsigned int P1Decoder::getCosemChannelSize(unsigned int n){
	auto cN = getCosemChannel(n);
	if(cN != nullptr){
		return cN->getChannelSize();
	}
	return 0;
}

unsigned int P1Decoder::getCosemChannelCount(){
	return CosemChannelList.size();
}

void P1Decoder::removeCosemChannel(CosemChannel * n){
	for(const auto &oPtr: CosemChannelList){
		if (oPtr->getChannelNumber() == n->getChannelNumber()){
			CosemChannelList.remove(oPtr);
			delete oPtr;
			break;
		}
	}
}

void P1Decoder::clearAllCosemChannels(){
	std::list<CosemChannel*>::iterator it = CosemChannelList.begin();

	while(it != CosemChannelList.end()){
		CosemChannel * prevP = (*it);
		it = CosemChannelList.erase(it);
		delete prevP;
	}
}

std::string P1Decoder::getDeviceIdentifier(){
	return deviceIdentifier;
}

void P1Decoder::setDeviceIdentifier(std::string id){
	deviceIdentifier = id;
}

int P1Decoder::decodeP1String(const char * msgStart){
	return decodeP1(msgStart, *this);
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

		pCursor += 4;

		if(*pCursor == '5'){
			//Msg is P1
			++pCursor;
			}else{
			return -4;
		}
		}else{
		return -5;
	}
	
	std::string devIdentifier = "";

	while(*pCursor != '\r'){
		devIdentifier += *pCursor;
		++pCursor;
	}

	p1C.setDeviceIdentifier(devIdentifier);

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

		CosemChannel * tester = p1C.addCosemChannel(OBISChannelNumber);

		OBISAddObjectToChannel(tester, pCursor, obisCodeSec1, obisCodeSec2, obisCodeSec3);

		}else{
		return -6;
	}

	//at this point regular OBIS object can be added
	OBISMoveCursorNextLine(pCursor);
	
	int countLimit = 0;
	
	while(*pCursor !='!' || *pCursor == '\0'){
		pCursor += 2;

		CosemChannel * channel = p1C.addCosemChannel(OBISCodeSectionToInt(pCursor));

		int obisCodeSec1 = OBISCodeSectionToInt(pCursor);
		int obisCodeSec2 = OBISCodeSectionToInt(pCursor);
		int obisCodeSec3 = OBISCodeSectionToInt(pCursor);
		
		OBISAddObjectToChannel(channel, pCursor, obisCodeSec1, obisCodeSec2, obisCodeSec3);

		OBISMoveCursorNextLine(pCursor);
		
		countLimit++;
		
		if(countLimit >= 80){
			break;
		}
	}

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

	while((*startSec >= '0') && (*startSec <= '9')){   //This will delemit on non digit
		val = val * 10 + (*startSec - '0');
		++startSec;
	}

	++startSec; //skip past delimination char

	return val;
}

int P1Decoder::OBISStringToIntLenght(const char * &startSec, unsigned int n){

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

int P1Decoder::OBISAddObjectToChannel(CosemChannel * oChannel, const char * &pCursor, int obisCodeSec1, int obisCodeSec2, int obisCodeSec3){

	int32_t bitMask = (obisCodeSec1 << 16) | (obisCodeSec2 << 8) | (obisCodeSec3 << 0);

	CosemObject * obj;

	switch (bitMask){
		case OBISBitMask(0,2,8):{
			//std::cout << "TESTING 0.2.8" << std::endl;
			obj = new CosemObjectNumber<int>(ObisType::Version, std::stold(pCursor, nullptr), "%i");
			oChannel->addCosemObject(obj);
			//pCursor += 2;
			break;
		}
		case OBISBitMask(1,0,0):{
			//std::cout << "TESTING 1.0.0" << std::endl;
			obj = new CosemObjectTst(ObisType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2));
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(1,7,0):{
			//std::cout << "TESTING 1.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::PDelivered, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(1,8,1):{
			//std::cout << "TESTING 1.8.1" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredToClientT1, std::stold(pCursor, nullptr), "%0.3f", "kWh"); //should be long double, long double to_string support is broken in my compiler
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(1,8,2):{
			//std::cout << "TESTING 1.8.2" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredToClientT2, std::stold(pCursor, nullptr), "%0.3f", "kWh");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(2,7,0):{
			//std::cout << "TESTING 2.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::PReceived, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(2,8,1):{
			//std::cout << "TESTING 2.8.1" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredByClientT1, std::stold(pCursor, nullptr), "%0.3f", "kWh");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(2,8,2):{
			//std::cout << "TESTING 2.8.2" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredByClientT2, std::stold(pCursor, nullptr), "%0.3f", "kWh");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(21,7,0):{
			//std::cout << "TESTING 21.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL1P, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(22,7,0):{
			//std::cout << "TESTING 22.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL1N, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(24,1,0):{
			//std::cout << "TESTING 24.1.0" << std::endl;
			obj = new CosemObjectNumber<unsigned int>(ObisType::DeviceType, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(24,2,1):{
			//std::cout << "TESTING 24.2.1" << std::endl;
			//24.2.1(170124210000W)(00671.790*m3)

			obj = new CosemEventLog(ObisType::GasDeliveredLog);
			
			if((*pCursor == '\n') && (*pCursor == '\0')){
				//ERROR STATE IN PARSE
				break;
			}

			CosemObjectTst * objT = new CosemObjectTst(ObisType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2),
			OBISStringToIntLenght(pCursor, 2));
			pCursor +=3;
			CosemObjectNumber<double> * objF = new CosemObjectNumber<double>(ObisType::GasDelivered, std::stold(pCursor, nullptr), "%0.3f", "m3");
			static_cast<CosemEventLog*>(obj)->addEventLog(objT, objF);

			oChannel->addCosemObject(obj);

			break;
		}
		case OBISBitMask(31,7,0):{
			//std::cout << "TESTING 31.7.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::ICurrentInL1, std::stold(pCursor, nullptr), "%u", "A");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(32,7,0):{
			//std::cout << "TESTING 32.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::IVoltInL1, std::stold(pCursor, nullptr), "%0.1f", "V");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(32,32,0):{
			//std::cout << "TESTING 32.32.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSagsPL1, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(32,36,0):{
			//std::cout << "TESTING 32.36.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSwellsPL1, std::stold(pCursor, nullptr),"%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(41,7,0):{
			//std::cout << "TESTING 41.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL2P, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(42,7,0):{
			//std::cout << "TESTING 42.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL2N, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(51,7,0):{
			//std::cout << "TESTING 51.7.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::ICurrentInL2, std::stold(pCursor, nullptr), "%u", "A");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(52,7,0):{
			//std::cout << "TESTING 52.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::IVoltInL2, std::stold(pCursor, nullptr), "%0.1f", "V");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(52,32,0):{
			//std::cout << "TESTING 52.32.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSagsPL2, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(52,36,0):{
			//std::cout << "TESTING 52.36.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSwellsPL2, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(61,7,0):{
			//std::cout << "TESTING 61.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL3P, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(62,7,0):{
			//std::cout << "TESTING 62.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL3N, std::stold(pCursor, nullptr), "%0.3f", "kW");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(71,7,0):{
			//std::cout << "TESTING 71.7.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::ICurrentInL3, std::stold(pCursor, nullptr), "%u", "A");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(72,7,0):{
			//std::cout << "TESTING 72.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::IVoltInL3, std::stold(pCursor, nullptr), "%0.1f", "V");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(72,32,0):{
			//std::cout << "TESTING 72.32.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSagsPL3, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(72,36,0):{
			//std::cout << "TESTING 72.36.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSwellsPL3, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
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
			
			obj = new CosemObjectString(ObisType::EquipmentIdentifierGas, convertedBuf);

			oChannel->addCosemObject(obj);
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
			
			obj = new CosemObjectString(ObisType::Equipment_identifier, convertedBuf);

			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(96,7,9):{
			//std::cout << "TESTING 96.7.9" << std::endl;
			//Number of long power failures in any phase
			obj = new CosemObjectNumber<unsigned long>(ObisType::NLongPowerFailuresInPhase, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(96,7,21):{
			//std::cout << "TESTING 96.7.21" << std::endl;
			//Number of power failures in any phase
			obj = new CosemObjectNumber<unsigned long>(ObisType::NPowerFailuresInPhase, std::stold(pCursor, nullptr), "%u");
			oChannel->addCosemObject(obj);
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
			
			obj = new CosemObjectString(ObisType::TextMessage, convertedBuf);

			oChannel->addCosemObject(obj);
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
			
			//CosemObject * obj = new CosemObjectString(ObisType::TariffIndicator, convertedBuf);

			obj = new CosemObjectNumber<int>(ObisType::TariffIndicator, strtol(pCursor, nullptr, 10), "%i");


			oChannel->addCosemObject(obj);
			break;
		}
		case OBISBitMask(99,97,0):{
			obj = new CosemEventLog(ObisType::PowerFailureEventLog);
			
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

				CosemObjectTst * objT = new CosemObjectTst(ObisType::Timestamp,	OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2),
				OBISStringToIntLenght(pCursor, 2));
				pCursor +=3;
				CosemObjectNumber<int> * objF = new CosemObjectNumber<int>(ObisType::PowerFailureEventLogTime, OBISCodeSectionToInt(pCursor), "%i", "s");
				static_cast<CosemEventLog*>(obj)->addEventLog(objT, objF);
			}

			oChannel->addCosemObject(obj);

			break;
		}
		default:
		break;
	}
	return 0;
}

P1Decoder::~P1Decoder(){
	if(!CosemChannelList.empty()){
		std::list<CosemChannel*>::iterator it;
		for(it = CosemChannelList.begin(); it != CosemChannelList.end(); ++it){
			delete *it;
		}
	}
} //~P1Decoder
