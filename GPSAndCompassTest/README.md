This is a test project for the Beitian BN-880Q GPS and Compass module, attached currently to an Arduino Mega2560.

The wiring from the Beitian BN-880Q to the Arduino is as follows:

- **SDL	=>	SDA 20**
- **GND	=>	Ground Pin**
- **TX	=>	RX 15**
- **RX	=>	TX 14**
- **VCC	=>	VIN (+5V)**
- **SCL	=>	SCL 21**


This project utilizes a header file called GPSHelper I created just as a go-between from an Arduino project to the TinyGPSPlus library created by Mikal Hart.

If you are utilizing another Arduino model, you may not have a Serial3 connection, and therefore you might have to utilize Serial1 or Serial2 instead, and change pins accordingly. A forewarning, if you utilize Serial (pins 0 and 1), you can not have the connection when the Arduino first starts up, as anything connected on pin 0 will stall the program. A work-around is to connect the pin only AFTER the program has started, which I avoided by using Serial3 (but Serial1 and Serial2 would also work).