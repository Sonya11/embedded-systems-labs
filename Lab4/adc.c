/**
 * Author: Sonya Schuppan, Garret Lemmon, Ryan Swanson
 *
 * File: lab4.c
 *
 * Description: configures ADC and defines ADC interrupt handler
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */
#include "msp.h"
#include "adc_circbuf.h"

#define TEMP_SLOPE 0.074932
#define TEMP_OFFSET 344

extern CircBuf * buffer;

void ADC_temp_config(void){
    //configure port 8.3 to eliminate stray voltage for the ADC
    P8->SELC |= 0x08;

    // Configure the Reference Module
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY); // Wait if reference generator is busy
    REF_A->CTL0 = 0x0031;   // Set up a 2.5 V reference voltage, Turn on Temperature Sensor

    ADC14->CTL0 = 0; //default and turn off ADC
    ADC14->CTL0 |= 0xC56A6090; //clock divided to take sample every 1.032s #doubleyay, HSMCLK as source
    ADC14->CTL1 |= 0x00800030; 
    ADC14->MCTL[0] |= 0x0116;

    //ADC14->CTL0 |= 0x03; //start the adc

    ADC14->IER0 |= 0x00000001; //Enable the MCTL0 interrupts!
    while(REF_A->CTL0 & REF_A_CTL0_GENRDY); // Wait for reference generator to settle
    ADC14->CTL0 |= 0x02; // ADC14 enable conversion
    NVIC_EnableIRQ(ADC14_IRQn); // enable ADC14 interrupts
}

void ADC14_IRQHandler(void){
    if(ADC14->IFGR0 & 0x01){
        //(flag cleared when data is read)
        //uint16_t dataPoint = 0;
        //uint16_t N_adc = ADC14->MEM[0];
        uint16_t dataPoint = TEMP_SLOPE*ADC14->MEM[0]-TEMP_OFFSET; //converting Nadc vlaue to temp value
        addItemToBuffer(buffer, dataPoint);
        P1->OUT ^= (1 << 7); //toggle pin
    }
}
