/* 
* CosemObject.h
*
* Created: 4-7-2023 00:02:44
* Author: Jordan
*/


#ifndef __COSEMOBJECT_H__
#define __COSEMOBJECT_H__

#include <stdio.h>
#include <list>
#include <cstring>
#include <iostream>
#include <string>

#include "CrcP1.h"

#include <ctime>

#include <typeinfo>

/**
 * All Obis codes have a defined type.
 */
enum class ObisType{
	Version = 0,
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
	COUNT,

};

/**
 * Holds functions to interact with ObisType.
 */
namespace Obis{
	
	/**
	 * Get next ObisType from ObisType enum.
	 *
	 * \param t_ current ObisType.
	 * \return ObisType next ObisType from ObisType enum.
	 */
	const ObisType getNextType(ObisType t_);
	
	/**
	 * Get previous ObisType from ObisType enum.
	 *
	 * \param t_ current ObisType.
	 * \return ObisType previous ObisType from ObisType enum.
	 */
	const ObisType getPrevType(ObisType t_);
	
	/**
	 * Get string associated with ObisType.
	 *
	 * \param t_ current ObisType.
	 * \return string readable string for ObisType.
	 */
	const char * getObisTypeString(ObisType t_);
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
	ObisType this_type;
	const char * unit_text;
	const char * format_p;

	public:
	ObisType getType() const{
		return this_type;
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
			sprintf(buf, format_p, (value - x->getValue()));
			return std::string(buf + std::string(unit_text));
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
		sprintf(buf, format_p, value);

		return std::string(buf);
	}

	/**
	 * Return the unit for the stored value in a string.
	 *
	 * \return std::string with the unit of the number
	 */
	std::string getUnitString(){
		return std::string(unit_text);
	}

	bool operator==(const CosemObject &o){
		if(o.getType() == this->getType()){
			return true;
		}
		return false;
	}

	std::string print(){
		char buf[20];
		sprintf(buf, format_p, value);

		return std::string(	Obis::getObisTypeString(getType())
							+ std::string(": ")
							+ buf
							+ unit_text);
	}

	std::string printValue(){
		char buf[20];
		sprintf(buf, format_p, value);

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
	 * \param v template parameter, type for stored number.
	 * \param format pointer to a const char array with the formatting string.
	 * \param unit pointer to a const char array holding the unit of the number.
	 */
	CosemObjectNumber(ObisType type, T v, const char * format, const char * unit = ""){
		unit_text = unit;
		this_type = type;
		value = v;
		format_p = format;
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
	ObisType this_type;

	public:

	ObisType getType() const{
		return this_type;
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
		return std::string(	Obis::getObisTypeString(getType())
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
	 * \param input_s input string.
	 */
	CosemObjectString(ObisType type, std::string input_s){
		s = input_s;
		this_type = type;
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
	ObisType this_type;

	public:

	ObisType getType() const{
		return this_type;
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
		return std::string(	Obis::getObisTypeString(getType()) + std::string(": ")
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
	 * \param mm month of timestamp.
	 * \param d day of timestamp.
	 * \param h hour of timestamp.
	 * \param m minute of timestamp.
	 * \param s second of timestamp.
	 */
	CosemObjectTst(ObisType type, int y, int mm, int d, int h, int m, int s){
		year = y;
		month = mm;
		day = d;
		hour = h;
		min = m;
		sec = s;
		this_type = type;
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
	ObisType this_type;

	public:

	ObisType getType() const{
		return this_type;
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
		std::string combined_log_print;

		combined_log_print.append(Obis::getObisTypeString(getType()) + std::string(":"));

		for(const auto &Optr: log){
			combined_log_print.append("\r\n\t");
			combined_log_print.append(Optr.timestamp->print());
			combined_log_print.append("\r\n\t");
			combined_log_print.append(Optr.duration->print());
		}
		return combined_log_print;
	}

	std::string printValue(){
		std::string combined_log_print;

		for(const auto &o_ptr: log){
			combined_log_print.append(o_ptr.timestamp->printValue());
			combined_log_print.append(",");
			combined_log_print.append(o_ptr.duration->printValue());
			combined_log_print.append("\r\n");
		}
		return combined_log_print;
	}
	
	/**
	* Generate a Cosem object for the power failure event log.
	*
	* \param type the ObisType of this object.
	*/
	CosemEventLog(ObisType type){
		this_type = type;
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
	
	std::list<CosemObject*> n_list;
	unsigned int channel_number;

	public:
	
	/**
	* Construct a CosemChannel .
	*
	* \param channel_n number for this channel.
	*/
	CosemChannel(unsigned int channel_n){
		channel_number = channel_n;
	}

	/**
	* Destructor for all stored channel info.
	*
	* Delete all objects stored in channel.
	*/
	~CosemChannel(){
		std::list<CosemObject*>::iterator it = n_list.begin();

		while(it != n_list.end()){
			CosemObject * prev_p = (*it);
			it = n_list.erase(it);
			delete prev_p;
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

	/**
	* Get channel size.
	*
	* \return unsigned int with the channel size.
	*/
	const unsigned int getChannelSize(){
		return n_list.size();
	}

	/**
	* Print channel number.
	*
	* \return string with the channel number.
	*/
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

	/**
	* \return std::list<CosemObject*> pointer to object list.
	*/
	std::list<CosemObject*> & getCosemObjectList(){
		return n_list;
	}

	/**
	* Add a CosemObject to this channel.
	*
	* \param o a pointer to CosemObject.
	*/
	void addCosemObject(CosemObject * o){
		for(const auto &o_ptr: n_list){
			if (*o_ptr == *o){
				n_list.remove(o_ptr);
				delete o_ptr;
				break;
			}
		}
		n_list.push_back(o);
	}
};

#endif //__COSEMOBJECT_H__
