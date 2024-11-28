/**
 * @file serial_functions.c
 * @brief Implementation of cross-platform serial communication interface
 */

#include "../include/serial_functions.h"
#include <errno.h>
#include <string.h>

#if defined(__linux__)
    static const int BAUD_RATES[] = {B9600, B19200, B38400, B57600, B115200};
    static const int BAUD_VALUES[] = {9600, 19200, 38400, 57600, 115200};
    #define NUM_BAUD_RATES (sizeof(BAUD_RATES) / sizeof(BAUD_RATES[0]))
#endif

/* Default configuration (9600-8N1) */
static const struct serial_config_s DEFAULT_CONFIG = {
    .baud_rate = 9600,
    .data_bits = 8,
    .stop_bits = 1,
    .parity = 0
};

#if defined(__linux__)
static int get_baud_const(uint32_t baud_rate) {
    for (size_t i = 0; i < NUM_BAUD_RATES; i++) {
        if (BAUD_VALUES[i] == baud_rate) {
            return BAUD_RATES[i];
        }
    }
    return B9600; /* Default to 9600 if unsupported */
}

static int configure_port(serial_handle_t handle, const struct serial_config_s *config) {
    struct termios options;

    if (tcgetattr(handle, &options) != 0) {
        return SERIAL_ERROR_CONFIG;
    }

    /* Configure baud rate */
    int baud = get_baud_const(config->baud_rate);
    if (cfsetispeed(&options, baud) != 0 || cfsetospeed(&options, baud) != 0) {
        return SERIAL_ERROR_CONFIG;
    }

    /* Configure data bits */
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= (config->data_bits == 7) ? CS7 : CS8;

    /* Configure stop bits */
    if (config->stop_bits == 2) {
        options.c_cflag |= CSTOPB;
    } else {
        options.c_cflag &= ~CSTOPB;
    }

    /* Configure parity */
    if (config->parity) {
        options.c_cflag |= PARENB;
        options.c_cflag |= (config->parity == 2) ? PARODD : 0;
    } else {
        options.c_cflag &= ~PARENB;
    }

    /* Raw mode */
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL);
    options.c_oflag &= ~OPOST;

    /* Apply settings */
    if (tcsetattr(handle, TCSANOW, &options) != 0) {
        return SERIAL_ERROR_CONFIG;
    }

    return SERIAL_SUCCESS;
}

#elif defined(_WIN32)
static int configure_port(serial_handle_t handle, const struct serial_config_s *config) {
    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(handle, &dcb)) {
        return SERIAL_ERROR_CONFIG;
    }

    dcb.BaudRate = config->baud_rate;
    dcb.ByteSize = config->data_bits;
    dcb.StopBits = (config->stop_bits == 2) ? TWOSTOPBITS : ONESTOPBIT;
    dcb.Parity = config->parity;

    if (!SetCommState(handle, &dcb)) {
        return SERIAL_ERROR_CONFIG;
    }

    /* Configure timeouts */
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(handle, &timeouts)) {
        return SERIAL_ERROR_CONFIG;
    }

    return SERIAL_SUCCESS;
}
#endif

serial_handle_t serial_open(const char *port_name, const struct serial_config_s *config) {
    if (!port_name) {
        return SERIAL_INVALID_HANDLE;
    }

    const struct serial_config_s *cfg = config ? config : &DEFAULT_CONFIG;

#if defined(__linux__)
    serial_handle_t handle = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (handle == SERIAL_INVALID_HANDLE) {
        return SERIAL_INVALID_HANDLE;
    }
#elif defined(_WIN32)
    serial_handle_t handle = CreateFileA(port_name,
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL);
    if (handle == SERIAL_INVALID_HANDLE) {
        return SERIAL_INVALID_HANDLE;
    }
#endif

    if (configure_port(handle, cfg) != SERIAL_SUCCESS) {
        serial_close(handle);
        return SERIAL_INVALID_HANDLE;
    }

    return handle;
}

int serial_close(serial_handle_t handle) {
    if (handle == SERIAL_INVALID_HANDLE) {
        return SERIAL_ERROR_INVALID_HANDLE;
    }

#if defined(__linux__)
    return close(handle) == 0 ? SERIAL_SUCCESS : SERIAL_ERROR_CONFIG;
#elif defined(_WIN32)
    return CloseHandle(handle) ? SERIAL_SUCCESS : SERIAL_ERROR_CONFIG;
#endif
}

int serial_write(serial_handle_t handle, const void *data, size_t size, size_t *bytes_written) {
    if (handle == SERIAL_INVALID_HANDLE || !data || !bytes_written) {
        return SERIAL_ERROR_INVALID_HANDLE;
    }

#if defined(__linux__)
    ssize_t result = write(handle, data, size);
    if (result < 0) {
        *bytes_written = 0;
        return SERIAL_ERROR_WRITE;
    }
    *bytes_written = (size_t)result;
#elif defined(_WIN32)
    DWORD written;
    if (!WriteFile(handle, data, (DWORD)size, &written, NULL)) {
        *bytes_written = 0;
        return SERIAL_ERROR_WRITE;
    }
    *bytes_written = written;
#endif

    return SERIAL_SUCCESS;
}

int serial_read(serial_handle_t handle, void *buffer, size_t size, size_t *bytes_read) {
    if (handle == SERIAL_INVALID_HANDLE || !buffer || !bytes_read) {
        return SERIAL_ERROR_INVALID_HANDLE;
    }

#if defined(__linux__)
    ssize_t result = read(handle, buffer, size);
    if (result < 0) {
        *bytes_read = 0;
        return (errno == EAGAIN) ? SERIAL_SUCCESS : SERIAL_ERROR_READ;
    }
    *bytes_read = (size_t)result;
#elif defined(_WIN32)
    DWORD read_count;
    if (!ReadFile(handle, buffer, (DWORD)size, &read_count, NULL)) {
        *bytes_read = 0;
        return SERIAL_ERROR_READ;
    }
    *bytes_read = read_count;
#endif

    return SERIAL_SUCCESS;
}
