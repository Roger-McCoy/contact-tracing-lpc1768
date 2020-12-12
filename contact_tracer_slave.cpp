/* --- Main Ultrasonic Sensor and Bluetooth Slave Code for Contact Tracing App.
 * --- Roger McCoy, Kubra Gundogan, Nuri Alperen Kose, Yalçin Taf.
 * --- Upon initialization by user in a terminal, the device takes measurements 
 * --- (in mm) of the user's distance from a passerby and saves them to a txt 
 * --- file. Information about the user's ID#, disease status(t,f), and a 
 * --- timestamp documenting the event are included. This data is also printed
 * --- to the terminal. If possible, the device will then attempt to connect to 
 * --- another tracing device over Bluetooth to share the contact data.
 */
#include "mbed.h"
#include "ultrasonic.h"
#include<string>
 
Serial pc(USBTX, USBRX);

Serial rn41(p9,p10);                    // TX wire on BT goes on Mbed RX(p10)!
                                        // RX wire on BT goes on Mbed TX(p9)!

LocalFileSystem local("local");         // Create the local filesystem under the name "local"
                                        // Requires latest version of LPC1768 firmware. Link below.
                                        // https://os.mbed.com/handbook/Firmware-LPC1768-LPC11U24

int id = 904186;                        // Arbitrary ID number.

bool diseaseStatus = false;             // Set whether or not the user has the contagious disease with this variable.

void dist(int distance)
{
    // Code here runs when the distance is changed.
    
    time_t seconds = time(NULL);
    
    string diseaseDiagnosis;
    if (diseaseStatus == true)
    {
        diseaseDiagnosis = "Pos. for disease.";
    } else if (diseaseStatus == false)
    {
        diseaseDiagnosis = "Neg. for disease.";
    }
    
    pc.printf("%s ID:%i %s Distance changed to %dmm\r\n", ctime(&seconds), id, diseaseDiagnosis, distance); // Prints the timestamp, the ID number, the disease diagnosis, and the distance measurements.
    
    FILE *fp = fopen("/local/distancereadings.txt", "a"); // Create or open "distancereadings.txt" for appending (writing at end-of-file).
    fprintf(fp, "%s ID:%i %s Distance changed to %dmm\r\n", ctime(&seconds), id, diseaseDiagnosis, distance);
    fclose(fp);
    
    if (rn41.writable())
    {
        // Code to send to the master device:
        
        /* Not fully functioning:
        char charArrayToSend[100];
        snprintf(charArrayToSend, sizeof (charArrayToSend), "%s ID:%i %s Distance changed to %dmm\r\n", ctime(&seconds), id, diseaseDiagnosis, distance);
        pc.printf(charArrayToSend);
        for (int i = 0; i<sizeof(charArrayToSend); i++)
        {
            rn41.putc(charArrayToSend[i]); 
            pc.printf(" %c", charArrayToSend[i]);
        }*/
    }
    
    if (rn41.readable()) // If data is available.
    {   
        // Code to read from master device:
        
        /* Not fully functioning:
        char charArrayToReceive[100];
        for (int i = 0; i<sizeof(charArrayToReceive); i++)
        {
            rn41.getc(charArrayToReceive[i]); 
            pc.printf(" %c", charArrayToReceive[i]);
        }*/
    }
}

ultrasonic mu(p12, p13, 1, 2, &dist);   // Set the trigger pin to p17 and the echo pin to p18.
                                        // Have updates every 1 seconds and a timeout after 2
                                        // seconds, and call dist when the distance changes.
                                        // Was originally every .1 and 1 second, but resulted in too much spam.
                                        // Wire MBED's VU(5V)pin to Sensor VCC pin, and GND to GND.

int main()
{
    rn41.baud(115200);                  // Sets the baud rate for RN41
    
    wait(1.0); 
    rn41.putc('$');                     // Enter command mode.
    rn41.putc('$');
    rn41.putc('$');
    wait(1.2);
    rn41.putc('S');                     // Set Authentication to 0.
    rn41.putc('A');
    rn41.putc(',');
    rn41.putc('0');
    rn41.putc(0x0D);
    rn41.putc('-');                     // Exit command mode.
    rn41.putc('-');
    rn41.putc('-');
    rn41.putc(0x0D);
    wait(0.5); 
    
    set_time(1607463000-21600);         // Central Time is 6 hours (21,600 seconds) behind UTC.
                                        // Get current Unix Time here: https://time.is/Unix_time_now
                                        
    pc.printf("\nUltrasonic sensor program has started.\r\n");
    mu.startUpdates();                  // Start measuring the distance.
    
    
    for(int i=1; i>=1; i++)             // Infinite for loop.
    {
        if (i==1)                       // Only prints this on the first iteration of loop.
        {
            pc.printf("Type 'S' to start logging distance measurements. Type any other letter to halt the program.\r\n");
        }
        
        char inputOne = pc.getc();
        if (inputOne == 's') 
        {
            for(int i=1; i>=1; i++)
            {
                if (i==1)
                {
                    pc.printf("Starting measurements. . .\n");
                }
            
                // Possibly do something else here later.
                
                mu.checkDistance();     // Call checkDistance() as much as possible, as this is where
                                        // the class checks if dist needs to be called.
            
                if(pc.readable())       // Allows you to check for readable input without halting program to wait for it.
                {         
                    char inputTwo = pc.getc();
                    if (inputTwo != 's')
                    {
                        break;
                    }
                }
            }
        }
    }
}
 
         