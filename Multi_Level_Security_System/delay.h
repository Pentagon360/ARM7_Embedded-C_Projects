//delay.h
void delay_ms(unsigned int d)
{
	T0PR = 15000-1;
	T0TCR = 0X01;
	while(T0TC<d);
	T0TCR = 0X03;
	T0TCR = 0X00;
}
void delay_sec(unsigned int d)
{
	T0PR = 15000000-1;
	T0TCR = 0X01;
	while(T0TC<d);
	T0TCR = 0X03;
	T0TCR = 0X00;
}
void delay_micro(unsigned int d)
{
	T0PR = 15-1;
	T0TCR = 0X01;
	while(T0TC<d);
	T0TCR = 0X03;
	T0TCR = 0X00;
}
