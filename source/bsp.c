#include  "../header/bsp.h"    // private library - BSP layer
//#include  <msp430xG46x.h> // IDE library
#include  <msp430g2553.h>          // MSP430x2xx
////UPDATE14;55
/*______________________________________
 *                                      *
 *         stabalize                    *
 *______________________________________*/

//void stabalize() {
//    int j;
//    FLL_CTL0 |= XCAP14PF;                     // Configure load caps
//
//// Wait for xtal to stabilize
//    do {
//        IFG1 &= ~OFIFG;                           // Clear OSCFault flag
//        for ( j = 0x47; j > 0; j--);             // Time for flag to set
//    } while ((IFG1 & OFIFG));                   // OSCFault flag still set?
//}

/*______________________________________
 *                                      *
 *         GPIO configuration           *
 *______________________________________*/
void GPIOconfig(void){
    //P6DIR &= ~BIT6;             // input -> P6.6 -> AMPLIFIER -> DAC0

    TAPortSEL |= TA1;
    TAPortDIR |= TA1;

    TAPortSEL |= TA2;
    TAPortDIR &= ~TA2;    

    // LCD configuration
    LCD_DATA_WRITE &= ~0xF0;
    LCD_DATA_DIR |= 0xF0;    // P10.4-P10.7 To Output('1')
    LCD_DATA_SEL &= ~0xF0;   // Bit clear P10.4-P10.7
    LCD_CTL_SEL  &= ~0xE0;   // Bit clear P9.5-P9.7
    LCD_CTL_DIR  |= 0xE0;   // Bit clear P9.5-P9.7
    
    TriggerDIR |= Trigger;                            // P5.1 output
    TriggerSEL &= ~Trigger;
    Trigger_OUT &= ~Trigger;


    LDR1SEL |= 0x01;                            // Enable A/D channel A0
    LDR2SEL |= 0x08;                            // Enable A/D channel A0

    _BIS_SR(GIE);                     // enable interrupts globally

}

void TimerA1_Config(){
    WDTCTL = WDTPW +WDTHOLD;                  // Stop WDT
    //  TB0_CONFIG
    TA1CCR0 = MAX_TBR-2;                             // 60 ms Period/2

    //  TB1_CONFIG
    TA1CCTL1 |= OUTMOD_6 ;                       // TBCCR1 toggle/set
    TA1CCR1 = 1000;                              // TACCR1 PWM duty cycle
    
    //  TB2_CONFIG
    TA1CCTL2 |= CAP | CCIE | CCIS_0 | CM_3 | SCS;                       // TACCR2 toggle/set

 
    TA1CTL |= TASSEL_2 | MC_1 | CCIE;          // counts to CCR0

//    TA1CCTL2 = CAP + CM_3 + CCIE + SCS + CCIS_0;
//                                            // TA0CCR1 Capture mode; CCI1A; Both
//                                            // Rising and Falling Edge; interrupt enable
//  TA1CTL |= TASSEL_2 + MC_2 + TACLR;        // SMCLK, Cont Mode; start timer

  
    _BIS_SR(GIE);                     // enable interrupts globally
}

void TimerA0_Config(){
    WDTCTL = WDTPW +WDTHOLD;                  // Stop WDT

    //  TB0_CONFIG
    TA0CCR0 = MAX_TBR-2;                             // 60 ms Period/2

    //  TB1_CONFIG
    TA0CCTL0 = CCIE;                       // TBCCR1 toggle/set
    TACTL |= TASSEL_2 | MC_1 | CCIE;          // counts to CCR0

    _BIS_SR(GIE);                     // enable interrupts globally
}

void UART_Config() {
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  
//  if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
//  {											
//    while(1);                               // do not load, trap CPU!!	
//  }
//  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
//  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
//  DCOCTL = CALDCO_1MHZ;
  
  UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
  UCA0BR0 = 104;                           // 
  UCA0BR1 = 0x00;                           //
  UCA0MCTL = UCBRS0;               // 
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void delay2(){
    volatile int f;
    f=0;
    f++;
    f--;

}

void delay_us(unsigned int del){
    TACTL = TACLR;
    TACTL |= TASSEL_2 | MC_1 | ID_0 | CCIE;                  // SMCLK, up-down mode
    TACCR0 = TAR+del;
//    volatile int a=TACCR0, b=TAR;
//    a++;
//    b++;
    __bis_SR_register(LPM0_bits + GIE);
    TACTL &= ~CCIE;
  //  TBCCTL4 = OUTMOD_4 + CCIE;
}

void ADC_config0(){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

   // ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_0;                       // input A0
    
    ADC10AE0 |= 0x01;                         // PA.1 ADC option select
  //  __enable_interrupt();                     // Enable interrupts
  
//     WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
//  ADC10CTL1 = INCH_0;                       // input A1
//  ADC10AE0 |= 0x01;                         // PA.1 ADC option select

}
void ADC_config1(){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

   // ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_3;                       // input A0
    ADC10AE0 |= 0x08;                         // PA.1 ADC option select
  //  __enable_interrupt();                     // Enable interrupts
//    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
//  ADC10CTL1 = INCH_3;                       // input A1
//  ADC10AE0 |= 0x08;                         // PA.1 ADC option select

}
void ADC_start(){
  
    ADC10CTL0 =  ADC10SHT_3 | ADC10ON | ADC10IE;                             // Sampling and conversion start
    ADC10CTL0 |= ADC10SC | ENC;
}

void ADC_stop(){
   // ADC10CTL0 &= ~ENC;                             // Sampling and conversion start
   // ADC10CTL0 &= ~ADC10SC;
  ADC10CTL0=0x00;
    ADC10AE0=0x00;
}

