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
#include <autoDelay.h>

autoDelay printDelay;
#define PRINT_DELAY_mS 3000


// Logging Level Constants
typedef enum {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
} msgLevel;


#define LOGGING_LEVEL DEBUG    // Sets the level of message that is logged to errors log
#define ERROR_LOG_MAX_ENTRY 5  // defines the maximum length of error log. When error log is filled, flag is set that could be used to warn user, \
                                 // New errors overwrite oldest errors.

msgLevel loggingLevel = LOGGING_LEVEL;

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
  char context[32];
  char msg[64];
};


statusStruc currentStatus = { 0, false, INFO, 0, " ", " " };

statusStruc errorLog[ERROR_LOG_MAX_ENTRY];

JsonDocument JSON;

#define PRETTY_PRINT_JSON true

bool fatal_error_flag = false;
int16_t fatal_error_code = 0;

void build_json() {
  JSON["timestamp"] = millis();
  JSON["active"] = false;
  JSON["level"] = debugLevel[loggingLevel];
  JSON["code"] = 0;
  JSON["context"] = "build_json()";
  JSON["msg"] = "";
}

void update_json() {
}

void print_json() {
#if PRETTY_PRINT_JSON == true
#pragma JSON set to Pretty Print !-May experience problems with Machine Parsing
  serializeJsonPretty(JSON, Serial);
#else
#pragma JSON set to standard JSON output - For human readability try enabling Pretty Print !
  serializeJson(JSON, Serial);
#endif
  Serial.print("\n");
}


size_t get_log_length() {
  size_t log_length = sizeof(errorLog) / sizeof(errorLog[0]);
  // Serial.print("log_length: ");
  //  Serial.println(log_length);
  return log_length;
}

void shuffle_log() {
  size_t log_length = get_log_length();
  for (int i = (log_length - 1); i > 0; i--) {
    Serial.print("Moving Log: ");
    Serial.print(i - 1);
    Serial.print(", to Log: ");
    Serial.println(i);
    errorLog[i] = errorLog[i - 1];
  }
}

void print_log_entry_as_json(statusStruc log_entry) {
  JsonDocument logJSON;
  JSON["timestamp"] = (log_entry.timestamp);
  JSON["active"] = (log_entry.active);
  JSON["level"] = (debugLevel[log_entry.level]);
  JSON["code"] = (log_entry.code);
  JSON["context"] = (log_entry.context);
  JSON["msg"] = (log_entry.msg);
  serializeJsonPretty(JSON, Serial);
  Serial.print("\n");
}

// Use this to set errors elsewhere, can also be used to reset errors by calling with no arguments
void set_error(bool active = true, msgLevel logLevel = INFO, int16_t code = 0, const char* context = "", const char* msg = "") {
  currentStatus.timestamp = millis();
  currentStatus.active = active;
  currentStatus.level = logLevel;
  currentStatus.code = code;
  strcpy(currentStatus.context, context);
  strcpy(currentStatus.msg, msg);
  JSON["timestamp"].set(currentStatus.timestamp);
  JSON["active"].set(currentStatus.active);
  JSON["level"].set(debugLevel[logLevel]);
  JSON["code"].set(currentStatus.code);
  JSON["context"].set(currentStatus.context);
  JSON["msg"].set(currentStatus.msg);
  if (logLevel == FATAL) {
    fatal_error_flag = true;  //
    fatal_error_code = code;  // log the error that caused the FATA_ERROR as can be cleared later if required
  }
  Serial.print("Log Level: ");
  Serial.print(logLevel);
  Serial.print(" >= ");
  Serial.println(LOGGING_LEVEL);
  if (logLevel >= LOGGING_LEVEL) {  // if tghe error code meets the logging level then it should be recorded
    Serial.println("Logging!");
    shuffle_log();
    errorLog[0].timestamp = currentStatus.timestamp;
    errorLog[0].active = currentStatus.active;
    errorLog[0].level = currentStatus.level;
    errorLog[0].code = currentStatus.code;
    strcpy(errorLog[0].context, currentStatus.context);
    strcpy(errorLog[0].msg, currentStatus.msg);
  }
#if DEBUG_ERRORS == true
  Serial.print("DEBUG_ERRORS (set): ");
  Serial.print(code);
  Serial.print("  msg: ");
  Serial.println(msg);
#endif
}

void clear_error(int16_t error_code, bool clear_fatal = false) {
  // Now need to search through whole list of entries to find code that matches and make active false
  int16_t current_error = JSON["payload"]["status"]["code"];
#if DEBUG_ERRORS == true
  Serial.print("DEBUG_ERRORS (clear): current_error: ");
  Serial.print(current_error);
  Serial.print(" clear_code: ");
  Serial.println(error_code);
#endif
  if (current_error == error_code) {
    set_error();  // Set error with no values passed clears the current error
    if (clear_fatal) {
      if (fatal_error_code == error_code) {  // This may lead to confusion if multiple fatal errors have occured, may need simplifying later
        fatal_error_flag = false;            //
        fatal_error_code = 0;
      }
    }
#if DEBUG_ERRORS == true
    Serial.println("Errors Matched & Deleted");
#endif
  }
}


void setup() {
  Serial.begin(115200);
  build_json();
  // print_json();
  get_log_length();
  print_log_entry_as_json(errorLog[0]);
  delay(1000);
  set_error(true, FATAL, -12, "setup()", "Testing Errors");
  print_log_entry_as_json(errorLog[0]);
}



uint16_t iteration;

void loop() {

  if (printDelay.millisDelay(PRINT_DELAY_mS)) {
    //  print_json();
  }

  iteration++;
  delay(1000);
}
