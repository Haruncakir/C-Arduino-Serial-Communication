/**
 * @file serial_functions.h
 * @brief Cross-platform serial communication interface
 *
 * This header provides a platform-independent interface for serial communication
 * on both Linux and Windows systems.
 */

#ifndef SERIAL_FUNCTIONS_H_
#define SERIAL_FUNCTIONS_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Platform-specific includes and definitions */
#if defined(__linux__)
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    typedef int serial_handle_t;
    #define SERIAL_INVALID_HANDLE (-1)
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    typedef HANDLE serial_handle_t;
    #define SERIAL_INVALID_HANDLE INVALID_HANDLE_VALUE
#else
    #error "Unsupported platform"
#endif

/* Error codes */
enum serial_error_e {
    SERIAL_SUCCESS = 0,
    SERIAL_ERROR_OPEN = -1,
    SERIAL_ERROR_CONFIG = -2,
    SERIAL_ERROR_READ = -3,
    SERIAL_ERROR_WRITE = -4,
    SERIAL_ERROR_INVALID_HANDLE = -5
};

/* Serial port configuration structure */
struct serial_config_s {
    uint32_t baud_rate;
    uint8_t data_bits;
    uint8_t stop_bits;
    uint8_t parity;
};

/**
 * @brief Opens and configures a serial port
 * @param port_name Path to the serial port (e.g., "/dev/ttyUSB0" or "COM1")
 * @param config Pointer to configuration structure (NULL for default 9600-8N1)
 * @return Handle to the serial port or SERIAL_INVALID_HANDLE on error
 */
serial_handle_t serial_open(const char *port_name, const struct serial_config_s *config);

/**
 * @brief Closes a serial port
 * @param handle Valid serial port handle
 * @return SERIAL_SUCCESS or error code
 */
int serial_close(serial_handle_t handle);

/**
 * @brief Writes data to a serial port
 * @param handle Valid serial port handle
 * @param data Pointer to data buffer
 * @param size Size of data to write
 * @param bytes_written Pointer to store number of bytes written
 * @return SERIAL_SUCCESS or error code
 */
int serial_write(serial_handle_t handle, const void *data, size_t size, size_t *bytes_written);

/**
 * @brief Reads data from a serial port
 * @param handle Valid serial port handle
 * @param buffer Pointer to receive buffer
 * @param size Maximum size to read
 * @param bytes_read Pointer to store number of bytes read
 * @return SERIAL_SUCCESS or error code
 */
int serial_read(serial_handle_t handle, void *buffer, size_t size, size_t *bytes_read);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_FUNCTIONS_H_ */
