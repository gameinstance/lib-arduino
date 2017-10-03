#include <Arduino.h>

/*
 * 
 * Abstract serial command class.
 * 
 * Needs to be implemented as per
 * README.md indications.
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
