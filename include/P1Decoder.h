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

#include "CrcP1.h"

#include <ctime>

#include <typeinfo>

enum class ObisType{
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

static const char * getObisTypeString(ObisType t){
	switch (t){
		case ObisType::Version:
			return "Version information";
		case ObisType::Power:
			return "Meter reading (Tariff 1)";
		case ObisType::Timestamp:
			return "Timestamp of msg";
		case ObisType::Equipment_identifier:
			return "Equipment identifier";
		case ObisType::EDeliveredToClientT1:
			return "Electricity delivered to client(T1)";
		case ObisType::EDeliveredToClientT2:
			return "Electricity delivered to client(T2)";
		case ObisType::EDeliveredByClientT1:
			return "Electricity delivered by client(T1)";
		case ObisType::EDeliveredByClientT2:
			return "Electricity delivered by client(T2)";
		case ObisType::TariffIndicator:
			return "Tariff indicator";
		case ObisType::PDelivered:
			return "Power delivered (+P)";
		case ObisType::PReceived:
			return "Power received (-P)";
		case ObisType::NPowerFailuresInPhase:
			return "Number of power failures in any phase";
		case ObisType::NLongPowerFailuresInPhase:
			return "Number of long power failures in any phase";
		case ObisType::NVoltageSagsPL1:
			return "Number of voltage sags in phase L1";
		case ObisType::NVoltageSagsPL2:
			return "Number of voltage sags in phase L2";
		case ObisType::NVoltageSagsPL3:
			return "Number of voltage sags in phase L3";
		case ObisType::NVoltageSwellsPL1:
			return "Number of voltage swells in phase L1";
		case ObisType::NVoltageSwellsPL2:
			return "Number of voltage swells in phase L2";
		case ObisType::NVoltageSwellsPL3:
			return "Number of voltage swells in phase L3";
		case ObisType::TextMessage:
			return "Text Message";
		case ObisType::IVoltInL1:
			return "Instantaneous voltage L1 in V resolution";
		case ObisType::IVoltInL2:
			return "Instantaneous voltage L2 in V resolution";
		case ObisType::IVoltInL3:
			return "Instantaneous voltage L3 in V resolution";
		case ObisType::ICurrentInL1:
			return "Instantaneous current L1 in A resolution.";
		case ObisType::ICurrentInL2:
			return "Instantaneous current L2 in A resolution.";
		case ObisType::ICurrentInL3:
			return "Instantaneous current L3 in A resolution.";
		case ObisType::InsActivePowerL1P:
			return "Instantaneous active power L1 (+P)";
		case ObisType::InsActivePowerL2P:
			return "Instantaneous active power L2 (+P)";
		case ObisType::InsActivePowerL3P:
			return "Instantaneous active power L3 (+P)";
		case ObisType::InsActivePowerL1N:
			return "Instantaneous active power L1 (-P)";
		case ObisType::InsActivePowerL2N:
			return "Instantaneous active power L2 (-P)";
		case ObisType::InsActivePowerL3N:
			return "Instantaneous active power L3 (-P)";
		case ObisType::DeviceType:
			return "Device type (M-bus)";
		case ObisType::EquipmentIdentifierGas:
			return "Equipment identifier (Gas)";
		case ObisType::PowerFailureEventLog:
			return "Power failure event log";
		case ObisType::PowerFailureEventLogTime:
			return "Event lenght";
		case ObisType::GasDeliveredLog:
			return "Last 5-minute value gas delivered";
		case ObisType::GasDelivered:
			return "Gas delivered";
		default:
			return "NO VALUE IN OBISTypeString";
			break;
	}
}

/**
 * Base virtual class for COSEM objects.
 */
class CosemObject{ //pure virtual class
	public:
		/**
		 * Virtual get the OBIS type of the object.
		 *
		 * All OBIS objects are unique and can only contain
		 * one type.
		 * 
		 * \return ObisType type of the object stored within.
		 */
		virtual ObisType getType() const= 0;
		
		/**
		 * Virtual destructor.
		 *
		 * Implementation must include a destructor to handle
		 * complex internal types.
		 */
		virtual ~CosemObject() = default;
		
		/**
		 * Virtual operator ==.
		 *
		 * Compare the COSEM object internal OBIS type.
		 *
		 * \param o reference to a CosemObject.
		 * \return bool is o of the same ObisType
		 */
		virtual bool operator== (const CosemObject &o) = 0;

		/**
		 * Calculates the delta between two CosemObjects.
		 *
		 * Internally compares the object to check if they are the same type.
		 * If they are it calculates the delta and returns a string 
		 * with the delta value.
		 *
		 * \param x pointer to a CosemObject.
		 * \return std::string containing the printable delta
		 */
		virtual std::string getDelta(CosemObject * x) = 0;
		
		/**
		 * Print out the COSEM object.
		 *
		 * Prints the ObisType string and the value
		 * held by the CosemObject.
		 *
		 * \return std::string containing the CosemObject information
		 */
		virtual std::string print() = 0;
		
		/**
		 * Print the COSEM value.
		 *		 
		 * \return std::string containing the printable value of the CosemObject
		 */
		virtual std::string printValue() = 0;

		
};

/**
 * Template implementation of CosemObject to hold numbers.
 *
 * Hold numbers of type T.
 */
template <typename T>
class CosemObjectNumber : public CosemObject{
	private:
	T value;
	ObisType myObisType;
	const char * unitText;
	const char * formatP;

	public:
	ObisType getType() const{
		return myObisType;
	}
	
	/**
	 * Get the internally stored number value.
	 */
	T getValue(){
		return value;
	}

	std::string getDelta(CosemObject * o){
		auto x = dynamic_cast<CosemObjectNumber<T>*>(o);
		if(x){
			char buf[20];
			sprintf(buf, formatP, (value - x->getValue()));
			return std::string(buf + std::string(unitText));
		}
		return std::string("NaN");
	}
	
	/**
	 * Return a std::string with the formated number.
	 *
	 * \return std::string with number formated in a string
	 */
	std::string getValueString(){
		char buf[20];
		sprintf(buf, formatP, value);

		return std::string(buf);
	}

	/**
	 * Return the unit for the stored value in a string.
	 *
	 * \return std::string with the unit of the number
	 */
	std::string getUnitString(){
		return std::string(unitText);
	}

	bool operator==(const CosemObject &o){
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		char buf[20];
		sprintf(buf, formatP, value);

		return std::string(	getObisTypeString(getType())
							+ std::string(": ")
							+ buf
							+ unitText);
	}

	std::string printValue(){
		char buf[20];
		sprintf(buf, formatP, value);

		return std::string(buf + getUnitString());
	}

	/**
	 * Create a Cosem object for numbers.
	 *
	 * Hold a number with the type specified by template.
	 * When printing this object the format and unit will be used
	 * to generate the string.
	 *
	 * \param type the ObisType of this object.
	 * \param value template parameter, type for stored number.
	 * \param format pointer to a const char array with the formatting string.
	 * \param unit pointer to a const char array holding the unit of the number.
	 */
	CosemObjectNumber(ObisType type, T v, const char * format, const char * unit = ""){
		unitText = unit;
		myObisType = type;
		value = v;
		formatP = format;
	}

	~CosemObjectNumber(){
	}

	private:
	CosemObjectNumber( const CosemObjectNumber &c );
	CosemObjectNumber& operator=( const CosemObjectNumber &c );
};

/**
 * Implementation of CosemObject to hold a string.
 */
class CosemObjectString : public CosemObject{
	private:
	std::string s;
	ObisType thisType;

	public:

	ObisType getType() const{
		return thisType;
	}

	bool operator==(const CosemObject &o){
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string getDelta(CosemObject * o){
		return std::string("NaN");
	}

	std::string print(){
		return std::string(	getObisTypeString(getType())
		+ std::string(": ")
		+ s);
	}

	std::string printValue(){
		return std::string(s); //Copy to keep temporary on stack
		//Might work without copy
	}
	
	/**
	 * Generate a cosem object with a string.
	 *
	 * \param type the ObisType of this object.
	 * \param inputS input string.
	 */
	CosemObjectString(ObisType type, std::string inputS){
		s = inputS;
		thisType = type;
	}

	~CosemObjectString(){
	}
};

/**
 * Implementation of CosemObject to hold a timestamp.
 */
class CosemObjectTst : public CosemObject{
	
	private:
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	ObisType thisType;

	public:

	ObisType getType() const{
		return thisType;
	}

	bool operator==(const CosemObject &o){
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string getDelta(CosemObject * o){
		return std::string("NaN");
	}

	std::string print(){
		return std::string(	getObisTypeString(getType()) + std::string(": ")
		+ std::to_string(year)
		+ std::string(":") + std::to_string(month)
		+ std::string(":") + std::to_string(day)
		+ std::string(":") + std::to_string(hour)
		+ std::string(":") + std::to_string(min)
		+ std::string(":") + std::to_string(sec));
	}

	std::string printValue(){
		return std::string(	  std::string("Y") + std::to_string(year)
		+ std::string("M") + std::to_string(month)
		+ std::string("D") + std::to_string(day)
		+ std::string("H") + std::to_string(hour)
		+ std::string("m") + std::to_string(min)
		+ std::string("s") + std::to_string(sec));
	}

	/**
	 * Generate a Cosem object holding a timestamp.
	 *
	 * \param type the ObisType of this object.
	 * \param y year of timestamp.
	 * \param MM month of timestamp.
	 * \param d day of timestamp.
	 * \param h hour of timestamp.
	 * \param m minute of timestamp.
	 * \param s second of timestamp.
	 */
	CosemObjectTst(ObisType type, int y, int MM, int d, int h, int m, int s){
		year = y;
		month = MM;
		day = d;
		hour = h;
		min = m;
		sec = s;
		thisType = type;
	}

	~CosemObjectTst(){
	}
};

/**
 * Implementation of CosemObject to hold a event log.
 */
class CosemEventLog : public CosemObject{
	
	private:
	
	struct PowerFailureEventObj{
		CosemObject * timestamp;
		CosemObject * duration;
	};
		
	std::list<PowerFailureEventObj> log;
	ObisType thisType;

	public:

	ObisType getType() const{
		return thisType;
	}

	std::string getDelta(CosemObject * o){
		return std::string("NaN");
	}

	bool operator==(const CosemObject &o){
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		std::string combinedLogPrint;

		combinedLogPrint.append(getObisTypeString(getType()) + std::string(":"));

		for(const auto &Optr: log){
			combinedLogPrint.append("\r\n\t");
			combinedLogPrint.append(Optr.timestamp->print());
			combinedLogPrint.append("\r\n\t");
			combinedLogPrint.append(Optr.duration->print());
		}
		return combinedLogPrint;
	}

	std::string printValue(){
		std::string combinedLogPrint;

		for(const auto &Optr: log){
			combinedLogPrint.append(Optr.timestamp->printValue());
			combinedLogPrint.append(",");
			combinedLogPrint.append(Optr.duration->printValue());
			combinedLogPrint.append("\r\n");
		}
		return combinedLogPrint;
	}
	
	/**
		* Generate a Cosem object for the power failure event log.
		*
		* \param type the ObisType of this object.
		*/
	CosemEventLog(ObisType type){
		thisType = type;
	}

	/**
		* Add an event to the log.
		*
		* \param time timestamp of event.
		* \param val duration of event.
		*/
	void addEventLog(CosemObject * time, CosemObject * val){
		log.push_back(PowerFailureEventObj{time, val});
	}

	~CosemEventLog(){
		std::list<PowerFailureEventObj>::iterator it = log.begin();

		while(it != log.end()){
			delete it->duration;
			delete it->timestamp;
			++it;
		}
	}
};

/**
 * CosemChannel that holds multiple CosemObject.
 *
 * A COSEM message can contain multiple devices on the same bus.
 * These are separated in a different channel and each hold CosemObjects.
 */
class CosemChannel{
	private:
	
	std::list<CosemObject*> nList;
	unsigned int channel_number;

	public:
	
	/**
		* Construct a CosemChannel .
		*
		* \param channelN number for this channel.
		*/
	CosemChannel(unsigned int channelN){
		channel_number = channelN;
	}

	/**
		* Destructor for all stored channel info.
		*
		* Delete all objects stored in channel.
		*/
	~CosemChannel(){
		std::list<CosemObject*>::iterator it = nList.begin();

		while(it != nList.end()){
			CosemObject * prevP = (*it);
			it = nList.erase(it);
			delete prevP;
		}
	}
	
	/**
		* Get channel number.
		*
		* \return unsigned int with the channel number.
		*/
	const unsigned int getChannelNumber() const{
		return channel_number;
	}

	const unsigned int getChannelSize(){
		return nList.size();
	}

	std::string print(){
		std::string s = "Channel # ";
		s.append(std::to_string(channel_number));
		return s;
	}

	/**
		* Compare channel.
		*
		* Every channel should have a different number to differentiate.
		* If two channels are compared and have the same channel number,
		* they will act as if they are the same, regardless of object in
		* the channel.
		*
		* \param o a const reference to CosemChannel.
		* \return bool do the channels match
		*/
	bool operator==(const CosemChannel &o){
		if(o.getChannelNumber() == this->getChannelNumber()){
			return true;
		}
		return false;
	}
	
	// @TODO Should be hidden, replace with a search function, in channel number
	/**
		* \return std::list<CosemObject*> pointer to object list.
		*/
	std::list<CosemObject*> & getCosemObjectList(){
		return nList;
	}

	/**
		* Add a CosemObject to this channel.
		*
		* \param o a pointer to CosemObject.
		*/
	void addCosemObject(CosemObject * o){
		for(const auto &Optr: nList){
			if (*Optr == *o){
				nList.remove(Optr);
				delete Optr;
				break;
			}
		}
		nList.push_back(o);
	}
};

/**
 * Holds decoded P1 data and interract with this data
 */
class P1Decoder{
	public:
		P1Decoder();

		~P1Decoder();	

		unsigned int getCosemChannelCount();

		unsigned int getCosemChannelSize(unsigned int n);

		std::string getCosemStringFromChannel(unsigned int channelNumber, ObisType type);

		std::string getDeltaString(	unsigned int channelNumber1, ObisType type1, 
									unsigned int channelNumber2, ObisType type2);

		/**
		 * Remove the channel matching on n.
		 *
		 * \param n channel number to match.
		 */
		void removeCosemChannel(CosemChannel * n);

		/**
		 * Removes all channels stored.
		 */
		void clearAllCosemChannels();
	
		/**
		 * Get device identifier string.
		 *
		 * \return std::string device identifier.
		 */
		std::string getDeviceIdentifier();
		
		/**
		 * Set device identifier.
		 *
		 * \param id string with device identifier.
		 */
		void setDeviceIdentifier(std::string id);

		/**
		 * Decode a P1 telegram string.
		 *
		 * \param msgStart pointer to char array.
		 */
		int decodeP1String(const char * msgStart);

	private:

		/**
		 * Static function to decode a P1 telegram.
		 *
		 * Decodes P1 telegram into Cosem objects.
		 * The decoded message is stored in the referenced P1Decoder obejct.
		 *
		 * \param msgStart string holding the P1 telegram.
		 * \param p1C reference to P1Decoder object where the decoded telegram is stored.
		 */
		static int decodeP1(const char * msgStart, P1Decoder & p1C); //should be null terminated string
		
		// [0] --- [1] --- [2] --- [n]
		// P1 --- M-bus0 --- Mbus1 --- Mbus n
		std::list<CosemChannel*> CosemChannelList;

		//@TODO should be abstracted away
		/**
		 * Returns pointer to channel list.
		 *
		 * \return std::list<CosemChannel*> pointer to channel list
		 */
		std::list<CosemChannel*> getAllCosemChannels();
		
		//@TODO should be abstracted away
		/**
		 * Returns pointer to a channel in the channel list.
		 *
		 * Match n to a channel if possible, if not found a new channel
		 * get added to the list.
		 *
		 * \param n number of the channel in channel list
		 * \return CosemChannel* pointer to channel
		 */
		CosemChannel * addCosemChannel(unsigned int n);

		CosemChannel * getCosemChannel(unsigned int n);		

		static int OBISCodeSectionToInt(const char * &startSec);
		static int OBISStringToIntLenght(const char * &startSec, unsigned int n);
		static int OBISMoveCursorNextLine(const char * &startSec);
		static int OBISAddObjectToChannel(CosemChannel * oChannel, const char * &startSec, int sec1, int sec2, int sec3);
	
		std::string deviceIdentifier = "";

		P1Decoder( const P1Decoder &c );
		P1Decoder& operator=( const P1Decoder &c );
	
		

}; //P1Decoder

#endif //__P1DECODER_H__
