#include "serial_functions.h"
#include <stdio.h>
#include <string.h>

FD open_serial_port(const char* portname) {
  FD serial_port = OPEN_SERIAL_PORT(portname);
  if(serial_port == UNABLE_TO_OPEN_PORT) {
    fprintf(stderr, "Unable to open serial port.\n");
    return ERROR_FD;
  }

  OPTIONS options;
  memset(&options, 0, sizeof(options));

  if(GET_SERIAL_PARAMS(serial_port, options) != ABLE_TO_GET_SERIAL_PORT_STATE) {
    CLOSE_SERIAL_PORT(serial_port);
    return ERROR_FD;
  }

  ADJUST_OPTIONS_ATTRS(options);

  if(SET_SERIAL_PARAMS(serial_port, options) != ABLE_TO_GET_SERIAL_PORT_STATE) {
    CLOSE_SERIAL_PORT(serial_port);
    return ERROR_FD;
  }

  return serial_port;
}

void close_serial_port(FD serial_port) {
  CLOSE_SERIAL_PORT(serial_port);
}

void serial_write(FD serial_port, char* command) {
  WRITE(serial_port, command, strlen(command));
}

void serial_read(FD serial_port) {
  char buffer[256];
  BYTES_READ bytes_read;
  if(READ(serial_port, buffer, sizeof(buffer) - 1, bytes_read) == UNABLE_TO_READ) {
    CLOSE_SERIAL_PORT(serial_port);
    return;
  }
  buffer[bytes_read] = '\0';
  printf("Arduino response: %s\n", buffer);
}
