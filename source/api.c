#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "msp430xG46x.h"
#include "stdio.h"




void sysConfig(){
    GPIOconfig();
    TimerB_Config();
    lcd_init();



}

void set_angel(int phi){
    TACCR2=phi;
}

/*______________________________________
 *                                      *
 *          LCD API                     *
 *______________________________________*/

//
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

void LDR_measurement(int t){
        ADC_config();
        ADC_start();
        enable_interrupts();
        enterLPM(mode0);
        ADC_stop();
        long_delay();
        lcd_reset();
}

void trigger_ultrasonic(){
    long_delay();
    delay2();
    P5OUT ^= 0x02;
    delay2();
    P5OUT ^= 0x02;
    long_delay();
}

void print_measurments(int LLDR , int RLDR){
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
