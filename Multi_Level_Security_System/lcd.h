//lcd.h
#define lcd 0XF<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

void lcd_command(unsigned char cmd)
{
	IOCLR1 =lcd;
	IOSET1 = (cmd&0xF0)<<16;
	IOCLR1 = rs|rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
	
	IOCLR1 =lcd;
	IOSET1 = (cmd&0x0F)<<20;
	IOCLR1 = rs|rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
}
void lcd_data(unsigned char d)
{
	IOCLR1 =lcd|rw;					 
	IOSET1 = (d&0xF0)<<16;
	IOSET1 = rs;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
	
	IOCLR1 =lcd|rw;
	IOSET1 =(d&0x0F)<<20;
	IOSET1 = rs;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
}
void lcd_string(unsigned char *s)
{
		while(*s)
		{
			lcd_data(*s++);		
		}
}

void lcd_int(unsigned int n)
{
	unsigned char arr[10],ch;
	int i =0;
	if(n == 0)
		lcd_data('0');
	else
	{
		if(n<0)
		{
			lcd_data('-');
			n=-n;
		}
		while(n>0)
		{
			arr[i++] = n%10;
			n = n/10;
		}
		for(--i;i>=0;i--)
		{
			 ch=(arr[i]+48);
			lcd_data(ch);
		}
	}
}


void lcd_config(void)
{
	IODIR1 |=lcd|rs|e|rw;
	lcd_command(0X01);
	lcd_command(0X02);
	lcd_command(0X0C);
	lcd_command(0X28);
	lcd_command(0X80);
}
