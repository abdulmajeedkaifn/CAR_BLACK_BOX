#include "external_EEPROM.h"
#include "i2c.h"
#include <xc.h>


void write_EEPROM(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_E);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    
    //writing in EEPROM will take some time so give some blocking delay
    for(int i = 3000; i--;);
}

unsigned char read_EEPROM(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_E);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_E);
	data = i2c_read();
	i2c_stop();

	return data;
}