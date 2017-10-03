#include <Arduino.h>

/*
 * 
 * Abstract serial command class.
 * 
 * Needs to be implemented as per
 * the following example:
*/
/*
#include <SerialCommand.h>

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
  // the rest of the setup code
}

void loop() {
  // 
  if (sc.Read()) {
    // 
    sc.Execute();
  }
  // the rest of the loop code
}

*/

class SerialCommand {

  public:
  
    /// default c-tor
    SerialCommand();
    /// destructor
    virtual ~SerialCommand();

    /// initializes the serial port with the baud rate
    void Init(int baudRate);
    /// reads one char from the serial port
    bool Read();
    /// executes the command
    bool Execute();


  protected:

    /// runs the command
    virtual bool Run() = 0;
    /// resets the read chars
    void Reset();
    
    /// the command string;
    char data[64];
    /// inner command index
    byte index;


  private:
    
    /// maneuver char
    char charIn;
    /// command detected
    bool command;
};
