#ifndef SERIAL_FUNCTIONS_H_
#define SERIAL_FUNCTIONS_H_

#if defined(__linux__)
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    typedef unsigned int size_;
    #define SIZE size_
    #define OPEN_SERIAL_PORT(path) open(path, O_RDWR | O_NOCTTY | O_NONBLOCK)
    #define CLOSE_SERIAL_PORT(fd) close(fd)
    #define FD int
    #define ERROR_FD -1;
    #define OPTIONS struct termios
    #define TIMEOUT struct timeval
    #define GET_SERIAL_PARAMS(fd, options) tcgetattr(fd, &options)
    #define SET_SERIAL_PARAMS(fd, options) tcsetattr(fd, TCSANOW, &options)
    #define BYTES_READ int
    #define ABLE_TO_GET_SERIAL_PORT_STATE 0
    #define ADJUST_OPTIONS_ATTRS(options)             \
            {                                         \
              options.c_cflag = CS8 | CREAD | CLOCAL; \
              options.c_iflag = IGNPAR;               \
              options.c_oflag = 0;                    \
              options.c_lflag = 0;                    \
              cfsetospeed(&options, B9600);           \
              cfsetispeed(&options, B9600);           \
            }
    #define UNABLE_TO_OPEN_PORT -1
    #define UNABLE_TO_READ UNABLE_TO_OPEN_PORT
    #define WRITE(fd, command, size_type) write(fd, command, size_type)
    #define READ(fd, buffer, buffer_size, bytes_read) bytes_read = read(fd, buffer, buffer_size)
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #define SIZE DWORD
    #define OPEN_SERIAL_PORT(path) CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)
    #define CLOSE_SERIAL_PORT(fd) CloseHandle(fd)
    #define UNABLE_TO_OPEN_PORT INVALID_HANDLE_VALUE
    #define UNABLE_TO_READ UNABLE_TO_OPEN_PORT
    #define OPTIONS DCB
    #define FD HANDLE
    #define ERROR_FD NULL
    #define GET_SERIAL_PARAMS(fd, options) GetCommState(fd, &options)
    #define SET_SERIAL_PARAMS(fd, options) SetCommState(fd, &options)
    #define ABLE_TO_GET_SERIAL_PORT_STATE TRUE
    #define ADJUST_OPTIONS_ATTRS(options)         \
            {                                     \
              options.BaudRate = CBR_9600;        \
              options.ByteSize = 8;               \
              options.StopBits = ONE_STOP_BIT;    \
              options.Parity   = NOPARITY;        \
            }
    #define TIMEOUT COMMTIMEOUTS
    #define BYTES_READ DWORD
    #define WRITE(fd, command, size_type) WriteFile(fd, command, 1, size_type ,NULL)
    #define READ(fd, buffer, buffer_size, bytes_read) ReadFile(fd, buffer, buffer_size, &bytes_read, NULL)
#endif

extern FD open_serial_port(const char* filename);
extern void close_serial_port(FD serial_port);
extern void serial_write(FD serial_port, char* command);
extern void serial_read(FD serial_port);

#endif // SERIAL_FUNCTIONS_H_
