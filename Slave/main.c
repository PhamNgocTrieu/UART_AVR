
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <myLCD.h>
#include <stdio.h>

volatile unsigned char rdata = 0, sla_address, dataIN;
volatile unsigned char dis[5], temp[3];

void uart_char_tx(unsigned char chr);
void uart_address_tx(unsigned char chr);

int main(void){
	PORTC = 0xff;
	DDRC = 0x00;
	sla_address = PINC & 0x03; //set Slave's address
	
	init_LCD();
	
	//set toc do baud, 57.6k ung voi f=8Mhz, xem bang 70 trang 165, Atmega8 datasheet
	UBRRH=0;
	UBRRL=8;
	
	//set khung truyen va kich hoat bo nhan du lieu
	UCSRA=(1<<MPCM);
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<UCSZ2);
	sei();
	_delay_ms(10);
	sprintf(dis, "%i", sla_address);
	print_LCD(dis);
	while(1)
	{
		clr_LCD();
		sprintf(dis, "%i", temp[1]);
		print_LCD(dis);
		move_LCD(2,1);
		sprintf(dis, "%i", temp[2]);
		print_LCD(dis);
		_delay_ms(500);
		
	}
}

ISR(USART_RXC_vect){
	rdata = UDR;
	
	if (dataIN == 0)
	{
		if (sla_address == rdata)
		{
			UCSRA &= ~(1<<MPCM);// reset MPCM to ready for receiving data
			dataIN++;
		}
	}
	else
	{
		temp[dataIN] = rdata;
		dataIN++;
		if (dataIN == 3)
		{
			UCSRA |= (1<<MPCM); // set MPCM for next time
			dataIN = 0;
		}
	}
}
