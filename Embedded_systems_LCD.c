#include "mkl25z4.h"
#include <string.h>
#define MASK(x)	(1UL << x)

static void initialize(void);
void delay_ms(int delay);
void lcd_write_instruc(unsigned char instruction);
void lcd_write_char(unsigned char c);
void lcd_init(void);
void lcd_clear(void);
void lcd_goto(unsigned char column, unsigned char row);
void lcd_write_string(char *s);
void lcd_write_long_string(char *row1,char *row2);
	

int main()
{
	//defining pins to map with ports
	#define RS (12)
	#define E (13)
	#define D7 (11)
	#define D6 (10)
	#define D5 (6)
	#define D4 (5)
	#define D3 (4)
	#define D2 (3)
	#define D1 (0)
	#define D0 (7)
	#define BLUE (1) //PTD 1
		
	initialize();
	lcd_init();
	
	
	lcd_clear();
	lcd_goto(0,0);
	lcd_write_string("Issifu Alhassan");
	lcd_goto(0,1);	
	lcd_write_string(" Just  trying ");
	
	PTD->PTOR=MASK(BLUE);
	delay_ms(200);
	PTD->PTOR=MASK(BLUE);
	delay_ms(2000);
	
	lcd_write_long_string("This is a long string occupying the first row. Scrolling achieved! ","This long string in the second row deserves some sleep sleep ... ");
	
	PTD->PTOR=MASK(BLUE);
	delay_ms(200);
	PTD->PTOR=MASK(BLUE);
	delay_ms(2000);
	
	lcd_clear();
	lcd_goto(0,0);
	lcd_write_string("By Issifu");
	lcd_goto(0,1);	
	lcd_write_string("Thank you!");
	
	while (1)
  {
  }
}

/*Initializes pots and pins to be used on board*/
static void initialize()
{
	//clock gating to port A, B, C & D
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK; ////gate port C n D

	//set up pins as GPIO
	PORTC->PCR[RS] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[RS] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[E] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[E] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D7] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D7] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D6] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D6] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D5] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D5] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D4] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D4] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D3] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D3] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D2] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D2] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D1] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D1] |= PORT_PCR_MUX(1);	//setup to be GPIO
	PORTC->PCR[D0] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTC->PCR[D0] |= PORT_PCR_MUX(1);	//setup to be GPIO
	
	PORTD->PCR[BLUE] &= ~PORT_PCR_MUX_MASK;	//Clear mux
	PORTD->PCR[BLUE] |= PORT_PCR_MUX(1);	//setup to be GPIO

	//set up outputs
	PTC->PDDR |= MASK(RS);
	PTC->PDDR |= MASK(E);
	PTC->PDDR |= MASK(D7);
	PTC->PDDR |= MASK(D6);
	PTC->PDDR |= MASK(D5);
	PTC->PDDR |= MASK(D4);
	PTC->PDDR |= MASK(D3);
	PTC->PDDR |= MASK(D2);
	PTC->PDDR |= MASK(D1);
	PTC->PDDR |= MASK(D0);
	
	PTD->PDDR |= MASK(BLUE);
	
	PTD->PSOR=MASK(BLUE);
}

void delay_ms(int delay) 
{    
	SysTick->LOAD = (20971520u/1000u)-1;  //roll over every millisecond.    
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; //this is 5 
	
	for (int i=0; i<delay; i++)
	{     	
		unsigned int ctr=0;     
		while (1)
		{      
			if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
			{     
				ctr++; //count the milliseconds     
				if(ctr==1)
				{      
					ctr=0;//one second has passed.      
					break;
				}      
			}    
		}	    
	}
	SysTick->CTRL  = 0; //Stop the timer
}

void lcd_write_instruc(unsigned char instruction)
{
	delay_ms(2);	
	PTC->PCOR=MASK(RS);//set RS pin LOW, it is an instruction not data
	PTC->PCOR=MASK(E);//set E LOW,
	//write instruction
	int writes[] = {D0,D1,D2,D3,D4,D5,D6,D7};
	for(int i=0;i<8;i++)
	{
		if(instruction & MASK(i))
		{
			PTC->PSOR=MASK(writes[i]);
		}
		else
		{
			PTC->PCOR=MASK(writes[i]);
		}
	}
	
	//set E  HIGH, then LOW, (ge
	PTC->PSOR=MASK(E);
	PTC->PCOR=MASK(E);
}

void lcd_write_char(unsigned char c)
{
	delay_ms(2);
	PTC->PSOR=MASK(RS);//set RS pin HIGH, it is data
	PTC->PCOR=MASK(E);//set E LOW,
	//write data
	
	int writes[] = {D0,D1,D2,D3,D4,D5,D6,D7};
	for(int i=0;i<8;i++)
	{
		if(c & MASK(i))
		{
			PTC->PSOR=MASK(writes[i]);
		}
		else
		{
			PTC->PCOR=MASK(writes[i]);
		}
	}
	
	//set E  HIGH, then LOW, (generate falling edge to write)
	PTC->PSOR=MASK(E);
	PTC->PCOR=MASK(E);
}


void lcd_init(void)
{
	delay_ms(2);
	lcd_write_instruc(0x06);        //Increment mode for the cursor
	lcd_write_instruc(0x0C);        //The display on, the cursor off
	lcd_write_instruc(0x38);        //An 8-bit data bus, two-line display
}


void lcd_clear(void)
{
	delay_ms(2);
	lcd_write_instruc(0x01);        //Clear the display
	lcd_write_instruc(0x02);        //returns home, even if shifted/but not clear display
}


void lcd_goto(unsigned char column, unsigned char row)
{
	delay_ms(2);
	if(row==0)
	lcd_write_instruc(0x80 + column);
	if(row==1)
	lcd_write_instruc(0xC0+ column);
}

void lcd_write_string(char *s)
{
	delay_ms(2);
	while(*s != 0)
	{
		lcd_write_char(*s);
		s++;
	}
	
}

/*Print long strings in a scrolling manner*/
void lcd_write_long_string(char *row1,char *row2)
{
	lcd_clear();
	delay_ms(2);
	int len1 = strlen(row1);
	int len2 = strlen(row2);
	int max_len = len1;
	if(len2>len1)
	{
		max_len = len2;
	}
	char print1[16];
	char print2[16];
	if(len1 <= 16 || len2 <= 16)
	{
		if(len1 <= 16)
		{
			lcd_goto(0,0);
			lcd_write_string(row1);
		}
		else
		{
			int i;
			//scrolling forward
			for(i=0;i<max_len - 16 ;i++)
			{
				for(int j=0;j<16;j++)
				{
					print1[j] = row1[i+j];
				}
				lcd_clear();
				lcd_goto(0,0);
				lcd_write_string(print1);
				lcd_goto(0,1);
				lcd_write_string(row2);
				delay_ms(200);
			}
			//scrolling backward
			for(i=max_len - 16;i>=0 ;i--)
			{
				for(int j=0;j<16;j++)
				{
					print1[j] = row1[i+j];
				}
				lcd_clear();
				lcd_goto(0,0);
				lcd_write_string(print1);
				lcd_goto(0,1);
				lcd_write_string(row2);
				delay_ms(200);
			}
		}
		if(len2 <= 16)
		{
			lcd_goto(0,1);
			lcd_write_string(row2);
		}
		else
		{
			int i;
			//scrolling forward
			for(i=0;i<max_len - 16 ;i++)
			{
				for(int j=0;j<16;j++)
				{
					print2[j] = row2[i+j];
				}
				lcd_clear();
				lcd_goto(0,0);
				lcd_write_string(row1);
				lcd_goto(0,1);
				lcd_write_string(print2);
				delay_ms(200);
			}
			//scrolling backward
			for(i=max_len - 16;i>=0 ;i--)
			{
				for(int j=0;j<16;j++)
				{
					print2[j] = row2[i+j];
				}
				lcd_clear();
				lcd_goto(0,0);
				lcd_write_string(row1);
				lcd_goto(0,1);
				lcd_write_string(print2);
				delay_ms(200);
			}
		}
		return;
	}
	else
	{
		int i;
		//scrolling forward
		for(i=0;i<max_len - 16 ;i++)
		{
			int j;
			if(i+16 < len1)
			{
				for(j=0;j<16;j++)
				{
					print1[j] = row1[i+j];
				}
			}
			if(i+16 < len2)
			{
				for(j=0;j<16;j++)
				{
					print2[j] = row2[i+j];
				}
			}
			lcd_clear();
			lcd_goto(0,0);
			lcd_write_string(print1);
			lcd_goto(0,1);
			lcd_write_string(print2);
			delay_ms(200);
		}
		//scrolling backward
		for(i=max_len - 16;i>=0;i--)
		{
			int j;
			if(i+16 < len1)
			{
				for(j=0;j<16;j++)
				{
					print1[j] = row1[i+j];
				}
			}
			if(i+16 < len2)
			{
				for(j=0;j<16;j++)
				{
					print2[j] = row2[i+j];
				}
			}
			lcd_clear();
			lcd_goto(0,0);
			lcd_write_string(print1);
			lcd_goto(0,1);
			lcd_write_string(print2);
			delay_ms(200);
		}
	}	
}
