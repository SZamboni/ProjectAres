# ProjectAres Introduction
Project Ares is a wearable videogame controller built with Arduino. The goal of the project is to be able to play Tekken 3 with your body, so when you do a kick or a punch also the character should do a kick or a punch. It is composed mainly by five gyroscope and accellerometer, so when the player makes a movement, like a kick or a punch, the Arduino code understand the movement and press a key on the PC keyboard, and Tekken 3 is running on an emulator on the PC.
The project is a collaboration between me and Michele Roselli (you can find him in the contributor page of GitHub).

# Bills of materials 
Hardware:
- Arduino-like controller
- 5 GY-521 gyroscope and accellerometers
- a lot af cables
- wood and strings to attach to the body the GY-521 sensors
- a long USB cable ( > 2m long) to connect Arduino to the PC

Software:
- a PC with the Arduino IDE
- the library Wire.h in the Arduino IDE (it is usually already present in the Arduino IDE)
- the library MPU6050.h (https://github.com/jarzebski/Arduino-MPU6050) installed in the Arduino IDE
- the Processing 3.4 IDE 
- the libraries processing.serial and java.awt.Robot installed in the Processing IDE (they are usually alreay present in the Processing IDE)

# Hardware manufacturing
Screw the five GY-521 sensors on a small piece of wood (7cmx4cm is ok). Then you have to came up with a way to attach these piece of wood to your body, we have used strings.
The Arduino has to be attatched to the player body, we screwd the Arduino on a wood board and then attach the wood board to a belt.
Then cables have to connect the Arduino to every sensor, following the wirings explained in the wirings section of this guide. Then you need a long USB cable to connect the Arduino to the PC in order to play.

# Wirings 
![alt text](https://github.com/SZamboni/ProjectAres/blob/master/Wirings.png)
The wirings is very simple: VCC, GND, SDA, SCL for every sensor are in parallel with all the other sensors and ther attatched to Arduino.
Then every sensor has its AD0 pin attatched to a different pin to Arduino (Right Leg Sensor -> pin 2 Arduino, Left Leg Sensor -> pin 3 Arduino, Head Sensor -> pin 4 Arduino, Right Arm Sensor -> pin 5 Arduino, Left Arm Sensor -> pin 6 Arduino)

# Final product
The final product shoul look something like this when dressed:
![alt text](https://github.com/SZamboni/ProjectAres/blob/master/project%20Ares%20hardware%20image.jpg)
As you can see there are 5 sensors, one for each arm, one for each leg and one on the head, attached to a running band. Every sensor is connected to the Arduino following the previously explained wirings and fos stability porupouse we weld all the connection to the wirings and covered the welding with insulating tape.

# Software instruction
Attach the Arduino to the USB port on your computer, open the Aruduino IDE, load the Project_Ares_Arduino.ino, and load the program on the Arduino. DO NOT OPEN THE SERIAL MONITOR.
Open the processing IDE and load the processing_desktop.pde program, and run it.
Open notepad. When you do a left punch you should see the c character appear on the screen.
If you have some problems check the COM port open in the Processing IDE.

# Keys setup
The default keys configuration is the following:
- Right Arm = v key
- Left Arm = c key
- Right Leg = x key
- Left Leg = z key
- Head Up = w key
- Head Down = s key
- Head Left = a key
- Head Right = d key

When you make a movement with a limb the key is pressed and released after 0.1sec, meanwhile when you move the head the key is pressed and released when your head returns in the starting position.

# Play tekken 3
To play tekken 3 with this controller you have to download a PS1 emulator, its BIOS and Tekken 3. Note that this procedure may be illegal. 
Then you have to setup the controller keys:
- v key = square on the PS1 emulator
- c key = triangle on the PS1 emulator
- x key = X on the PS1 emulator
- z key = circle on the PS1 emulator
- w key = Arrow Up on the PS1 emulator
- s key = Arrow Down on the PS1 emulator
- a key = Arrow Left on the PS1 emulator
- d key = Arrow Right on the PS1 emulator

Then you can start play the videogame and when you do a left punch the character should do a left punch, and when you move the head foreward or backward the character should move foreward or backward.
