// main.c
#include <lpc21xx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "delay.h"
#include "lcd.h"
#include "uart.h"
#include "i2c_main.h"
#include "keypad.h"
#include "otp.h"

void print(void)
{
	int i;
	lcd_command(0x83);
	lcd_string("MULTILEVEL");
	lcd_command(0xc0);
	lcd_string("SECURITY SYSTEM");
	uart_string("MULTILEVEL SECURITY SYSTEM\r\n");
	delay_sec(5);
	lcd_command(0x10);
	lcd_command(0x80);
	lcd_string("PRESS THE SWITCH1");
	lcd_command(0xc0);
	lcd_string("TO CONTINUE\r\n");
	uart_string("PRESS THE SWITCH1 TO CONTINUE\r\n");
	for(i=1000;i<=9999;i++)
	{
		if(i==9999)
			i=999;
		if((IOPIN0 & 1<<14)==0)
			break;
	}
	sprintf(s,"%d",i);
	//uart_string(s);
}
int main()
{
	int i,j;
	IODIR0 |=3<<8;
	uart_config();			//config the uart
	lcd_config();				//config the lcd
	i2c_config();				//config the i2c
	print();
	i2c_main();					//writing password in i2c
	delay_ms(5);
	i=0;

	password_generator:
		lcd_command(0X01);
		lcd_command(0x80);
		lcd_string("ENTER PASSWORD");
		uart_string("ENTER PASSWORD\r\n");
		keypad_main();
		delay_ms(5);
		j=strcmp(pass,eeprom_pass);
		i++;
	if((i<4)&&(j!=0))
	{
		lcd_command(0X01);
		lcd_command(0x80);
		lcd_string("INCORRECT PASS");
		lcd_command(0xC0);
		lcd_string("TRY AGAIN");
		uart_string("INCORRECT PASSWORD\r\n");
		uart_string("TRY AGAIN\r\n");
		if(i==3)
		{
			uart_string("more than 3 times given sorry\r\n");
			uart_string("acess denied\r\n");
			lcd_command(0X01);
			lcd_string("ACESS DENIED");
			return 0;
		}
		goto password_generator;
	}
		lcd_command(0X01);
		lcd_string("CORRECT PASS");
		uart_string("\r\nCORRECT PASSWORD\r\n");
	
	i=0;
	otp_main();
	delay_sec(2);
	otp_generator:
		lcd_command(0X01);
		lcd_command(0x80);
		lcd_string("ENTER OTP");
		uart_string("ENTER OTP\r\n");
		keypad_main();
		delay_ms(5);
		j=strcmp(pass,s);
		i++;
	if((i<4)&&(j!=0))
	{
		uart_string("OTP INCORRECT\r\n");
		uart_string("TRY AGAINr\n");
		if(i==3)
		{
			uart_string("more than 3 times given sorry\r\n");
			uart_string("acess denied\r\n");
			lcd_command(0X01);
			lcd_string("ACESS DENIED");
			return 0;
		}
		goto otp_generator;
	}
	lcd_command(0x01);
	lcd_command(0x80);
	uart_string("CONGRATS\r\n");
	lcd_string("CONGRATS");
	
	while(1)
	{
	lcd_command(0x01);
	lcd_command(0x80);
	uart_string("CLOCK WISE\r\n");
	lcd_string("CLOCK WISE");
	
	IOSET0 = 1<<8;
	IOCLR0 = 1<<9;
	delay_sec(5);
	IOSET0 = 1<<8|1<<9;
	delay_sec(1);
	
	lcd_command(0x01);
	lcd_command(0x83);
	uart_string("ANTI CLOCK WISE\r\n");
	lcd_string("ANTI");
	lcd_command(0xC0);
	lcd_string("CLOCK WISE");
		
	IOCLR0 = 1<<8;
	IOSET0 = 1<<9;
	delay_sec(5);
	IOSET0 = 1<<8|1<<9;
	delay_sec(1);
	}
	
	
}
