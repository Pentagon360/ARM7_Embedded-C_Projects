//otp.h
unsigned char s[10];

/*void otp(void)
{
	unsigned int i;

	for(i=1000;i<=9999;i++)
	{
		if(i==9999)
			i=999;
		if((IOPIN0 & 1<<14)==0)
			break;
	}
	sprintf(s,"%d",i);
	
	lcd_string(s);
	uart_string(s);
}*/

void otp_main(void)
{
	
	uart_string("at\r\n");
	delay_sec(2);
	uart_string("at+cmgf=1\r\n");
	delay_sec(2);
	uart_string("at+cmgs=\"+917358923067\"\r\n");
	delay_sec(2);
	uart_string(s);
	delay_sec(2);
	transmit(0X1A);
	delay_sec(10);
	uart_string("atd7358923067;\r\n");
	delay_sec(2);
}
