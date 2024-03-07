#define ledPinRed 13 // Pin number where the LED is connected
#define ledPinYellow 12 // Pin number where the LED is connected
#define ledPinBlue 11 // Pin number where the LED is connected

void setup() {
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
  Serial.begin(9600); // Set the baud rate to 9600
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      digitalWrite(ledPinRed, HIGH); // Turn on the LED
      Serial.println("LED RED ON");
      delay(100);
    } else if (command == '2') {
      digitalWrite(ledPinYellow, HIGH); // Turn on the LED
      Serial.println("LED YELLOW ON");
      delay(100);
    } else if (command == '3') {
      digitalWrite(ledPinBlue, HIGH); // Turn on the LED
      Serial.println("LED BLUE ON");
      delay(100);
    } else if (command == '4') {
      digitalWrite(ledPinRed, LOW); // Turn off the LED
      digitalWrite(ledPinBlue, LOW); // Turn off the LED
      digitalWrite(ledPinYellow, LOW); // Turn off the LED
      Serial.println("LEDS OFF");
      delay(100);
    }
  }
}
