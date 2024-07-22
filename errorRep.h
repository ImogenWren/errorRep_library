/* errorRep Library

Library for reporting, tracking & clearing of Error & Status codes in embedded systems

Features:
  - set_error(); // Add a new error to the active error list
  - clear_error(); // Remove Error code from active error list
  - error_report(); // Print the current errors
  - error_log();    // Print List of all logged errors & their status
  - current_error();   // Prints the last highest level error. (eg if last error was DEBUG, but previous error was ERROR, ERROR will be saved,
  DEBUG will be logged but not saved as current error)

// Note: Error_Log could end up taking up a significant amount of memory. Lower Memory topologies will be added later

// Note: Error codes are negative integers

// Note: errorRep uses ArduinoJSON library for generating error reports


*/


#pragma once

#ifndef errorRep_h
#define errorRep_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//#include "stdio.h"






#endif
