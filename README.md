# contact-tracing-lpc1768
A simple contact tracing application for the Mbed LPC1768 microcontroller.

* 12/11/2020 - Update:
    * Removed unutilized libraries and outdated files.
    * Added basic Bluetooth connectivity using designated master and slave files.
    * Added more variables, such as diagnosis status & ID number, to print and file write statements.
* 11/23/2020 - Initial Git Commit:
    * HC_SR04 Library utilized for the HC-SR04 ultrasonic sensor.
    * Program takes distance measurements and writes them to a file along with a timestamp.