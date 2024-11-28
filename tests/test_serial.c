/**
 * @file test_serial.c
 * @brief Implementation of serial communication tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "test_serial.h"
#include "../include/serial_functions.h"

#if defined(__linux__)
    #include <fcntl.h>
    #include <unistd.h>
    #define TEST_PORT "/dev/ttyUSB0"
#elif defined(_WIN32)
    #include <windows.h>
    #define TEST_PORT "COM1"
#endif

// Mock serial port for testing
static serial_handle_t mock_port = SERIAL_INVALID_HANDLE;

void setup_test_environment(void) {
    // Setup mock environment before each test
    printf("Setting up test environment...\n");
}

void cleanup_test_environment(void) {
    // Cleanup after each test
    if (mock_port != SERIAL_INVALID_HANDLE) {
        serial_close(mock_port);
        mock_port = SERIAL_INVALID_HANDLE;
    }
    printf("Test environment cleaned up.\n");
}

// Test serial port configuration
int run_serial_config_tests(void) {
    int failed = 0;
    printf("Running configuration tests...\n");

    // Test default configuration
    struct serial_config_s config = {
        .baud_rate = 9600,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0
    };

    // Test invalid configurations
    struct serial_config_s invalid_config = {
        .baud_rate = 0,
        .data_bits = 0,
        .stop_bits = 0,
        .parity = 255
    };

    serial_handle_t port = serial_open(TEST_PORT, &config);
    if (port != SERIAL_INVALID_HANDLE) {
        printf("FAIL: Opened non-existent port\n");
        failed++;
        serial_close(port);
    } else {
        printf("PASS: Correctly rejected non-existent port\n");
    }

    port = serial_open(NULL, &config);
    if (port != SERIAL_INVALID_HANDLE) {
        printf("FAIL: Opened with NULL port name\n");
        failed++;
        serial_close(port);
    } else {
        printf("PASS: Correctly rejected NULL port name\n");
    }

    port = serial_open(TEST_PORT, &invalid_config);
    if (port != SERIAL_INVALID_HANDLE) {
        printf("FAIL: Opened with invalid configuration\n");
        failed++;
        serial_close(port);
    } else {
        printf("PASS: Correctly rejected invalid configuration\n");
    }

    return failed;
}

// Test serial port operations
int run_serial_port_tests(void) {
    int failed = 0;
    printf("\nRunning port operation tests...\n");

    // Test invalid handle operations
    if (serial_close(SERIAL_INVALID_HANDLE) == SERIAL_SUCCESS) {
        printf("FAIL: Closed invalid handle\n");
        failed++;
    } else {
        printf("PASS: Correctly rejected invalid handle close\n");
    }

    size_t bytes_written;
    if (serial_write(SERIAL_INVALID_HANDLE, "test", 4, &bytes_written) == SERIAL_SUCCESS) {
        printf("FAIL: Wrote to invalid handle\n");
        failed++;
    } else {
        printf("PASS: Correctly rejected write to invalid handle\n");
    }

    size_t bytes_read;
    char buffer[10];
    if (serial_read(SERIAL_INVALID_HANDLE, buffer, sizeof(buffer), &bytes_read) == SERIAL_SUCCESS) {
        printf("FAIL: Read from invalid handle\n");
        failed++;
    } else {
        printf("PASS: Correctly rejected read from invalid handle\n");
    }

    return failed;
}

// Test serial I/O operations
int run_serial_io_tests(void) {
    int failed = 0;
    printf("\nRunning I/O operation tests...\n");

    // Test NULL buffer handling
    size_t bytes_written, bytes_read;
    serial_handle_t port = serial_open(TEST_PORT, NULL);  // Use default config

    if (port != SERIAL_INVALID_HANDLE) {
        if (serial_write(port, NULL, 10, &bytes_written) == SERIAL_SUCCESS) {
            printf("FAIL: Wrote NULL buffer\n");
            failed++;
        } else {
            printf("PASS: Correctly rejected NULL write buffer\n");
        }

        if (serial_read(port, NULL, 10, &bytes_read) == SERIAL_SUCCESS) {
            printf("FAIL: Read into NULL buffer\n");
            failed++;
        } else {
            printf("PASS: Correctly rejected NULL read buffer\n");
        }

        serial_close(port);
    }

    // Test zero-length operations
    port = serial_open(TEST_PORT, NULL);
    if (port != SERIAL_INVALID_HANDLE) {
        if (serial_write(port, "test", 0, &bytes_written) != SERIAL_SUCCESS || bytes_written != 0) {
            printf("FAIL: Zero-length write failed\n");
            failed++;
        } else {
            printf("PASS: Zero-length write handled correctly\n");
        }

        char buffer[10];
        if (serial_read(port, buffer, 0, &bytes_read) != SERIAL_SUCCESS || bytes_read != 0) {
            printf("FAIL: Zero-length read failed\n");
            failed++;
        } else {
            printf("PASS: Zero-length read handled correctly\n");
        }

        serial_close(port);
    }

    return failed;
}