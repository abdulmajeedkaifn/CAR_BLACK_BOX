#include <xc.h>
#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "uart.h"
#include "external_EEPROM.h"
#include "ds1307.h"

#define MAX_EVENT_COUNT     10

extern State_t state;

extern unsigned char key;
extern unsigned int max_event_count, hour = 0, min = 0, sec = 0;
extern char time[9];
unsigned int index = 0, log_index = 0, delay = 0, delay1 = 0, delay2 = 0, max_event_count1 = 0;
unsigned char reg_address = 0;

char menu[4][14] = {"VIEW LOG      ", "DOWNLOAD LOG  ", "CLEAR LOG     ", "SET TIME      "};
char Log[10][15];

//Function to display main menu
void display_main_menu(void)
{
    if(key == MK_SW11 && index < 3)
    {
        index++;
    }
    if(key == MK_SW12 && index > 0)
    {
        index--;
    }

    if( !index )
    {
        clcd_print("->", LINE1(0));
        clcd_print(menu[index], LINE1(2));

        clcd_print("  ", LINE2(0));
        clcd_print(menu[index+1], LINE2(2));
    }
    else
    {
        clcd_print("  ", LINE1(0));
        clcd_print(menu[index-1], LINE1(2));
        clcd_print("->", LINE2(0));
        clcd_print(menu[index], LINE2(2));
    }
    
    //To switch to view log in main menu
    if(key == MK_SW1 && index == 0)
    {
        state = e_view_log;
    }
    //To switch to Download log in main menu
    if(key == MK_SW1 && index == 1)
    {
        state = e_download_log;
    }
    //To switch to Clear log in main menu
    if(key == MK_SW1 && index == 2)
    {
        state = e_clear_log;
    }
    //To switch to Set Time in main menu
    if(key == MK_SW1 && index == 3)
    {
        state = e_set_time;
    }
    
    //To go back to Dashboard
    if(key == MK_SW2)
    {
        state = e_dashboard;
        CLEAR_DISP_SCREEN;
    }
}

//Function to display Events
void view_log(void)
{
    if( !max_event_count )
    {
        clcd_print("NO EVENTS FOUND!", LINE1(0));
        clcd_print("                ", LINE2(0));
    }
    else
    {
        clcd_print("L   TIME   E  S", LINE1(0));
        
        if( log_index < MAX_EVENT_COUNT)
        {
            clcd_putch((log_index) + '0', LINE2(0));
            clcd_print(Log[log_index], LINE2(2));
        }

        event_reader();

        if(key == MK_SW11 && log_index < max_event_count-1)
        {
            log_index++;
        }
        if(key == MK_SW12 && log_index > 0)
        {
            log_index--;
        }
    }
    
    //To go back to Main menu
    if(key == MK_SW2)
    {
        state = e_main_menu;
    }
}

//Function to read stored events from External EEPROM
void event_reader(void)
{
    reg_address = 0;
    
    for(int i = 0; i < max_event_count; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            if(j == 8 || j == 11)
            {
                Log[i][j] = ' ';
            }
            else if(j == 14)
            {
                Log[i][j] = '\0';
            }
            else
            {
                Log[i][j] = read_EEPROM(reg_address++);
            }
        }
    }
}

//To View Events into TeraTerm using UART Protocol
void download_log(void)
{
    if(max_event_count < MAX_EVENT_COUNT)
    {
        max_event_count1 = max_event_count;
    }
    else
    {
        max_event_count1 = MAX_EVENT_COUNT;
    }
    
    if( !max_event_count )
    {
        while(delay++ == 100)
        {
            delay = 0;
            puts("\n\r");
            puts("####  NO EVENTS STORED    ####\n\r");
            state = e_main_menu;
            CLEAR_DISP_SCREEN;
        }
    }
    else
    {
        event_reader();
        clcd_print("    DOWNLOAD   ", LINE1(0));
        clcd_print("   SUCCESSFUL  ", LINE2(0));
        
        if(delay++ == 100)
        {
            delay = 0;
            state = e_main_menu;
            CLEAR_DISP_SCREEN;
            puts("LOG   TIME   EV SP\n\r");
            for(int i = 0; i < max_event_count1; i++)
            {
                putch(' ');
                putch((i) + '0');
                puts("  ");
                puts(Log[i]);
                puts("\n\r");
            }        
        }
    }
}

//Function to Clear Events that are stored
void clear_log(void)
{
    if( !max_event_count )
    {
        while(delay++ == 100)
        {
            delay = 0;
            
            state = e_main_menu;
            CLEAR_DISP_SCREEN;
        }
    }
    else
    {
        clcd_print("     CLEAR     ", LINE1(0));
        clcd_print("   SUCCESSFUL  ", LINE2(0));

        if(delay2++ == 2000)
        {
            delay2 = 0;
            state = e_main_menu;
            CLEAR_DISP_SCREEN;

            //Clear operation
            max_event_count = 0;          
        }
    }
}

int flag1 = 1;

// Function to print time to clcd in SET TIME Funcion
void print_clcd(void)
{
    clcd_putch(hour/10 + '0', LINE2(4));
    clcd_putch(hour%10 + '0', LINE2(5));
    
    clcd_putch(':', LINE2(6));
    
    clcd_putch(min/10 + '0', LINE2(7));
    clcd_putch(min%10 + '0', LINE2(8));
    
    clcd_putch( ':', LINE2(9));
    
    clcd_putch(sec/10 + '0', LINE2(10));
    clcd_putch(sec%10 + '0', LINE2(11));
}

//Function to set time 
void set_time(void)
{
    clcd_print("    HH:MM:SS    ", LINE1(0));
    clcd_print("    ", LINE2(0));
//    clcd_print(time, LINE2(4));
    clcd_print("    ", LINE2(12));
    
    if (key == MK_SW11)
    {
        if (flag1 == 1) 
        {  // Increment hours
            if (++hour == 24) 
            {
                hour = 0;
            }
        } 
        else if (flag1 == 2) 
        {  // Increment minutes
            if (++min == 60) 
            {
                min = 0;
            }
        } 
        else if (flag1 == 3) 
        {  // Increment seconds
            if (++sec == 60) 
            {
                sec = 0;
            }
        }
    }
    
    if(flag1 == 1)
    { 
        if(delay1++ <= 500)
        {
            print_clcd();
        }
        else if(delay1++ <= 1000)
        {
//           clcd_print("  ",LINE2(4));
           clcd_putch(0xFF, LINE2(4));
           clcd_putch(0xFF, LINE2(5));
        }
        else 
        {
            delay1 = 0;
        }
    }
    
    if(flag1 == 2)
    { 
        if(delay1++ <= 500)
        {
            print_clcd();
        }
        else if(delay1++ <= 1000)
        {
//           clcd_print("  ",LINE2(7));
            clcd_putch(0xFF, LINE2(7));
            clcd_putch(0xFF, LINE2(8));
        }
        else 
        {
            delay1 = 0;
        }
    }
    
    if(flag1 == 3)
    { 
        if(delay1++ <= 500)
        {
            print_clcd();
        }
        else if(delay1++ <= 1000)
        {
//           clcd_print("  ",LINE2(10));
           clcd_putch(0xFF, LINE2(10));
           clcd_putch(0xFF, LINE2(11));
        }
        else 
        {
            delay1 = 0;
        }
    }
    
    if( key == MK_SW12 )
    {
        if(++flag1 == 4)
        {
            flag1 = 1;
        }
    }   
    
    //To Save and Exit the desired time
    if(key == MK_SW1)
    {
        write_ds1307(HOUR_ADDR, (((hour/10) << 4) | (hour % 10)));
        write_ds1307(MIN_ADDR, (((min/10) << 4) | (min % 10)));
        write_ds1307(SEC_ADDR, (((sec/10) << 4) | (sec % 10)));
        
        state = e_main_menu;
    }
    
    //To exit without saving the set time
    if(key == MK_SW2)
    {
        state = e_main_menu;
    }
}