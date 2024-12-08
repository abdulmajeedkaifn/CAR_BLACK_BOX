
#include <xc.h>
#include "clcd.h"

//byte is data here and control bit is Register Select RS bit
void clcd_write(unsigned char byte, unsigned char control_bit)
{
    //portc
	CLCD_RS = control_bit;
    //PORTD
	CLCD_PORT = byte;
    
    //Providing clock pulse to LCD bcoz it doesn't have its oscillation circuit
	/* Should be atleast 200ns */
	CLCD_EN = HI;
	CLCD_EN = LO;

    //PORT_DIR is TRISD7 = INPUT ( Busy Flag )
	PORT_DIR = INPUT;
    
    //RW = 1 read data and montior the Busy Flag, RW = 0 then write the data
	CLCD_RW = HI;
	CLCD_RS = INSTRUCTION_COMMAND;

	do
	{
		CLCD_EN = HI;
		CLCD_EN = LO;
	} while (CLCD_BUSY);

	CLCD_RW = LO;
	PORT_DIR = OUTPUT;
}

void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;
	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8;
    
    //If RW = 0 then we can make read and write operations
	CLCD_RW = LO;

	
     /* Startup Time for the CLCD controller */
    //
    __delay_ms(30);
    
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(1); 
    
    CURSOR_HOME;
    __delay_us(100);
    //T0 set 8 bit mode, how many lines we use and 
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);
    CLEAR_DISP_SCREEN;
    __delay_us(500);
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
    
    //writing one character at a time
	while (*data != '\0')
	{
		clcd_write(*data++, DATA_COMMAND);
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	clcd_write(data, DATA_COMMAND);
}
