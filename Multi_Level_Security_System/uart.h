//uart.h
void uart_config(void)
{
	PINSEL0 |= 0X05;
	U0LCR = 0X83;
	U0DLL = 97;
	U0LCR = 0X03;
}

void transmit(unsigned char ch)
{
	while((U0LSR &(1<<5))==0);
	U0THR = ch;
}
unsigned char recieve()
{
	while((U0LSR &(1<<0))==0);
	return U0RBR;
}
void uart_string(unsigned char* s)
{
		while(*s)
		{
				transmit(*s++);
		}
}

void uart_int(unsigned int n)
{
	unsigned char arr[5],ch;
	int i =0;
	if(n == 0)
	{
		transmit('0');
	}
	else
	{
		if(n<0)
		{
			transmit('-');
			n = -n;
		}
		while(n>0)
		{
			arr[i++] = n%10;
			n = n/10;
		}
		for(--i;i>=0;i--)
		{
			 ch=(arr[i]+48);
			transmit(ch);
		}
	}
}
