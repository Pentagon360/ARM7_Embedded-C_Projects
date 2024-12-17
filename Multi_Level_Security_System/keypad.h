#define c0 (IOPIN0 & (1<<16) )
#define c1 (IOPIN0 & (1<<17) )
#define c2 (IOPIN0 & (1<<18) )
#define c3 (IOPIN0 & (1<<19) )
#define r0 1<<20
#define r1 1<<21
#define r2 1<<22
#define r3 1<<23

//unsigned int keypad[4][4] ={ {1,2,3,4} , {5,6,7,8} ,{9,10,11,12} ,{13,14,15,16} };
unsigned int keypad[4][4] ={ {48,49,50,51} , {52,53,54,55} ,{56,57,65,66} ,{67,68,69,70} };
char pass[5];
unsigned char keyscan(void)
{
	unsigned char row,col;
	PINSEL0 |=0x0;
	IODIR0 |= r0|r1|r2|r3;
	while(1)
	{
		IOCLR0 |= r0|r1|r2|r3;
		IOSET0 |= c0|c1|c2|c3;
		while( (c0 && c1 && c2 && c3) ==1); //wait untill the key is pressed
		 
		//testinf for r0
		IOCLR0 |= r0;
		IOSET0 |= r1|r2|r3;
		if( (c0 && c1 && c2 && c3)==0)
		{
			row = 0;
			break;
		}
		
		//testing for r1
		IOCLR0 |= r1;
		IOSET0 |= r0|r2|r3;
		if( (c1 && c0 && c2 && c3)==0)
		{
			row = 1;
			break;
		}
		
		//testing for r2
		IOCLR0 |= r2;
		IOSET0 |= r1|r0|r3;
		if( (c2 && c1 && c0 && c3)==0)
		{
			row = 2;
			break;
		}
		
		//testing for r3
		IOCLR0 |= r3;
		IOSET0 |= r0|r2|r1;
		if( (c3&&c1&&c2&&c0)==0)
		{
			row = 3;
			break;
		}
	}
	if(c0==0)
		col=0;
	else if(c1==0)
		col=1;
	else if(c2==0)
		col=2;
	else if(c3== 0)
		col=3;
	
	delay_ms(750);						  //for switch bouncing
	while( (c0 && c1 && c2 && c3) ==0);

	return keypad[row][col];
}
void keypad_main(void)
{
	int i=0;
	lcd_command(0xC0);
	while(i<4)
	{
		pass[i] = keyscan();
		uart_string("*");
		lcd_string("*");
		delay_ms(500);
		i++;
	}
	pass[i]='\0';
	uart_string("\r\n");
	uart_string("your written password\r\n");
	uart_string(pass);
	uart_string("\r\n");
}
