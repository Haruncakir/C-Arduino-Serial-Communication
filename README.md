# Arduino Serial LED Control

A cross-platform C program to control multiple LEDs on an Arduino board via serial communication. This project demonstrates basic serial communication between a computer and Arduino using a simple LED control interface.

## Hardware Requirements

- Arduino Uno/Nano/Pro Mini
- Breadboard
- 3 LEDs (any colors)
- 3 × 330Ω resistors
- 7 jumper wires
- USB cable for Arduino connection

## Circuit Setup

1. Connect components on breadboard:
   - Connect LED anodes (longer leg) to Arduino digital pins through 330Ω resistors
   - Connect LED cathodes (shorter leg) to Arduino GND
   - Default pin configuration:
     - Red LED: Digital Pin 13
     - Yellow LED: Digital Pin 12
     - Blue LED: Digital Pin 11

See the complete circuit diagram and simulation at: [Wokwi Arduino Simulator](wokwi.txt)

## Software Setup

### 1. Arduino Setup

1. Open Arduino IDE
2. Load the Arduino sketch from `arduino/led_control.ino`
3. Verify pin assignments match your physical setup
4. Upload sketch to Arduino

### 2. Build C Program

```bash
# Clone repository
git clone https://github.com/Haruncakir/C-Arduino-Serial-Communication.git
cd C-Arduino-Serial-Communication/src

# Build program
gcc -o led_control main.c serial_functions.c
```

### 3. Run Program

```bash
# Linux
./led_control /dev/ttyACM0

# Windows
./led_control COM3
```

## Troubleshooting

### Linux Permission Issues

If you encounter: `avrdude: ser_open(): can't open device "/dev/ttyUSB0": Permission denied`

```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER

# Log out and log back in for changes to take effect
# Or restart system
```

Finding your port:
```bash
# List all USB serial devices
ls /dev/tty{USB,ACM}*

# Get detailed USB device information
lsusb
udevadm info -q property -n /dev/ttyACM0
```

### Windows Permission Issues

If you see: `avrdude: ser_open(): can't open device "COM1": Access is denied`

1. Open Device Manager (Win + X → Device Manager)
2. Locate Ports (COM & LPT)
3. Right-click Arduino port → Properties
4. Port Settings → Advanced → Change Permissions
5. Add your user account with Full Control

Finding your port:
1. Device Manager → Ports (COM & LPT)
2. Note the COM port number for Arduino

## Program Usage

After launching, the program presents a menu:
```
Select an option:
1-Turn on Red Led
2-Turn on Yellow Led
3-Turn on Blue Led
4-Turn off the Leds
5-Quit
>
```

Enter a number (1-5) and press Enter to control LEDs.

## Development

### Building from Source

Requirements:
- GCC compiler
- Make (optional)
- Git

```bash
# Clone with submodules
git clone --recursive https://github.com/Haruncakir/C-Arduino-Serial-Communication.git

# Build manually
gcc -Wall -Wextra -o led_control main.c serial_functions.c

# Or use make
make
```

### Running Tests

```bash
# Build and run tests
make test

# Run specific test
./tests/test_serial
```

## Contributing

1. Fork repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.
