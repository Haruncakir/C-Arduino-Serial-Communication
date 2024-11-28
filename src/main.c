/**
 * @file main.c
 * @brief LED control program using serial communication
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/serial_functions.h"

#if defined(__linux__)
    #include <unistd.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#elif defined(_WIN32)
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#endif

#define COMMAND_BUFFER_SIZE 2
#define READ_BUFFER_SIZE 256
#define SERIAL_DELAY_MS 100

static const char* const MENU_OPTIONS[] = {
    "1-Turn on Red Led",
    "2-Turn on Yellow Led",
    "3-Turn on Blue Led",
    "4-Turn off the Leds",
    "5-Quit"
};

static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void display_menu(void) {
    printf("\nSelect an option:\n");
    for (size_t i = 0; i < sizeof(MENU_OPTIONS) / sizeof(MENU_OPTIONS[0]); i++) {
        printf("%s\n", MENU_OPTIONS[i]);
    }
    printf("> ");
    fflush(stdout);
}

static int process_command(serial_handle_t serial_port, char command) {
    size_t bytes_written, bytes_read;
    char read_buffer[READ_BUFFER_SIZE];
    
    /* Validate command range */
    if (command == '5') {
        return 0;  /* Exit requested */
    }
    
    if (command < '1' || command > '4') {
        printf("Invalid option. Please select 1-4 or 5 to quit.\n");
        return 1;
    }

    /* Send command to device */
    if (serial_write(serial_port, &command, 1, &bytes_written) != SERIAL_SUCCESS || bytes_written != 1) {
        fprintf(stderr, "Failed to send command to device\n");
        return -1;
    }

    /* Wait for device to process command */
    SLEEP_MS(SERIAL_DELAY_MS);

    /* Read response */
    if (serial_read(serial_port, read_buffer, READ_BUFFER_SIZE - 1, &bytes_read) != SERIAL_SUCCESS) {
        fprintf(stderr, "Failed to read device response\n");
        return -1;
    }

    if (bytes_read > 0) {
        read_buffer[bytes_read] = '\0';
        printf("Arduino response: %s", read_buffer);
    }

    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <serial_port>\n", argv[0]);
        fprintf(stderr, "Example: %s /dev/ttyACM0\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Configure serial port */
    struct serial_config_s config = {
        .baud_rate = 9600,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0
    };

    /* Open serial port */
    serial_handle_t serial_port = serial_open(argv[1], &config);
    if (serial_port == SERIAL_INVALID_HANDLE) {
        fprintf(stderr, "Error: Unable to open serial port %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Connected to %s\n", argv[1]);

    /* Main program loop */
    int running = 1;
    char command;

    while (running) {
        display_menu();
        
        if (scanf(" %c", &command) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        int result = process_command(serial_port, command);
        if (result < 0) {
            /* Error occurred */
            fprintf(stderr, "Error during communication with device\n");
            break;
        } else if (result == 0) {
            /* Exit requested */
            running = 0;
        }
    }

    /* Cleanup */
    if (serial_close(serial_port) != SERIAL_SUCCESS) {
        fprintf(stderr, "Warning: Error while closing serial port\n");
    }

    printf("Program terminated.\n");
    return EXIT_SUCCESS;
}
