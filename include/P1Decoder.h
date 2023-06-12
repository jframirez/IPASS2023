/*
* P1Decoder.h
*
* Created: 4-6-2023 18:22:03
* Author: Jordan
*/


#ifndef __P1DECODER_H__
#define __P1DECODER_H__

#include <stdio.h>
#include <list>
#include <cstring>
#include <iostream>
#include <string>

#include "CRCp1.h"

#include <ctime>

extern const char * testP1Telegram;
extern const char * testP1Telegram2;

enum class OBISType{
	Version,
	Power,
	Timestamp,
	Equipment_identifier,
	EDeliveredToClientT1,
	EDeliveredToClientT2,
	EDeliveredByClientT1,
	EDeliveredByClientT2,
	TariffIndicator,
	PDelivered,
	PReceived,
	NPowerFailuresInPhase,
	NLongPowerFailuresInPhase,
	NVoltageSagsPL1,
	NVoltageSagsPL2,
	NVoltageSagsPL3,
	NVoltageSwellsPL1,
	NVoltageSwellsPL2,
	NVoltageSwellsPL3,
	TextMessage,
	IVoltInL1,
	IVoltInL2,
	IVoltInL3,
	ICurrentInL1,
	ICurrentInL2,
	ICurrentInL3,
	InsActivePowerL1P,
	InsActivePowerL2P,
	InsActivePowerL3P,
	InsActivePowerL1N,
	InsActivePowerL2N,
	InsActivePowerL3N,
	DeviceType,
	EquipmentIdentifierGas,
	PowerFailureEventLog,
	PowerFailureEventLogTime,
	GasDeliveredLog,
	GasDelivered,

};

static const char * getOBISTypeString(OBISType t){
	switch (t){
		case OBISType::Version:
		return "Version information";
		break;
		case OBISType::Power:
		return "Meter reading (Tariff 1)";
		case OBISType::Timestamp:
		return "Timestamp of msg";
		case OBISType::Equipment_identifier:
		return "Equipment identifier";
		case OBISType::EDeliveredToClientT1:
		return "Electricity delivered to client(T1)";
		case OBISType::EDeliveredToClientT2:
		return "Electricity delivered to client(T2)";
		case OBISType::EDeliveredByClientT1:
		return "Electricity delivered by client(T1)";
		case OBISType::EDeliveredByClientT2:
		return "Electricity delivered by client(T2)";
		case OBISType::TariffIndicator:
		return "Tariff indicator";
		case OBISType::PDelivered:
		return "Power delivered (+P)";
		case OBISType::PReceived:
		return "Power received (-P)";
		case OBISType::NPowerFailuresInPhase:
		return "Number of power failures in any phase";
		case OBISType::NLongPowerFailuresInPhase:
		return "Number of long power failures in any phase";
		case OBISType::NVoltageSagsPL1:
		return "Number of voltage sags in phase L1";
		case OBISType::NVoltageSagsPL2:
		return "Number of voltage sags in phase L2";
		case OBISType::NVoltageSagsPL3:
		return "Number of voltage sags in phase L3";
		case OBISType::NVoltageSwellsPL1:
		return "Number of voltage swells in phase L1";
		case OBISType::NVoltageSwellsPL2:
		return "Number of voltage swells in phase L2";
		case OBISType::NVoltageSwellsPL3:
		return "Number of voltage swells in phase L3";
		case OBISType::TextMessage:
		return "Text Message";
		case OBISType::IVoltInL1:
		return "Instantaneous voltage L1 in V resolution";
		case OBISType::IVoltInL2:
		return "Instantaneous voltage L2 in V resolution";
		case OBISType::IVoltInL3:
		return "Instantaneous voltage L3 in V resolution";
		case OBISType::ICurrentInL1:
		return "Instantaneous current L1 in A resolution.";
		case OBISType::ICurrentInL2:
		return "Instantaneous current L2 in A resolution.";
		case OBISType::ICurrentInL3:
		return "Instantaneous current L3 in A resolution.";
		case OBISType::InsActivePowerL1P:
		return "Instantaneous active power L1 (+P)";
		case OBISType::InsActivePowerL2P:
		return "Instantaneous active power L2 (+P)";
		case OBISType::InsActivePowerL3P:
		return "Instantaneous active power L3 (+P)";
		case OBISType::InsActivePowerL1N:
		return "Instantaneous active power L1 (-P)";
		case OBISType::InsActivePowerL2N:
		return "Instantaneous active power L2 (-P)";
		case OBISType::InsActivePowerL3N:
		return "Instantaneous active power L3 (-P)";
		case OBISType::DeviceType:
		return "Device type (M-bus)";
		case OBISType::EquipmentIdentifierGas:
		return "Equipment identifier (Gas)";
		case OBISType::PowerFailureEventLog:
		return "Power failure event log";
		case OBISType::PowerFailureEventLogTime:
		return "Event lenght";
		case OBISType::GasDeliveredLog:
		return "Last 5-minute value gas delivered";
		case OBISType::GasDelivered:
		return "Gas delivered";
		default:
		return "NO VALUE IN OBISTypeString";
		break;
	}
}

class OBISObject{ //pure virtual class
	public:
	virtual OBISType getType() const= 0;
	virtual ~OBISObject() = default;
	virtual bool operator== (const OBISObject &o) = 0;
	virtual std::string print() = 0;
};



class OBISObjectVersion : public OBISObject{
	private:
	int versionMain = 0;
	int versionDecimal = 0;

	public:
	

	OBISType getType() const{
		return OBISType::Version;
	}

	bool operator==(const OBISObject &o){
		////std::cout << "COMPARING OBISObject" << std::endl;
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		return std::string("DSMR Version: " + std::to_string(versionMain) + "." + std::to_string(versionDecimal));
	}

	OBISObjectVersion(int m, int d){
		versionMain = m;
		versionDecimal = d;
	}

	~OBISObjectVersion(){
		//std::cout << "OBIS OBJECT GETTING DELETED," << this->print() << std::endl;
	}
};


template <typename T>
class OBISObjectFloatingDecimal : public OBISObject{
	private:
	T value;
	OBISType myObisType;
	const char * unitText;
	const char * formatP;

	public:
	OBISType getType() const{
		return myObisType;
	}

	T getValue(){
		return value;
	}

	std::string getValueString(){
		char buf[20];
		sprintf(buf, formatP, value);

		return std::string(buf);
	}

	std::string getUnitString(){
		return std::string(unitText);
	}

	bool operator==(const OBISObject &o){
		////std::cout << "COMPARING OBISObject" << std::endl;
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		char buf[20];
		sprintf(buf, formatP, value);

		return std::string(getOBISTypeString(getType()) + std::string(": ") + buf + unitText);
	}

	OBISObjectFloatingDecimal(OBISType t, T v, const char * f, const char * unit = ""){
		unitText = unit;
		myObisType = t;
		value = v;
		formatP = f;
	}

	~OBISObjectFloatingDecimal(){
		//std::cout << "OBIS OBJECT GETTING DELETED," << this->print() << std::endl;
	}
};

class OBISObjectString : public OBISObject{
	private:
	std::string s;
	OBISType thisType;

	public:

	OBISType getType() const{
		return thisType;
	}

	bool operator==(const OBISObject &o){
		// ///std::cout << "COMPARING OBISObject" << std::endl;
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		return std::string(getOBISTypeString(getType()) + std::string(": ") + s);
	}

	OBISObjectString(OBISType t, std::string inputS){
		s = inputS;
		thisType = t;
	}

	~OBISObjectString(){
		//std::cout << "OBIS OBJECT GETTING DELETED," << this->print() << std::endl;
	}
};

class OBISObjectTST : public OBISObject{
	
	private:
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	OBISType thisType;

	public:

	OBISType getType() const{
		return thisType;
	}

	bool operator==(const OBISObject &o){
		////std::cout << "COMPARING OBISObject" << std::endl;
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		return std::string(getOBISTypeString(getType()) + std::string(": ") + std::to_string(year)
		+ std::string(":") + std::to_string(month)
		+ std::string(":") + std::to_string(day)
		+ std::string(":") + std::to_string(hour)
		+ std::string(":") + std::to_string(min)
		+ std::string(":") + std::to_string(sec));
	}

	OBISObjectTST(OBISType t, int y, int MM, int d, int h, int m, int s){
		year = y;
		month = MM;
		day = d;
		hour = h;
		min = m;
		sec = s;
		thisType = t;
	}

	~OBISObjectTST(){
		//std::cout << "OBIS OBJECT GETTING DELETED," << this->print() << std::endl;
	}
};

// struct PowerFailureEventObj{
// 	OBISObjectTST * timestamp;
// 	OBISObjectFloatingDecimal<int> * duration;
// };

struct PowerFailureEventObj{
	OBISObject * timestamp;
	OBISObject * duration;
};

class OBISPowerFailureEventLog : public OBISObject{
	private:
	std::list<PowerFailureEventObj> log;
	OBISType thisType;

	public:

	OBISType getType() const{
		return thisType;
	}

	bool operator==(const OBISObject &o){
		////std::cout << "COMPARING OBISObject" << std::endl;
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		std::string combinedLogPrint;

		combinedLogPrint.append(getOBISTypeString(getType()) + std::string(":\r\n"));

		for(const auto &Optr: log){
			combinedLogPrint.append("\t");
			combinedLogPrint.append(Optr.timestamp->print());
			combinedLogPrint.append("\r\n\t");
			combinedLogPrint.append(Optr.duration->print());
			combinedLogPrint.append("\r\n");
		}
		return combinedLogPrint;
	}

	OBISPowerFailureEventLog(OBISType t){
		thisType = t;
	}

	// void addOBISLog(OBISObjectTST * time, OBISObjectFloatingDecimal<int> * val){
	// 	log.push_back(PowerFailureEventObj{time, val});
	// }
	void addOBISLog(OBISObject * time, OBISObject * val){
		log.push_back(PowerFailureEventObj{time, val});
	}

	~OBISPowerFailureEventLog(){
		//std::cout << "OBIS OBJECT GETTING DELETED," << this->print() << std::endl;

		std::list<PowerFailureEventObj>::iterator it = log.begin();
		while(it != log.end()){
			//std::cout << "DELETE : " << it->duration->print() << std::endl;
			delete it->duration;
			delete it->timestamp;
			++it;
		}
	}
};

class OBISChannel{
	private:

	std::list<OBISObject*> nList;
	unsigned int channelNumber;

	public:

	OBISChannel(int channelN){
		channelNumber = channelN;
	}

	~OBISChannel(){
		//std::cout << "OBIS CHANNEL GETTING DELETED" << std::endl;
		std::list<OBISObject*>::iterator it = nList.begin();
		
		// for(it = nList.begin(); it != nList.end(); ++it){
		// 	//std::cout << "DELETE : " << dynamic_cast<OBISObject*>(*it)->print() << std::endl;
		// 	ref = it;
		// 	delete *ref;
		// }
		while(it != nList.end()){
			//std::cout << "DELETE : " << dynamic_cast<OBISObject*>(*it)->print() << std::endl;
			OBISObject * prevP = (*it);
			it = nList.erase(it);
			delete prevP;
		}
	}
	
	const unsigned int getChannelNumber() const{
		return channelNumber;
	}

	bool operator==(const OBISChannel &o){
		if(o.getChannelNumber() == this->getChannelNumber()){
			return true;
		}
		return false;
	}

	std::list<OBISObject*> & getOBISObjectList(){
		return nList;
	}

	void addOBISObject(OBISObject * o){

		//OBISObject * toRemove;

		for(const auto &Optr: nList){
			////std::cout << "COMP: " << o->print();
			if (*Optr == *o){
				//std::cout << "OBISObject Collision" << std::endl;
				nList.remove(Optr);
				delete Optr;
				break; //the for loop itterator breaks on deleting Optr, only one unique value should be in nList
				//Doing a single remove should be enough to remove collision and add the new OBISObject
			}
		}

		//std::cout << "ADDING OBJECT: " << o->print() << std::endl;
		nList.push_back(o);
	}
};

class P1Decoder{
	public:
	P1Decoder();
	~P1Decoder();

	// [0] --- [1] --- [2] --- [n]
	// P1 --- M-bus0 --- Mbus1 --- Mbus n
	std::list<OBISChannel*> OBISChannelList;
	
	static int decodeP1(const char * msgStart, std::list<OBISChannel*>&channelList, P1Decoder & p1C); //should be null terminated string

	std::list<OBISChannel*> getAllOBISChannels(){
		return OBISChannelList;
	}

	OBISChannel * getOBISChannel(unsigned int n){
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

	void removeOBISChannel(OBISChannel * n){
		for(const auto &Optr: OBISChannelList){
			if (Optr->getChannelNumber() == n->getChannelNumber()){
				//std::cout << "OBISObject Collision" << std::endl;
				OBISChannelList.remove(Optr);
				delete Optr;
				break;
			}
		}
	}

	private:

	static int OBIScodeSectionToInt(const char * &startSec);
	static int OBISStringToIntLenght(const char * &startSec, unsigned int n);
	static int OBISMoveCursorNextLine(const char * &startSec);
	static int OBISAddObjectToChannel(OBISChannel * oChannel, const char * &startSec, int sec1, int sec2, int sec3);

	P1Decoder( const P1Decoder &c );
	P1Decoder& operator=( const P1Decoder &c );

}; //P1Decoder

#endif //__P1DECODER_H__
