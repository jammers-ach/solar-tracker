#define NO_BIT_DEFINES
#include <pic14regs.h>
#include <stdint.h>

// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN),
// disable watchdog,
// and disable low voltage programming.
// The rest of fuses are left as default.
__code uint16_t __at (_CONFIG) __configword = _INTRC_OSC_NOCLKOUT & _IESO_ON & _WDT_OFF & _MCLRE_OFF & _BOREN_OFF;//&& _LVP_OFF;

/*#define LED_PORT PORTCbits.RC2*/
/*#define LED_TRIS TRISCbits.TRISC2*/

#define LED_FULL_PORT PORTC
#define LED_FULL_TRIS TRISC

#define SW_PORT PORTAbits.RA3
#define SW_TRIS TRISAbits.TRISA3

#define RES_PORT PORTAbits.RA0
#define RES_TRIS TRISAbits.TRISA0


// Uncalibrated delay, just waits a number of for-loop iterations
void delay(uint16_t iterations){
    uint16_t i;
    for (i = 0; i < iterations; i++) {
        // Prevent this loop from being optimized away.
        __asm nop __endasm;
    }
}

void update_lights(char value){
    LED_FULL_PORT = value;
}


void main(void){
    int value;

    LED_FULL_TRIS = 0;
    LED_FULL_PORT = 0x00;

    SW_PORT = 0;
    SW_TRIS = 1;

    RES_PORT = 0;
    RES_TRIS = 1;

    ANSEL = 0b00000001;
    ADCON0 = 0b10000000;   // select right justify result. ADC port channel 0
    ADCON1 = 0b00110000;   // Select the FRC for 8 Mhz
    ADCON0bits.ADON =1;       // turn on the A2D conversion module


    //Flash to show booted
    LED_FULL_PORT = 0xff;
    delay(8000);
    LED_FULL_PORT = 0x00;

    while(1) {

        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
        value=((unsigned)ADRESH<<4)+ADRESL;
        update_lights(ADRESH);
    }
}
