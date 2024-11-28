/**
 * @file test_mock_serial.c
 * @brief Mock implementation of serial port for testing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/serial_functions.h"

// Mock serial port structure
typedef struct {
    int is_open;
    struct serial_config_s config;
    char rx_buffer[1024];
    size_t rx_buffer_size;
    size_t rx_buffer_pos;
    char tx_buffer[1024];
    size_t tx_buffer_size;
} mock_serial_port_t;

static mock_serial_port_t mock_port = {0};

// Initialize mock port
void mock_serial_init(void) {
    memset(&mock_port, 0, sizeof(mock_port));
}

// Clean up mock port
void mock_serial_cleanup(void) {
    memset(&mock_port, 0, sizeof(mock_port));
}

// Simulate data reception
void mock_serial_push_rx_data(const char* data, size_t size) {
    if (size > sizeof(mock_port.rx_buffer) - mock_port.rx_buffer_size) {
        size = sizeof(mock_port.rx_buffer) - mock_port.rx_buffer_size;
    }
    memcpy(mock_port.rx_buffer + mock_port.rx_buffer_size, data, size);
    mock_port.rx_buffer_size += size;
}

// Get transmitted data
size_t mock_serial_get_tx_data(char* buffer, size_t size) {
    if (size > mock_port.tx_buffer_size) {
        size = mock_port.tx_buffer_size;
    }
    memcpy(buffer, mock_port.tx_buffer, size);
    mock_port.tx_buffer_size = 0;
    return size;
}

// Mock implementation of serial_open
serial_handle_t mock_serial_open(const char* port_name, const struct serial_config_s* config) {
    if (!port_name || mock_port.is_open) {
        return SERIAL_INVALID_HANDLE;
    }
    
    mock_port.is_open = 1;
    if (config) {
        memcpy(&mock_port.config, config, sizeof(struct serial_config_s));
    }
    
    return (serial_handle_t)&mock_port;
}

// Mock implementation of serial_close
int mock_serial_close(serial_handle_t handle) {
    if (handle != (serial_handle_t)&mock_port || !mock_port.is_open) {
        return SERIAL_ERROR_INVALID_HANDLE;
    }
    
    mock_port.is_open = 0;
    return SERIAL_SUCCESS;
}

// Mock implementation of serial_write
int mock_serial_write(serial_handle_t handle, const void* data, size_t size, size_t* bytes_written) {
    if (handle != (serial_handle_t)&mock_port || !mock_port.is_open || !data || !bytes_written) {
        return SERIAL_ERROR_INVALID_HANDLE;
    }
    
    if (size > sizeof(mock_port.tx_buffer) - mock_port.tx_buffer_size) {
        size = sizeof(mock_port.tx_buffer) - mock_port.tx_buffer_size;
    }
    
    memcpy(mock_port.tx_buffer + mock_port.tx_buffer_size, data, size);
    mock_port.tx_buffer_size += size;
    *bytes_written = size;
    
    return SERIAL_SUCCESS;
}

// Mock implementation of serial_read
int mock_serial_read(serial_handle_t handle, void* buffer, size_t size, size_t* bytes_read) {
    if (handle != (serial_handle_t)&mock_port || !mock_port.is_open || !buffer || !bytes_read) {
        return SERIAL_ERROR_INVALID_HANDLE;
    }
    
    if (size > mock_port.rx_buffer_size - mock_port.rx_buffer_pos) {
        size = mock_port.rx_buffer_size - mock_port.rx_buffer_pos;
    }
    
    memcpy(buffer, mock_port.rx_buffer + mock_port.rx_buffer_pos, size);
    mock_port.rx_buffer_pos += size;
    *bytes_read = size;
    
    return SERIAL_SUCCESS;
}