# C-Arduino-Serial-Communication

**Requirements:**
- Arduino Uno
- Breadboard
- 3 LEDs
- 3 330ohm resistors
- 7 jumper wires

[Visit the website for the Arduino build](wokwi.txt).

**How to execute:**

1. Prepare your Arduino. (Visit the website in the wokwi.txt)
2. Upload the Arduino code to your Arduino port. (Ensure to check the LED pins and their connections. They are defined at the beginning of the Arduino code. Change them if necessary.)

**For Linux users:** If you encounter an error when uploading the sketch to the Arduino:
avrdude: ser_open(): can't open device "/dev/ttyUSB0": Permission denied
Failed uploading: uploading error: exit status 1
Ensure that the user you're using has permission to access the serial port. You can add your user to the `dialout` group, which typically has access to serial ports:
```zsh
sudo usermod -a -G dialout $USER
```
After running this command, restart your Arduino IDE.

To compile and run the C code:
```bash
gcc -o main main.c serial_functions.c && ./main your_port_name
```

You can find your port name in Linux with:
```bash
ls /dev/ttyUSB*
```
If you see multiple ports, try using the one that corresponds to your Arduino.

**For Windows users:** If you encounter an error when uploading the sketch to the Arduino:
avrdude: ser_open(): can't open device "COM1": Access is denied.

To fix this issue, you can grant your user account permission to access the COM port:

1. Right-click on the Start menu and select "Device Manager".
2. In the Device Manager window, expand the "Ports (COM & LPT)" section.
3. Right-click on the COM port that corresponds to your Arduino and select "Properties".
4. Go to the "Port Settings" tab and click on "Advanced".
5. In the Advanced Settings window, click on "Change Permissions...".
6. Click on "Add..." and enter your Windows username, then click "Check Names" to verify.
7. Click "OK" to add your user account to the list, then select it and click "Edit...".
8. Check the "Allow" box for "Full control" and click "OK" to apply the changes.
9. Click "OK" again to close the Properties window.

After applying these changes, try uploading the sketch to your Arduino again.


**To compile and run the C code:** Same as mentioned earlier for Linux users.
