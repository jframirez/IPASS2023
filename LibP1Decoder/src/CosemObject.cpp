/* 
* CosemObject.cpp
*
* Created: 4-7-2023 00:02:44
* Author: Jordan
*/


#include "CosemObject.h"

namespace Obis{
	
	const ObisType getNextType(ObisType t_){
		int t_val = static_cast<int>(t_);
		
		if(t_val < (static_cast<int>(ObisType::COUNT) - 1) ){
			return static_cast<ObisType>(t_val + 1);
		}
		return static_cast<ObisType>(0);
	}
	

	const ObisType getPrevType(ObisType t_){
		int t_val = static_cast<int>(t_);
		
		if(t_val > 0 ){
			return static_cast<ObisType>(t_val - 1);
		}
		return static_cast<ObisType>(Obis::getPrevType(ObisType::COUNT));
	}
	
	
	const char * getObisTypeString(ObisType t_){
		switch (t_){
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
				return "Event length";
			case ObisType::GasDeliveredLog:
				return "Last 5-minute value gas delivered";
			case ObisType::GasDelivered:
				return "Gas delivered";
			default:
				return "NO VALUE FOR OBISType";
				break;
		}
	}
}
