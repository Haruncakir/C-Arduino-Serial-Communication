#include <stdio.h>
#include "serial_functions.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s /dev/ttyACM0\n", argv[0]);
    return 1;
  }

  const char* portName = argv[1];
  int serialPort = open_serial_port(portName);

  for(;;) {
    char command;
    printf("Select an option:\n");
    printf("1-Turn on Red Led.\n");
    printf("2-Turn on Yellow Led.\n");
    printf("3-Turn on Blue Led.\n");
    printf("4-Turn off the Leds.\n");
    printf("5-Quit.\n");
    printf(">");
    scanf(" %c", &command);

    if (command == '5') {
      close_serial_port(serialPort);
      return 0;
    }

    if (command >= '1' && command <= '4') {
      serial_write(serialPort, &command);
      usleep(100000); // Wait for 100ms to allow the Arduino to process the command
      serial_read(serialPort);
    } else {
      printf("Invalid option. Please select 1-4 or 5 to quit.\n");
    }
  }
  return 0;
}
