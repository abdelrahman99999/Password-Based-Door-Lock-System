# Password-Based-Door-Lock-System
Embedded System Project based on AVR Architecture using layered architecture model. I used 2 ATmega16 microcontrollers ,one for Human Interface which connected to one keypad and one LCD. the second microcontroller for control system which connected to one Buzzer and one Motor and one EEPROM.

## Sequence of the Application
- **first time to the program** 
    you must set the Application password
    first, you should enter the desired password
    second,you should enter again the password to confirm it
    - if the 2 passwords are matched, the password is saved and you go the main options
    - else you should repeat this operation untill complete setting password
- **Main Options of the program**
    you can select from 2 options 
    - *Open The Door* --> to select this, press '+' then press 'enter'
    - *Change Application Password* --> to select this, press '-' then press 'enter'
- **Checking the password**
    after selcting an option, you should enter the password to check if you is the owner or thief
    - if you enter password correctly your option will execute 
        * if the option open the door, the door will be opening then hold for some time then closing. during this operation the state of door will show in the LCD then you will go to the main options
        * if the option change the password you should set password again as the first time you visit the program then you will go to the main options
    - if you enter password wrongly 
         * if it is the first try or second you will go the main options to try again
         * if it is the third try --> may Someone is trying to steal so we turn on buzzer for 1 minute and showing danger state on the LCD and then you will go to the main options

## Layered Architecture Model of the project
### Human Machine Interface ECU Layers
![image from program](https://github.com/abdelrahman99999/Password-Based-Door-Lock-System/blob/main/HMI%20and%20%20Control%20Layers/HMI_Layers.png?raw=true)
### Control ECU Layers
![image from program](https://github.com/abdelrahman99999/Password-Based-Door-Lock-System/blob/main/HMI%20and%20%20Control%20Layers/Control_Layers.png?raw=true)
## proteus simulation
![image from program](https://github.com/abdelrahman99999/Password-Based-Door-Lock-System/blob/main/project%20screenshots%20while%20working/3.png?raw=true)

## Main Hardware Components
- 2 ATmege16 
- 2*20 LCD
- 4*4 Keypad
- Led
- Buzzer
- Dc Motor
- External EEPROM
- Resistors
- L293D

## Implemented Drivers
- GPIO
- UART
- TIMER
- I2C
- LCD
- Keypad
- Buzzer
- Dc Motor
- EEPROM

## Tools
- Eclipse
- Proteus

## Demo
link: 
