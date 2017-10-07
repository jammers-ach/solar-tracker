#define NO_BIT_DEFINES
#include <pic14regs.h>
#include <stdint.h>

// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN),
// disable watchdog,
// and disable low voltage programming.
// The rest of fuses are left as default.
__code uint16_t __at (_CONFIG) __configword = _INTRC_OSC_NOCLKOUT & _IESO_ON & _WDT_OFF & _MCLRE_OFF & _BOREN_OFF;//&& _LVP_OFF;

#define LDR1_PORT PORTAbits.RA0
#define LDR1_TRIS TRISAbits.TRISA0

#define LDR2_PORT PORTAbits.RA1
#define LDR2_TRIS TRISAbits.TRISA1

#define LDR3_PORT PORTAbits.RA2
#define LDR3_TRIS TRISAbits.TRISA2

#define LDR4_PORT PORTAbits.RA3
#define LDR4_TRIS TRISAbits.TRISA3

// Uncalibrated delay, just waits a number of for-loop iterations
void delay(uint16_t iterations){
    uint16_t i;
    for (i = 0; i < iterations; i++) {
        // Prevent this loop from being optimized away.
        __asm nop __endasm;
    }
}


/**
 * Read all 4 LDRs into values
 */
void read_ldrs(uint16_t *values) {

    ADCON0 = 0b10000000;   // select right justify result. ADC port channel 0
    ADCON1 = 0b00110000;   // Select the FRC for 8 Mhz
    ADCON0bits.ADON =1;       // turn on the A2D conversion module

    ANSEL = 0b00000001;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    values[0]=((unsigned)ADRESH<<4)+ADRESL;
    delay(100);

    ANSEL = 0b00000010;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    values[1]=((unsigned)ADRESH<<4)+ADRESL;
    delay(100);

    ANSEL = 0b00000100;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    values[2]=((unsigned)ADRESH<<4)+ADRESL;
    delay(100);

    ANSEL = 0b00001000;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    values[3]=((unsigned)ADRESH<<4)+ADRESL;
}


void main(void){
    uint16_t ldrs[4];

    // Initialize the analog ports for LDRS as inputs
    LDR1_PORT = 0;
    LDR1_TRIS = 1;
    LDR2_PORT = 0;
    LDR2_TRIS = 1;
    LDR3_PORT = 0;
    LDR3_TRIS = 1;
    LDR4_PORT = 0;
    LDR4_TRIS = 1;

    while(1) {
        read_ldrs(ldrs);
    }
}
