/* errorRep Library

Library for reporting, tracking & clearing of Error codes in embedded systems

Features:
  - set_error(); // Add a new error to the active error list
  - clear_error(); // Remove Error code from active error list
  - error_report(); // Print the current errors
  - error_log();    // Print List of all logged errors & their status

// Note: Error_Log could end up taking up a significant amount of memory. Lower Memory topologies will be added later

// Note: Error codes are negative integers

// Note: errorRep uses ArduinoJSON library for generating error reports


*/

#include <ArduinoJson.h>


// Logging Level Constants
typedef enum {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
} msgLevel;


#define LOGGING_LEVEL DEBUG      // Sets the level of message that is logged to errors log
#define ERROR_LOG_MAX_ENTRY 100  // defines the maximum length of error log. When error log is filled, flag is set that could be used to warn user, \
                                 // New errors overwrite oldest errors.

messageLevel loggingLevel = LOGGING_LEVEL;

char debugLevel[5][8] = {
  "DEBUG",
  "INFO",
  "WARN",
  "ERROR",
  "FATAL"
};


struct statusStruc {
  uint32_t timestamp;
  bool active;
  msgLevel level;
  int16_t code;
  char location[32];
  char msg[64];
};

statusStruc currentStatus = { 0, false, INFO, 0, " ", " " };



void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
