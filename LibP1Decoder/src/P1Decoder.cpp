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
	return cosem_channel_list;
}

CosemChannel * P1Decoder::addCosemChannel(unsigned int n_){
	//Add unique channel number
	for(const auto &o_ptr: cosem_channel_list){
		if (o_ptr->getChannelNumber() == n_){
			return o_ptr;
		}
	}

	CosemChannel * channel_ = new CosemChannel(n_);
	cosem_channel_list.push_back(channel_);

	return channel_;
}

CosemChannel * P1Decoder::getCosemChannel(unsigned int n_){
	for(const auto &o_ptr: cosem_channel_list){
		if (o_ptr->getChannelNumber() == n_){
			return o_ptr;
		}
	}
	return nullptr;
}

std::string P1Decoder::getCosemStringFromChannel(unsigned int channel_number, ObisType type_){
	CosemChannel *const & channel = getCosemChannel(channel_number);

	if(channel != nullptr){
		for(auto &o_ptr: channel->getCosemObjectList()){
			if(o_ptr->getType() == type_){
				return o_ptr->print();
			}
		}
	}
	return std::string("NaN");
}

std::string P1Decoder::getDeltaString(unsigned int channel_number, ObisType type_1, ObisType type_2){
	CosemObject * delta_p1 = nullptr;
	CosemObject * delta_p2 = nullptr;

	CosemChannel *const & channel_1 = getCosemChannel(channel_number);

	if(channel_1){
		for(auto &o_ptr: channel_1->getCosemObjectList()){
			if(o_ptr->getType() == type_1){
				delta_p1 = o_ptr;
			}
			if(o_ptr->getType() == type_2){
				delta_p2 = o_ptr;
			}
		}
		
		if(delta_p1){
			return delta_p1->getDelta(delta_p2);
		}
	}
	
	return std::string("NaN");
	
}

unsigned int P1Decoder::getCosemChannelSize(unsigned int n_){
	auto c_n = getCosemChannel(n_);
	if(c_n != nullptr){
		return c_n->getChannelSize();
	}
	return 0;
}

unsigned int P1Decoder::getCosemChannelCount(){
	return cosem_channel_list.size();
}

void P1Decoder::removeCosemChannel(CosemChannel * n_){
	for(const auto &o_ptr: cosem_channel_list){
		if (o_ptr->getChannelNumber() == n_->getChannelNumber()){
			cosem_channel_list.remove(o_ptr);
			delete o_ptr;
			break;
		}
	}
}

void P1Decoder::clearAllCosemChannels(){
	std::list<CosemChannel*>::iterator it_ = cosem_channel_list.begin();

	while(it_ != cosem_channel_list.end()){
		CosemChannel * prev_p = (*it_);
		it_ = cosem_channel_list.erase(it_);
		delete prev_p;
	}
}

std::string P1Decoder::getDeviceIdentifier(){
	return device_identifier;
}

void P1Decoder::setDeviceIdentifier(std::string id_){
	device_identifier = id_;
}

int P1Decoder::decodeP1String(const char * msg_start){
	return decodeP1(msg_start, *this);
}


int P1Decoder::decodeP1(const char * msg_start, P1Decoder & p1_d){
	
	//Return -1 -> msg is not a P1 Telegram
	//Return -2 -> CRC error
	//Return -3 -> Generic parsing error
	//Return 0 -> Succes
	//Return 1 -> Return pointer available in recent msg
	
	const char * p_cursor = msg_start;

	if(*p_cursor == '/'){ //CRC skip /
		//Chekc crc
		CrcReturn CRCout = CrcP1::CrcGen(msg_start, '!');
		if(CRCout.crcErrorCode == 0){
			if(p_cursor[CRCout.crcByteLenght] == '!'){
				//Make sure 5th next symbol = \r else error
				if((p_cursor[CRCout.crcByteLenght + 5] == '\0') | (p_cursor[CRCout.crcByteLenght + 5] == '\r') | (p_cursor[CRCout.crcByteLenght + 5] == '\n')){
					if(std::stoi(p_cursor + (CRCout.crcByteLenght + 1), nullptr, 16) != CRCout.crcValue){
						return -1;
					}
					}else{
					return -2;
				}
			}
			}else{
			return -3;
		}

		p_cursor += 4;

		if(*p_cursor == '5'){
			//Msg is P1
			++p_cursor;
			}else{
			return -4;
		}
		}else{
		return -5;
	}
	
	std::string dev_identifier = "";

	while(*p_cursor != '\r'){
		dev_identifier += *p_cursor;
		++p_cursor;
	}

	p1_d.setDeviceIdentifier(dev_identifier);

	p_cursor += 4;

	int obis_channel_number = 0;

	//parse first exception line
	if((*p_cursor == '1') && (*(p_cursor + 1)  == '-') && (*(p_cursor + 2)  == '3')){
		p_cursor += 4;

		int obis_code_sec_1 = ObisCodeSectionToInt(p_cursor);
		int obis_code_sec_2 = ObisCodeSectionToInt(p_cursor);
		int obis_code_sec_3 = ObisCodeSectionToInt(p_cursor);
		
		//Channel is
		obis_channel_number = 0;

		CosemChannel * tester_ = p1_d.addCosemChannel(obis_channel_number);

		ObisAddObjectToChannel(tester_, p_cursor, obis_code_sec_1, obis_code_sec_2, obis_code_sec_3);

		}else{
		return -6;
	}

	//at this point regular OBIS object can be added
	ObisMoveCursorNextLine(p_cursor);
	
	int count_limit = 0;
	
	while(*p_cursor !='!' || *p_cursor == '\0'){
		p_cursor += 2;

		CosemChannel * channel = p1_d.addCosemChannel(ObisCodeSectionToInt(p_cursor));

		int obisCodeSec1 = ObisCodeSectionToInt(p_cursor);
		int obisCodeSec2 = ObisCodeSectionToInt(p_cursor);
		int obisCodeSec3 = ObisCodeSectionToInt(p_cursor);
		
		ObisAddObjectToChannel(channel, p_cursor, obisCodeSec1, obisCodeSec2, obisCodeSec3);

		ObisMoveCursorNextLine(p_cursor);
		
		count_limit++;
		
		if(count_limit >= 80){
			break;
		}
	}

	return 0;
}

void P1Decoder::ObisMoveCursorNextLine(const char * &start_sec){
	while ((*start_sec != '\n') && (*start_sec != '!') && (*start_sec != '\0')){
		++start_sec;
	}

	if(*start_sec == '\n'){
		++start_sec;
	}

}

int P1Decoder::ObisCodeSectionToInt(const char * &start_sec){
	//end of obis sector is '.' or '('
	int val_ = 0;

	while((*start_sec >= '0') && (*start_sec <= '9')){   //This will delimit on non digit
		val_ = val_ * 10 + (*start_sec - '0');
		++start_sec;
	}

	++start_sec; //skip past delimination char

	return val_;
}

int P1Decoder::ObisStringToIntLenght(const char * &start_sec, unsigned int n_){

	int val = 0;

	unsigned int count = 0;

	while((*start_sec >= '0') && (*start_sec <= '9')){
		if(count >=n_ ){
			break;
		}
		++count;

		val = val * 10 + (*start_sec - '0');
		++start_sec;
	}

	return val;
}

constexpr int32_t ObisBitMask(int i_1, int i_2, int i_3){
	return (i_1 << 16) | (i_2 << 8) | (i_3 << 0);
}

int P1Decoder::ObisAddObjectToChannel(CosemChannel * o_channel, const char * &p_cursor, int obis_sec_1, int obis_sec_2, int obis_sec_3){

	int32_t bit_mask = (obis_sec_1 << 16) | (obis_sec_2 << 8) | (obis_sec_3 << 0);

	CosemObject * obj;

	switch (bit_mask){
		case ObisBitMask(0,2,8):{
			//std::cout << "TESTING 0.2.8" << std::endl;
			obj = new CosemObjectNumber<int>(ObisType::Version, std::stold(p_cursor, nullptr), "%i");
			o_channel->addCosemObject(obj);
			//p_cursor += 2;
			break;
		}
		case ObisBitMask(1,0,0):{
			//std::cout << "TESTING 1.0.0" << std::endl;
			obj = new CosemObjectTst(ObisType::Timestamp,	ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2));
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(1,7,0):{
			//std::cout << "TESTING 1.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::PDelivered, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(1,8,1):{
			//std::cout << "TESTING 1.8.1" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredToClientT1, std::stold(p_cursor, nullptr), "%0.3f", "kWh"); //should be long double, long double to_string support is broken in my compiler
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(1,8,2):{
			//std::cout << "TESTING 1.8.2" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredToClientT2, std::stold(p_cursor, nullptr), "%0.3f", "kWh");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(2,7,0):{
			//std::cout << "TESTING 2.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::PReceived, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(2,8,1):{
			//std::cout << "TESTING 2.8.1" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredByClientT1, std::stold(p_cursor, nullptr), "%0.3f", "kWh");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(2,8,2):{
			//std::cout << "TESTING 2.8.2" << std::endl;

			obj = new CosemObjectNumber<double>(ObisType::EDeliveredByClientT2, std::stold(p_cursor, nullptr), "%0.3f", "kWh");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(21,7,0):{
			//std::cout << "TESTING 21.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL1P, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(22,7,0):{
			//std::cout << "TESTING 22.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL1N, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(24,1,0):{
			//std::cout << "TESTING 24.1.0" << std::endl;
			obj = new CosemObjectNumber<unsigned int>(ObisType::DeviceType, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(24,2,1):{
			//std::cout << "TESTING 24.2.1" << std::endl;
			//24.2.1(170124210000W)(00671.790*m3)

			obj = new CosemEventLog(ObisType::GasDeliveredLog);
			
			if((*p_cursor == '\n') && (*p_cursor == '\0')){
				//ERROR STATE IN PARSE
				break;
			}

			CosemObjectTst * obj_t = new CosemObjectTst(ObisType::Timestamp,	ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2),
			ObisStringToIntLenght(p_cursor, 2));
			p_cursor +=3;
			CosemObjectNumber<double> * obj_f = new CosemObjectNumber<double>(ObisType::GasDelivered, std::stold(p_cursor, nullptr), "%0.3f", "m3");
			static_cast<CosemEventLog*>(obj)->addEventLog(obj_t, obj_f);

			o_channel->addCosemObject(obj);

			break;
		}
		case ObisBitMask(31,7,0):{
			//std::cout << "TESTING 31.7.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::ICurrentInL1, std::stold(p_cursor, nullptr), "%u", "A");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(32,7,0):{
			//std::cout << "TESTING 32.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::IVoltInL1, std::stold(p_cursor, nullptr), "%0.1f", "V");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(32,32,0):{
			//std::cout << "TESTING 32.32.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSagsPL1, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(32,36,0):{
			//std::cout << "TESTING 32.36.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSwellsPL1, std::stold(p_cursor, nullptr),"%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(41,7,0):{
			//std::cout << "TESTING 41.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL2P, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(42,7,0):{
			//std::cout << "TESTING 42.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL2N, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(51,7,0):{
			//std::cout << "TESTING 51.7.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::ICurrentInL2, std::stold(p_cursor, nullptr), "%u", "A");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(52,7,0):{
			//std::cout << "TESTING 52.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::IVoltInL2, std::stold(p_cursor, nullptr), "%0.1f", "V");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(52,32,0):{
			//std::cout << "TESTING 52.32.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSagsPL2, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(52,36,0):{
			//std::cout << "TESTING 52.36.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSwellsPL2, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(61,7,0):{
			//std::cout << "TESTING 61.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL3P, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(62,7,0):{
			//std::cout << "TESTING 62.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::InsActivePowerL3N, std::stold(p_cursor, nullptr), "%0.3f", "kW");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(71,7,0):{
			//std::cout << "TESTING 71.7.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::ICurrentInL3, std::stold(p_cursor, nullptr), "%u", "A");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(72,7,0):{
			//std::cout << "TESTING 72.7.0" << std::endl;
			obj = new CosemObjectNumber<double>(ObisType::IVoltInL3, std::stold(p_cursor, nullptr), "%0.1f", "V");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(72,32,0):{
			//std::cout << "TESTING 72.32.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSagsPL3, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(72,36,0):{
			//std::cout << "TESTING 72.36.0" << std::endl;
			obj = new CosemObjectNumber<unsigned long>(ObisType::NVoltageSwellsPL3, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(96,1,0):{
			//std::cout << "TESTING 96.1.0" << std::endl;

			char hex_buffer[3];
			char cBuf;
			std::string convertedBuf;

			while((*p_cursor != ')') && (*p_cursor != '\0')){
				std::strncpy(hex_buffer, p_cursor, 2);
				hex_buffer[2] = '\0';
				cBuf = std::stoul(hex_buffer, nullptr, 16);
				convertedBuf += cBuf;
				p_cursor += 2;
			}
			
			obj = new CosemObjectString(ObisType::EquipmentIdentifierGas, convertedBuf);

			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(96,1,1):{
			char hex_buffer[3];
			char cBuf;
			std::string converted_buf;

			while((*p_cursor != ')') && (*p_cursor != '\0')){
				std::strncpy(hex_buffer, p_cursor, 2);
				hex_buffer[2] = '\0';
				cBuf = std::stoul(hex_buffer, nullptr, 16);
				converted_buf += cBuf;
				p_cursor += 2;
			}
			
			obj = new CosemObjectString(ObisType::Equipment_identifier, converted_buf);

			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(96,7,9):{
			//std::cout << "TESTING 96.7.9" << std::endl;
			//Number of long power failures in any phase
			obj = new CosemObjectNumber<unsigned long>(ObisType::NLongPowerFailuresInPhase, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(96,7,21):{
			//std::cout << "TESTING 96.7.21" << std::endl;
			//Number of power failures in any phase
			obj = new CosemObjectNumber<unsigned long>(ObisType::NPowerFailuresInPhase, std::stold(p_cursor, nullptr), "%u");
			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(96,13,0):{
			//std::cout << "TESTING 96.13.0" << std::endl;
			char hex_buffer[3];
			char cBuf;
			std::string converted_buf;

			while((*p_cursor != ')') && (*p_cursor != '\0')){
				std::strncpy(hex_buffer, p_cursor, 2);
				hex_buffer[2] = '\0';
				cBuf = std::stoul(hex_buffer, nullptr, 16);
				converted_buf += cBuf;
				p_cursor += 2;
			}
			
			obj = new CosemObjectString(ObisType::TextMessage, converted_buf);

			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(96,14,0):{
			// char hex_buffer[3];
			// char cBuf;
			// std::string converted_buf;

			// while((*p_cursor != ')') && (*p_cursor != '\0')){
			//     std::strncpy(hex_buffer, p_cursor, 2);
			//     hex_buffer[2] = '\0';
			//     cBuf = std::stoul(hex_buffer, nullptr, 16);
			//     converted_buf += cBuf;
			//     p_cursor += 2;
			// }

			//This is implemented wrongfully implemented in P1, just convert to int
			
			//CosemObject * obj = new CosemObjectString(ObisType::TariffIndicator, converted_buf);

			obj = new CosemObjectNumber<int>(ObisType::TariffIndicator, strtol(p_cursor, nullptr, 10), "%i");


			o_channel->addCosemObject(obj);
			break;
		}
		case ObisBitMask(99,97,0):{
			obj = new CosemEventLog(ObisType::PowerFailureEventLog);
			
			int n_log_count = ObisCodeSectionToInt(p_cursor);
			++p_cursor;

			for (int i = 0; i < n_log_count; i++){
				//move p_cursor to next section
				while ((*p_cursor != ')') && (*p_cursor != '\n') && (*p_cursor != '\0')){
					++p_cursor;
				}
				
				if((*p_cursor == '\n') && (*p_cursor == '\0')){
					//ERROR STATE IN PARSE
					break;
				}
				
				p_cursor +=2;

				CosemObjectTst * objT = new CosemObjectTst(ObisType::Timestamp,	ObisStringToIntLenght(p_cursor, 2),
				ObisStringToIntLenght(p_cursor, 2),
				ObisStringToIntLenght(p_cursor, 2),
				ObisStringToIntLenght(p_cursor, 2),
				ObisStringToIntLenght(p_cursor, 2),
				ObisStringToIntLenght(p_cursor, 2));
				p_cursor +=3;
				CosemObjectNumber<int> * objF = new CosemObjectNumber<int>(ObisType::PowerFailureEventLogTime, ObisCodeSectionToInt(p_cursor), "%i", "s");
				static_cast<CosemEventLog*>(obj)->addEventLog(objT, objF);
			}

			o_channel->addCosemObject(obj);

			break;
		}
		default:
		break;
	}
	return 0;
}

P1Decoder::~P1Decoder(){
	if(!cosem_channel_list.empty()){
		std::list<CosemChannel*>::iterator it;
		for(it = cosem_channel_list.begin(); it != cosem_channel_list.end(); ++it){
			delete *it;
		}
	}
} //~P1Decoder
