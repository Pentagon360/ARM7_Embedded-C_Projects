#define scl 0x10		//p0.2
#define sda 0x40		//p0.3

#define cclk 60000000
#define pclk cclk/4		//set a peripheral clock 15000000
#define i2c_speed 100000  //i2c standard speed 100kbps
#define loadval ((pclk/i2c_speed)/2)

#define	aa_bit	2 			 			//assert aknowldge bit
#define	si_bit	3  						//si flag bit
#define	stop_bit	4 		 			//stop bit
#define	start_bit	5		  			//start bit
#define	i2c_enable_bit	6 	 //enable bit

#define i2c_eeprom	0x50
#define led1	1<<17					//p0.17
#define led2	1<<19					//p0.19

unsigned char eeprom_pass[40];
typedef unsigned char u8;

void i2c_config(void)
{
	PINSEL0 |= 	scl|sda	;	//configure the i/o pin function for scl and sda
	I2SCLL  =   loadval;	//loadvalue of low clock speed=75
	I2SCLH  =   loadval;	//loadvalue of high clock speed=75
	I2CONSET = 1<<i2c_enable_bit;
}

void i2c_start(void)
{
	I2CONSET = 1<<start_bit;  					//start condition
	while(((I2CONSET>>si_bit)&1)==0);	//wait for si bit status
	I2CONCLR = 1<<start_bit;								//clear the start bit 
}

void i2c_write(u8 data)
{
	I2DAT = data;													//write the data into I2DAT register
	I2CONCLR = 1<<si_bit;										//clear the si bit flag
	while(((I2CONSET>>si_bit)&1)==0);	//wait for the si bit status
}

void i2c_stop(void)
{
	I2CONSET = 1<<stop_bit;							//set the stop bit for stop condition
	I2CONCLR = 1<<si_bit;								//clear the si bit flag
}
void i2c_restart(void)
{
	I2CONSET = 1<<start_bit;  					//start condition
	I2CONCLR = 1<<si_bit;								//clear the si bit
	while(((I2CONSET>>si_bit)&1)==0);	//wait for si bit status
	I2CONCLR = 1<<start_bit;								//clear the start bit
}

u8 i2c_nack(void)
{
	I2CONSET = 0X00;				//assert no aknowlwdegement
	I2CONCLR = 1<<si_bit;
	while(((I2CONSET>>si_bit)&1)==0);	//wait for si bit status
	I2CONCLR = 0X04;
	return I2DAT;
}

u8 i2c_masterack(void)
{
	I2CONSET = 0X04;				//assert no aknowlwdegement
	I2CONCLR = 1<<si_bit;
	while(((I2CONSET>>si_bit)&1)==0);	//wait for si bit status
	return I2DAT;
}
void i2c_eeprom_page_write(u8 slave_addr,u8 word_addr,unsigned char* p)
{
	unsigned char i=0;
	i2c_start();
	i2c_write(slave_addr<<1);
	i2c_write(word_addr);
	for(i=0;i<4;i++)
	{
			i2c_write(p[i]);
	}
	i2c_stop();
	delay_ms(10);
}

void i2c_eeprom_seq_read(u8 slave_addr,u8 word_addr)
{
	u8 i;
	i2c_start();
	i2c_write(slave_addr<<1);
	i2c_write(word_addr);
	i2c_restart();
	i2c_write(slave_addr<<1|1);
	for(i=0;i<3;i++)
	{
			eeprom_pass[i]=i2c_masterack();
	}
	eeprom_pass[i] = i2c_nack();
	eeprom_pass[i+1]='\0';
	i2c_stop();
	delay_ms(10);
}
void i2c_main(void)
{
	i2c_eeprom_page_write(i2c_eeprom,0x00,"0123");
	i2c_eeprom_seq_read(i2c_eeprom,0x00);
   //uart_string(eeprom_pass);
	//lcd_string(eeprom_pass);
	
}
