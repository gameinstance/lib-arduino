# lib-arduino
Arduino libraries:<br />
1. NTC Thermistor
2. LCD Nokia 5110
3. SerialCommand
Needs to be implemented, not instantiated, as per the following example:

//------------------------
#include <SerialCommand.h>

/*
 * The app specific serial command class.
 */
class MySerialCommand : public SerialCommand {

  protected:

    /// runs the command
    bool Run() {
      // 
      switch (data[0]) {
        // 
        case '?':
          // identify
          //Identify();
          return true;
        case 'd':
          // dump the FSM state
          //Dump();
	  return true;
      }
      // unknown command
      Serial.println("Unknown command!");
      return false;
    };
};

void setup() {
  // 
  sc.Init(9600);
}

void loop() {
  // 
  if (sc.Read()) {
    // 
    sc.Execute();
  }
  // the rest of your code goes here
}
//------------------------
