/*
 * Project:   CAR_BLACK_BOX 
 * Author: Abdul Majeed Kaif N
 *
 * Created on 14 February, 2024, 5:40 PM
 */

#include <xc.h>
#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "uart.h"
#include "ds1307.h"
#include "i2c.h"
#include "external_EEPROM.h"

State_t state;

unsigned char key;

void init_config()
{
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_uart();
    init_i2c();
	init_ds1307();
    
    state = e_dashboard;
}

void main(void) 
{
    init_config();

    while(1)
    {
        // Detect key press
        
        key = read_switches(STATE_CHANGE);
        
        switch (state)
        {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;
            
            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;
            
            case e_view_log:
                // Display dashboard
                view_log();
                break;
                 
            case e_download_log:
                download_log();
                break;
                
            case e_clear_log:
                clear_log();
                break;
                
            case e_set_time:
                set_time();
                break;
        }
    }
}
