# RFID-chip-interfacing
Radio-frequency identification (RFID) uses electromagnetic fields to automatically identify and track tags attached to objects. When triggered by an electromagnetic interrogation pulse from a nearby RFID reader device, the tag transmits digital data, usually an identifying inventory number, back to the reader.

# Components Required
* Arduino UNO x1
* OLED display x1
* RFID kits
* Breadboard
* Connecting wires

# Connect Set up
There are two conponents. One is the RFID and the other one OLED. You can look the connection in the code.

# Run
AVR toolchain will convert "main.c" to "main.hex" than upload to Arduino.
You might need to test your RFID kits first to make sure it works.
We can use the AVR toolchain to convert program to hex file and then upload to the Arduino. I also uploaded my hex file. 
Please pay to attention to the connection and make it correct. Don't connect the RFID Vcc to 5V, it MUST connect to the 3.3V.

# Important things
1. Make sure you have a good work sensor and test before you go to next step. Or not you will have trouble when you meeting some problems.
2. From my experience, you can but the kits on Amazon and I think it is the best way to buy it.
3. According to the connection.txt, make sure wire the conponents in right position.


# Demo link
video demo link: https://youtu.be/1np3dZ6B0pc
