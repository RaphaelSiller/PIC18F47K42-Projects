// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator Selection (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC  // Reset Oscillator Selection (EXTOSC operating per FEXTOSC bits (device manufacturing default))

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTC = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// CONFIG5H

#define _XTAL_FREQ 20000000L

#include <xc.h>

void InitPorts(void)
{
    /*
    TRISC = 0x00;       //PORTC als Ausgang (LEDs)
    ANSELC = 0x00;
    LATC = 0x00;
    */
    TRISD = 0x00;       //PORTD als Ausgang ()
    ANSELD = 0x00;
    LATD = 0x00; 
    
    TRISB = 0xff;
    WPUB = 0x07;        // Pull Up-Widerstand @ RB0, RB1 und RB2
    ANSELB = 0x00;
    LATB = 0x00;
}

void InitTimer0(void)
{
    T0CON0bits.MD16 = 1;    // 16 Bit 
    T0CON0bits.OUTPS = 0b0000;      // Postscaler 1:1, Bits OUTPS3 bis OUTPS0

    T0CON1bits.CS = 0b010;      // Taktquelle ist Instruction Clock, Bits CS2 bis CS0
    T0CON1bits.ASYNC = 1;       // NOT synchronized with instruction clock
    T0CON1bits.CKPS = 0b0110;       // Prescaler, 1:64, Bits CKPS3 bis CKPS0   Werte sind im Datenblatt auf Seite 303

    PIR3bits.TMR0IF = 0;            // Must be cleared by software

    T0CON0bits.EN = 1;      // Timer0 ein

}

void main(void)
{
    InitPorts();
    InitTimer0();
    
    

    while(1) {
        if(PIR3bits.TMR0IF) {         // Hier Polling = abfragen
            //Setze Wert auf 8AD0 = 35536
            TMR0H = 0x8A;
            TMR0L = 0xD0;
            
             //LATC = ~LATC;
             
             LATDbits.LATD0 = ~LATDbits.LATD0;
 	     PIR3bits.TMR0IF = 0; //Timer zuruecksetzen
        }  
    }
}


/*
 * ?t = (65536 - startwert)*PS*02�s
 * PreScaler = 1:64
 * Startwert = 35536
 * ?t = (65536 - 35536)*(64)*02�s = 65535,54�s = 65.535ms
 * 
 * 
 * 
 * ?t = (65536 - startwert)*PS*02�s = 0.500
 * 
 * (65536 - startwert) = 0.500 / (PS*02�s)
 * startwert = 65536 - (0.500 / (PS*02�s))
 * 26473.5 = 65536 - (0.500 / (64*02�s)) ~ 26473 = 0x6769
 */