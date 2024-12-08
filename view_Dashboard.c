
#include <xc.h>
#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ds1307.h"
#include "i2c.h"
#include "external_EEPROM.h"

extern unsigned char key;
unsigned int speed, event_count = 1, max_event_count, hour = 0, min = 0, sec = 0;;

unsigned char reg_add = 0;

unsigned char clock_reg[3];
char time[9];
char Events[9][3] = {"C_", "ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR"};

//Dashboard function defination
void view_dashboard(void)
{
    //To get time from RTC
    get_time();
    
    clcd_print("  TIME    EV SP", LINE1(0));
    clcd_print(time, LINE2(0));
//    clcd_print(Events[event_count++], LINE2(8));
    
    //To get speed from ADC (Potentiometer)
    speed = read_adc(CHANNEL4);
    speed = speed / 10.33;
    
    
    clcd_putch(((speed/10) + '0' ), LINE2(13)); 
    clcd_putch(((speed%10) + '0'), LINE2(14));
    
    if(key == MK_SW1)
    {
        event_count = 0;
        
        event_store();
    }
    if(key == MK_SW2 && event_count < 8)
    {
        event_count++;
        
        event_store();
        
    }
    if(key == MK_SW3 && event_count > 1)
    {
        event_count--;
        
        event_store();    
    }
    
    clcd_print(Events[event_count], LINE2(10));
    
    //To get the current time whenever Main menu is called
    hour = ((time[0]- '0')*10) + (time[1]- '0');
    min = ((time[3]- '0')*10) + (time[4]- '0');
    sec = ((time[6]- '0')*10) + (time[7]- '0');
    
    //To switch to main menu
    if(key == MK_SW11)
    {
        state = e_main_menu;
//        CLEAR_DISP_SCREEN;
    }
}


//Function to store events in External EEPROM whenever any key is pressed in Dashboard 
void event_store(void)
{
    if(reg_add >= 120)
    {
        reg_add = 0;
    }
    
    for(int i = 0; i < 8; i++)
    {
        write_EEPROM(reg_add++, time[i]);
    }

    for(int i = 0; i < 2; i++)
    {
        write_EEPROM(reg_add++, Events[event_count][i]);
    }

    write_EEPROM(reg_add++, (speed/10) + '0');
    write_EEPROM(reg_add++, (speed%10) + '0');
    
    max_event_count++;
}

//Function to get time from RTC(DS1307)
void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
    
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}