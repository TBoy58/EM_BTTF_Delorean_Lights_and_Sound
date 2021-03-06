# EM_BTTF_Delorean_Lights_and_Sound
 Light and sound effects for the Eaglemoss Back To The Future Delorean model car.

Main Parts:

Arduino Mega 2650 Pro Mini x 1 (Alliexpress),

Mounting for Arduino Mega x 1 (Shapeways),

Arduino Pro Mini x 1 (Driver's Console) (Alliexpress),

DfRobot Beetle x 1 (Reactor) (Pi Hut)

DFPlayer Mini MP3 player (Amazon),

PAM8403 Stereo Amplifier (Amazon),

AIYIMA Speakers 8 Ohm 5W x 2 (Alliexpress),

DSD TECH HM-17 Bluetooth 4.1 Module (Amazon),

CJMCU 12 Bit WS2812 5050 RGB LED Ring (Reactor)(Banggood),

DC Adaptor 9V 2A,

The shops in brackets are where I bought the items. Shop around :)


Speedometer:

Adafruit 0.56" 4-Digit 7-Segment Display w/I2C Backpack (Pimoroni)

Flux Bands:
Inverter - 9v Potted - 100cm2 No Switch
3-way Standard Splitter
3 x MAXI 1.5mm x 1m EL wire
NOTE - the above items were bought from EL Tape and Wire (address below).
	Alternatively the EL Mod can be bought from ModelModz.

Modelmodz Parts:

Flux Capacitor,

Xmas Tree,

Dash Display,

Center Console,

EL Wire Mod.


This system is controlled by an Bluetooth Android app created in App Inventor 2(ai2)
(http://ai2.appinventor.mit.edu/)
Two App Inventor 2 files are provide, an ai2 file to allow changes (.aia) and an Android 
file ready for loading on to a phone (.apk).
There is no IOS app available.

In the sounds folder, the mp3 folder needs to be placed on to an empty sd card 
for the DFPlayer.

The radio tracks are numbered in the same format as the car sounds from
0021 to 0040 (20 tracks) and placed in the mp3 folder.
Radio tracks are not supplied.

Mega Pro Mini Pin Allocations

D02 - LEFT SIDELIGHT

D03 - RIGHT SIDELIGHT

D04 - 

D05 - 

D06 - 

D07 - FLUXBAND (EL Wire)

D08 - FC_ONE (Flux capacitor outer lights)

D09 - FC_TWO

D10 - FC_THREE

D11 - FC_FOUR (Flux capacitor inner lights)

D12 - FC_MID (Flux capacitor center light)

D13 - 

D14 - 

D15 - 

D16 - TC_OFF (Time circuits OFF light, center console)

D17 - DFPLAYER BUSY LED (Radio light)

D18 - DFPLAYER RX

D19 - DFPLAYER TX

D20 - SDA (I2C Data)

D21 - SCL (I2C Clock)
 
D22 - DFPLAYER BUSY

D23 - MAIN LIGHTS

D24 - FULL BEAM

D25 - REAR LIGHTS

D26 - BRAKE LIGHTS

D27 - REVERSE LIGHTS

D28 - RED/AMBER/GREEN KEYPAD

D29 - WHITE KEYPAD

D30 - PLATE LIGHTS

D31 - 

D32 - TC_ON (Time circuits ON light, center console)

D33 - TCD (Time Circuits Display)

D34 - 88MPH LIGHT

D35 - P_EMPTY (Plutonium display empty light)

D36 - DASH BACKLIGHT

D37 - PSU

D38 - PLUTONIUM DISPLAY BACKLIGHT

D39 - L_BLINK (Left rear indicator)

D40 - R_BLINK (Right rear indicator)

D41 - 

D42 - 

D43 - 

D44 - DIGITAL USE ONLY - NOT PWM

D45 - DIGITAL USE ONLY - NOT PWM

D46 - HM17 RX PIN (Bluetooth module)

D47 - COURTESY LIGHT

D48 - HM17 TX PIN (Bluetooth module)

D49 - ENGINE COMPARTMENT LIGHT

D50 - DOOR 1 LIGHTS

D51 - DOOR 2 LIGHTS

D52 - FRONT HOOD LIGHT

D53 - ROOF LIGHT


A0  - BRAKE SWITCH

A1  - 

A2  - ACCELERATOR SWITCH

A3  - 

A4  - 

A5  - 

A6  - DOOR SWITCH 1

A7  - 

A8  -

A9  -

A10 - DOOR SWITCH 2 

A11 - 

A12 - 

A13 - 

A14 - FRONT HOOD SWITCH

A15 - ENGINE COMPARTMENT SWITCH


References:

Modelmodz store:
https://modelmodz.com/

Concurrent programming - 
https://learn.adafruit.com/multi-tasking-the-arduino-part-1/ditch-the-delay

Bluetooth Low Energy -
http://www.martyncurrey.com/arduino-hm-10-and-app-inventor-2/

MIT App Inventor 2 -
https://appinventor.mit.edu

FaceBook Demo -
https://www.facebook.com/noci50/videos/10156699960936765/

EL Wire for Flux Bands - 
https://elpanelandtape.co.uk/

DfRobot Beetle - 
https://wiki.dfrobot.com/Beetle_SKU_DFR0282

Shapeways - 
https://www.shapeways.com/product/8V7J5HJEP/1-8-bttf-delorean-bulkhead-hatch-for-arduino-mega?etId=189700217&option=86395156&utm_campaign=payment-received&utm_content=7&utm_medium=email&utm_source=automated-contact


