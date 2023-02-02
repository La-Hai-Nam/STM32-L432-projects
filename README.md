# STM32-L432-projects
by La Hai Nam
# Overview
All projects in this repository are currently part of projects I have done for my college. Both college and home projects will be added in the future.
## Content
 1.  blinky: Simple source code which toggles the inbuild LED to blink
 2.  helloworld: Simple source code which transmits a string "Hello World." through USART.
 3.  morsecode: Source code which takes in a string and converts it to morse code. The morse code will then be displayed through toggling the inbuild LED.
 4.  blinkytng: Simple source code which toggles an external LED.
    - requirement: red LED
 5.  trafficlight: Source code which simulates a simple trafficlight with 3 LEDs.
    - requirement: red, yellow and red LED
 6.  trafficlight_blinky: Project 5 with the addition of enabling an out-of-order-state thorugh a button.
    - requirement: red, yellow, red LED and button
 7.  melody: Source code which plays different music(credit to [robsoncouto](https://github.com/robsoncouto/arduino-songs)) through a passive buzzer.
    - requirement: passive buzzer 
 8.  colorcycle: Source code which toggles a RGB LED to display different colors.
    - requirement: RGB LED
 9.  Jukebox: Source code which combines project 7. and 8. to switch and play different music(credit to [robsoncouto](https://github.com/robsoncouto/arduino-songs)), while displaying self chosen colors for each note with a button.
    - requirement: passive buzzer, RGB LED and button
 10.  adc_ldr: Source code which enables the inbuild ADC to measure voltage of a LDR.
    - requirement: LDR
 11.  charger: Source code which enables the inbuild ADC to measure the charging and discharging of a capacitor.
    - requirement: capacitor
 12. blinky_assembler: Source code in assembly which toggles the inbuild LED.
 13. sosblinky: Source code which displays the abbreviation "SOS" in morsecode through the inbuild LED.
 14. sosblinky2: source code which is an extension of project 13, which only plays the abbreviation "SOS" once after a button press.
    -requirement: button
 15. secret_bin: bin file which contains an unknown compiled source code.(Not relevant as it was just a disassembling college task)
 16. therealhelloworld: Source code which prints out a string "Hello World" in bare-metal.
 17. blinkyprime: Source code which toggles an external LED through a systick interrupt in bare-metal.
    -requirement: red LED
 18. stopwatch: Source code which simulates a stopwatch in bare-metal, transmitting the time, which is count using systick, through USART.
    - requirement: button
 19. stopwatch_display. Source code which extends project 18. to display the time on a 7-segment-display.
    - requirement: button and 7-segment-display([TM1637](https://www.az-delivery.de/products/4-digit-display))
# Documentation (only in german)
Refer to the documents folder for a brief documentation on how my projects were made. Do mind that the documentations were made in a hurry and are still in a WIP state. 
# Building the Code

The projects from 1. to 11. were coded in C in a linux enviroment with the use of the software STM32CubeMX to create the STM32 framework with a makefile. The projects from 12. to were made in bare-metal in linux as well, which also uses a makefile for compilation. To deploy the code, enter the designated project and enter "make -j 8". Afterwards copy the "name_of_project".bin file in the newly made build folder to your STM32 board.  
Example: 
-  "cd blinky"
-  "make"
-  "cp build/blinky.bin /media/mrmr/NODE_L432KC" where blinky is the desired project and mrmr is the name of your computer(linux).
