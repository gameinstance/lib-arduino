#include <Arduino.h>
#include <NTCThermistor.h>


NTCThermistor::NTCThermistor() {
	// making a series of assumptions
	m_NominalResistance = 100000;
	m_NominalTemperature = 25;
	m_BetaCoefficient = 3950;
	m_SeriesResistance = 100000;
	m_ADCResolution = 1024;
}

void NTCThermistor::Config(
	float nominalResistance, 
	float nominalTemperature, 
	int betaCoefficient, 
	float seriesResistance, 
	int ADCResolution) {
	// 
	m_NominalResistance = nominalResistance;
	m_NominalTemperature = nominalTemperature;
	m_BetaCoefficient = betaCoefficient;
	m_SeriesResistance = seriesResistance;
	m_ADCResolution = ADCResolution;
}

float NTCThermistor::TemperatureC(int value) {
	//
	float tmpValue = ((m_ADCResolution - 1) / (float)value) - 1;
	tmpValue = m_SeriesResistance / tmpValue;
	float steinhart = tmpValue / m_NominalResistance;
	steinhart = log(steinhart);
	steinhart /= m_BetaCoefficient;
	steinhart += 1.0 / (m_NominalTemperature + 273.15);
	steinhart = 1.0 / steinhart;
	steinhart -= 273.15;
	return steinhart;
}
