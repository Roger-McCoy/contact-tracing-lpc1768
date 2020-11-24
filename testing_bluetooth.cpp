// THREE BLUETOOTH CODE EXAMPLES:
#include "mbed.h"

Serial rn41(p9,p10);                    // Names the serial port rn41 // Was previously (p13, p14)
                                        // TX wire on BT goes on Mbed RX(p10)!
                                        // RX wire on BT goes on Mbed TX(p9)!

BusOut led(LED4,LED3,LED2,LED1);        // Used in first two examples. Should it also be DigitalOut? Apparently not.

// DigitalOut myled(LED1);              // Used in last "SIMPLE" example.





/* Program Example 11.1: Bluetooth serial test data program
Data is transferred from mbed to PC via Bluetooth. */
/*
int main() 
{
    rn41.baud(115200);                  // Sets the baud rate for RN41
    while (1) 
    {
        for (char x=0x30;x<=0x39;x++)   // ASCII numerical characters 0-9
        {
            rn41.putc(x);               // Sends test char data on serial to RN41
            led = x & 0x0F;             // Sets LEDs to count in binary
            wait(0.5);
        }
    }
} */

/* Program Example 11.2: Bluetooth serial test data program
Data is transferred bidirectionally between mbed and PC via Bluetooth.
---
This program sends a character string from mbed to PC, appearing on Tera
Term if enabled. It then receives any data from the host PC keyboard;
The remote mbed displays the lower four bits of the received byte on the onboard LEDs. */

int main() 
{
    rn41.baud(115200);                  // Sets the baud rate for RN41
    rn41.printf("Serial data test: outputs received data to LEDs\n\r");
    while (1) 
    {
        if (rn41.readable())            // If data is available
        {
            char x=rn41.getc();         // Get data
            led=x;                      // Output LSByte to LEDs
        }
    }
} 

// MUCH MORE SIMPLE EXAMPLE:
/*
int main() 
{
 
    rn42.baud(115200);
 
                                        // Echo back characters, toggle the LED
    while (1) 
    {
        if (rn42.readable()) {
            rn42.putc(rn42.getc());
            myled = !myled;
        }
    }
}
*/