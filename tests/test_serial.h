/**
 * @file test_serial.h
 * @brief Test declarations for serial communication
 */

#ifndef TEST_SERIAL_H
#define TEST_SERIAL_H

// Test suite functions
int run_serial_config_tests(void);
int run_serial_port_tests(void);
int run_serial_io_tests(void);

// Helper functions
void setup_test_environment(void);
void cleanup_test_environment(void);

#endif // TEST_SERIAL_H