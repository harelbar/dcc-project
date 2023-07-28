#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
//#include "msp430xG46x.h"
#include  <msp430g2553.h>          // MSP430x2xx
#include "stdio.h"
int fdfd=0;



void sysConfig(){
    GPIOconfig();
    TimerA0_Config();
    TimerA1_Config();
    lcd_init();
    UART_Config();

}

void set_angel(int phi){
    TA1CCR1=phi;
    start_PWM();
}

void lcd_reset(){
    lcd_clear();
    lcd_home();
}

void long_delay(){
    delay_us(del60ms);
    delay_us(del60ms);
    delay_us(del60ms);
    delay_us(del60ms);
}

void LDR_measurement(unsigned volatile int arr[]){
        fdfd++;
        ADC_config0();
        ADC_start();
         fdfd++;
         
       // enable_interrupts();
         fdfd++;
         
        __bis_SR_register(LPM0_bits + GIE);
      //  ADC_stop();
        arr[0]=ADC10MEM;
        fdfd++;
        
        ADC_config1();
        ADC_start();
      //  enable_interrupts();
        __bis_SR_register(LPM0_bits + GIE);
      //  ADC_stop();
        arr[1]=ADC10MEM;
        fdfd++;
        
        long_delay();
        lcd_reset();
        fdfd++;
}

void trigger_ultrasonic(){
    delay_us(del60ms);
   // delay2();
    delay_us(20);
    Trigger_OUT |= 0x08;
    //delay2();
    delay_us(20);
    Trigger_OUT &= ~0x08;
    long_delay();

}

void print_measurments(unsigned int LLDR , unsigned int RLDR){
    char L[16],R[16];
    sprintf(L, "%d", LLDR);
    sprintf(R, "%d", RLDR);
    cursor_off;
    lcd_reset();
    lcd_putrow("RLDR: ");
    lcd_putrow(L);
    if(LLDR>RLDR) lcd_putrow(" LS");
    else lcd_putrow(" RS");
    lcd_new_line;
    lcd_putrow("LLDR: ");
    lcd_putrow(R);
    long_delay();
}

void send_msg(){
    int ss=12345;
    sprintf(message, "%d s", ss);
    start_msg();

}
