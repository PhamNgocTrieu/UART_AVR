
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


volatile unsigned char u_data=0;
void uart_char_tx(unsigned char chr);
void uart_address_tx(unsigned char chr);

int main(void){
	//set toc do baud, 57.6k ung voi f=8Mhz, xem bang 70 trang 165, Atmega32 datasheet
	UBRRH=0;
	UBRRL=8;
	//set khung truyen va kich hoat bo nhan du lieu
	UCSRA=0x00;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); //9 bit frame
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE)|(1<<UCSZ2);
	
	//phat du lieu
	_delay_ms(10);
	uart_address_tx(1); //phat dia chi 1
	uart_char_tx(10);
	uart_char_tx(20);
	
	uart_address_tx(2); //phat dia chi 2
	uart_char_tx(30);
	uart_char_tx(40);
	
	while(1){
		//do nothing
	}
	return 1;
}
//chuong trinh con phat du lieu
void uart_char_tx(unsigned char chr){
	while ( !( UCSRA & (1<<UDRE))) ; //cho den khi bit UDRE=1 moi thoat khoi while
	UCSRB &= ~(1<<TXB8); //reset the 9th bit
	UDR=chr;
}
//chuong trinh con phat dia chi
void uart_address_tx(unsigned char chr){
	while ( !( UCSRA & (1<<UDRE))) ; //cho den khi bit UDRE=1 moi thoat khoi while
	UCSRB |= (1<<TXB8);
	UDR=chr;
}
