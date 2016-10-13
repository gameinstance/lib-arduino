#include <Arduino.h>

/////////////////////////////////////////////////
/*
*	A thermistor based temperature probe
*	in a voltage divisor configuration.
*
*	Vcc
*	o--------+
*		 |
*		[ ] Rseries
*		[ ]
*		 |
*		 +----o Vout
*		 |
*		[ ] Rthermistor
*		[ ]
*		 |
*		---
*	GND	 -
*/
class NTCThermistor {
	
	public:
		
		/// default constructor
		NTCThermistor();
		
		/// configures the object
		void Config(
			float nominalResistance, 
			float nominalTemperature, 
			int betaCoefficient, 
			float seriesResistance, 
			int ADCResolution
		);
		
		/// gets the temperature given the read value
		float TemperatureC(int valoare);
		
	private:
	
		/// thermistor's nominal resistance [Ohm]
		float m_NominalResistance;
		/// thermistor's nominal temperature [C]
		float m_NominalTemperature;
		/// thermistor's beta coefficient
		int m_BetaCoefficient;
		/// the series resistor's resistance [Ohm]
		float m_SeriesResistance;
		/// the ADC resolution
		int m_ADCResolution;
};
