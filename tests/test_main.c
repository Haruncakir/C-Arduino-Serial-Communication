/**
 * @file test_main.c
 * @brief Main test runner for serial communication tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "test_serial.h"

int main(void) {
    printf("Running serial communication tests...\n\n");

    // Run all test suites
    int failed = 0;
    failed += run_serial_config_tests();
    failed += run_serial_port_tests();
    failed += run_serial_io_tests();

    // Report results
    if (failed == 0) {
        printf("\nAll tests passed successfully!\n");
        return EXIT_SUCCESS;
    } else {
        printf("\nTests failed: %d\n", failed);
        return EXIT_FAILURE;
    }
}