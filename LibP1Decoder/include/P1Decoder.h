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

#include "CosemObject.h"

/**
 * Holds decoded P1 data and interact with this data
 */
class P1Decoder{
	public:
		P1Decoder();

		~P1Decoder();	

		/**
		 * Get the number of channels in decoded message.
		 *
		 * \return unsigned int number of channels
		 */
		unsigned int getCosemChannelCount();
		
		/**
		 * Get the number of objects in a channel.
		 *
		 * \param n channel number.
		 * \return unsigned int channel size.
		 */
		unsigned int getCosemChannelSize(unsigned int n);

		/**
		 * Get string of stored Cosem object in channel.
		 *
		 * If the object does not exist in the channel return
		 * "NaN"
		 *
		 * \param n channel number.
		 * \param type obis type to get string for
		 * \return string holding string for ObisType in channel
		 */
		std::string getCosemStringFromChannel(unsigned int n, ObisType type);
		
		/**
		 * Get the delta between two objects.
		 *
		 * If the objects exist and are of the same type and numeric the delta will be calculated and
		 * returned as a string with unit. If the object can not be found in the channel
		 * or the delta value can not be calculated return will be "NaN"
		 *
		 * \param n channel number.
		 * \param type1 ObisType of first item.
		 * \param type2 ObisType of second item.
		 * \return string holding delta.
		 */
		std::string getDeltaString(	unsigned int n, ObisType type1, ObisType type2);

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

		/**
		 * Returns pointer to channel list.
		 *
		 * \return std::list<CosemChannel*> pointer to channel list
		 */
		std::list<CosemChannel*> getAllCosemChannels();
		
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
