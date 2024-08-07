/*
 * File:   main.c
 * Author: STUDENT NAMES HERE
 *
 * Created on November 2, 2023, 9:26 AM
 */



// PIC24F16KA101 Configuration Bit Settings

/*
 * I recommend adding the FWDTEN setting to OFF pragma, as this stops your microcontroller periodically resetting itself when nothing happens
 */
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
/*
 * Adding the FCKSM = CSECMD enables clock switching
 */
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
/*
 * This pragma enables using the stepping debugger/setting breakpoints on the microcontroller
 */
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include <xc.h>
#include "clkChange.h"
#include "uart.h"
#include "header.h"

#define BUF_SIZE 10
extern uint8_t RXFlag;

char receive_buffer[BUF_SIZE];

int main(void) {
    
    newClk(500); //500 KHZ clock frequency
    
    InitUART2();
    
    AD1PCFG = 0xFFFF; // Make sure the UART RX bit is set to digital!
    uint16_t adcVal;
    
    while(1) {
        //replace while loop with if statements 
        //nest while(1) in each if condition
        int i = 0;
        Disp2String("Enter a single char, then hit ENTER: ");
        char choice = RecvUartChar();
        
        while(choice == 'x'){
            //hex code here
            adcVal = do_ADC();
            delay_ms(5000);
            DisplayBarGraph(adcVal);
            Disp2String("\r\n");
            Disp2Hex(adcVal);
            delay_ms(5000);   
          
        }
        while(choice == 'd'){
            //decimal code here
            adcVal = do_ADC();
            delay_ms(1000);
            DisplayBarGraph(adcVal);
            Disp2String("\r\n");
            Disp2Dec(adcVal);
            delay_ms(1000);
        }
        
        if(choice != 'x' || choice != 'd'){
            Disp2String("Please enter a valid digit");
            continue;
        }
        
        /* This ANSI escape sequence takes us home */
        XmitUART2(0x1b,1);
        XmitUART2('[',1);
        XmitUART2('H',1);
        /* This ANSI escape sequence clears the whole screen and takes us home*/
        XmitUART2(0x1b,1);
        XmitUART2('[',1);
        XmitUART2('2',1);
        XmitUART2('J',1);
       
        }
    /* An optional test you can run below to simply check the shared RXFlag*/
//        if (RXFlag == 1) {
//            RXFlag = 0;
//            Disp2String("received a char\n\r");
//            
//        }
//        Disp2String("test\n\r");
//        Disp2String("\r\n Now enter a sequence of up to 10 characters: ");
//        RecvUart(receive_buffer, BUF_SIZE);
//        Disp2String("\r\nYou said: ");
//        Disp2String(receive_buffer);
//        Disp2String("\r\n");
    return 0;
}

