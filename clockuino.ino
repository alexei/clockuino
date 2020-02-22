#include <DS3231M.h>

DS3231M_Class DS3231M;

char output_buffer[32];

const uint8_t INPUT_BUFFER_SIZE = 32;
const char RESET_COMMAND[] = "RESET";
const char SET_COMMAND[] = "SET";

const char JAN_CODE[] = "Jan";
const char FEB_CODE[] = "Feb";
const char MAR_CODE[] = "Mar";
const char APR_CODE[] = "Apr";
const char MAY_CODE[] = "May";
const char JUN_CODE[] = "Jun";
const char JUL_CODE[] = "Jul";
const char AUG_CODE[] = "Aug";
const char SEP_CODE[] = "Sep";
const char OCT_CODE[] = "Oct";
const char NOV_CODE[] = "Nov";
const char DEC_CODE[] = "Dec";

void setup() {
  Serial.begin(9600);

  Serial.println("Init setup");
  Serial.flush();

  uint8_t retries = 0;
  bool found = false;
  while (retries < 3 && !found) {
    Serial.println("Trying to find DS3231M...");
    Serial.flush();
    
    if (DS3231M.begin()) {
      Serial.println("Found DS3231M");
      Serial.flush();
      found = true;
    }
    else {
      delay(1000);
      retries++;
    }
  }
  if (!found) {
    Serial.println("Couldn't find DS3231M");
    Serial.flush();
    exit(0);
  }

  DS3231M.pinSquareWave();
}

void loop() {
  static uint8_t secs;
  DateTime now = DS3231M.now();
  if (secs != now.second()) {
    sprintf(
      output_buffer,
      "%04d-%02d-%02d %02d:%02d:%02d",
      now.year(),
      now.month(),
      now.day(),
      now.hour(),
      now.minute(),
      now.second()
    );
    Serial.println(output_buffer);
    secs = now.second();
  }

  handle_input();
}

void handle_input() {
  static uint8_t i = 0;
  char input_char;
  static char input_buffer[INPUT_BUFFER_SIZE];
  while (Serial.available()) {
    input_char = Serial.read();
    if (input_char == '\n' || i == INPUT_BUFFER_SIZE) {
      input_buffer[i] = 0;

      if (strcmp(input_buffer, RESET_COMMAND) == 0) {
        handle_reset_command();
      }
      else {
        bool got_set_command = true;
        for (uint8_t i = 0; i < strlen(SET_COMMAND); i++) {
          if (input_buffer[i] != SET_COMMAND[i]) {
            got_set_command = false;
            break;
          }
        }
        if (got_set_command) {
          handle_set_command(input_buffer);
        }
      }
      
      memset(input_buffer, 0, sizeof(input_buffer));
      i = 0;
    }
    else {
      input_buffer[i] = input_char;
      i++;
    }
  }
}

void handle_reset_command() {
  Serial.println("Resetting time...");

  unsigned int tokens, year, month, day, hour, minute, second;
  char month_code[3];

  tokens = sscanf(__DATE__, "%s %u %u", month_code, &day, &year);
  if (tokens != 3) {
    Serial.println("Unable to parse date.");
    Serial.flush();
    return;
  }
  
  if (strcmp(month_code, JAN_CODE) == 0) {
    month = 1;
  }
  else if (strcmp(month_code, FEB_CODE) == 0) {
    month = 2;
  }
  else if (strcmp(month_code, MAR_CODE) == 0) {
    month = 3;
  }
  else if (strcmp(month_code, APR_CODE) == 0) {
    month = 4;
  }
  else if (strcmp(month_code, MAY_CODE) == 0) {
    month = 5;
  }
  else if (strcmp(month_code, JUN_CODE) == 0) {
    month = 6;
  }
  else if (strcmp(month_code, JUL_CODE) == 0) {
    month = 7;
  }
  else if (strcmp(month_code, AUG_CODE) == 0) {
    month = 8;
  }
  else if (strcmp(month_code, SEP_CODE) == 0) {
    month = 9;
  }
  else if (strcmp(month_code, OCT_CODE) == 0) {
    month = 10;
  }
  else if (strcmp(month_code, NOV_CODE) == 0) {
    month = 11;
  }
  else if (strcmp(month_code, DEC_CODE) == 0) {
    month = 12;
  }
  
  if (!month) {
    Serial.println("Unable to parse month");
    Serial.flush();
    return;
  }

  tokens = sscanf(__TIME__, "%u:%u:%u", &hour, &minute, &second);
  if (tokens != 3) {
    Serial.println("Unable to parse time");
    Serial.flush();
    return;
  }
  
  DS3231M.adjust(DateTime(year, month, day, hour, minute, second));
  DateTime now = DS3231M.now();

  char buffer[100];
  sprintf(
    buffer,
    "The time is now %04d-%02d-%02d %02d:%02d:%02d",
    now.year(),
    now.month(),
    now.day(),
    now.hour(),
    now.minute(),
    now.second()
  );
  Serial.println(buffer);
}

void handle_set_command(char input_buffer[INPUT_BUFFER_SIZE]) {
  Serial.println("Setting time...");
  unsigned int tokens, year, month, day, hour, minute, second;
  tokens = sscanf(
    input_buffer,
    "SET %u-%u-%u %u:%u:%u",
    &year,
    &month,
    &day,
    &hour,
    &minute,
    &second
  );
  if (tokens != 6) {
    Serial.println("Unable to parse date, time.");
    Serial.flush();
    return;
  }

  DS3231M.adjust(DateTime(year, month, day, hour, minute, second));
  DateTime now = DS3231M.now();

  char buffer[100];
  sprintf(
    buffer,
    "The time is now %04d-%02d-%02d %02d:%02d:%02d",
    now.year(),
    now.month(),
    now.day(),
    now.hour(),
    now.minute(),
    now.second()
  );
  Serial.println(buffer);
}
