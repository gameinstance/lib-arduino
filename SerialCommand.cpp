#include <Arduino.h>
#include <SerialCommand.h>

SerialCommand::SerialCommand() {
	// 
}

SerialCommand::~SerialCommand() {
	// 
}

void SerialCommand::Init(int baudRate) {
	// 
	Serial.begin(baudRate);
}

bool SerialCommand::Read() {
	// 
	if (!Serial.available()) {
		// no data available
		return false;
	}
	// data available
	charIn = Serial.read();
	data[index] = charIn;
	index ++;
	data[index] = '\0';
	if (charIn == '\n') {
		// new command
		command = true;
	}
	return true;
}

bool SerialCommand::Execute() {
	// 
	if (!command) {
		// nothing to execute
		return false;
	}
	// a command was issued
	command = false;
	bool res = Run();
	Reset();
	return res;
}


void SerialCommand::Reset() {
	// 
	index = 0;
}
