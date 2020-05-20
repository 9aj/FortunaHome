/* Fortuna Home */
/* Author: Alexander Miles */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "lcd.h"
#include <string.h>
#include <rotary.h>

#define LED_INIT    DDRB  |=  _BV(PINB7)
#define LED_ON      PORTB |=  _BV(PINB7)
#define LED_OFF     PORTB &= ~_BV(PINB7)
#define LED_TOGGLE  PINB  |=  _BV(PINB7)

#define CHAR_X 6
#define CHAR_Y 8

/*
   Characters are 8 tall and 6 wide
   
   Display is 320 wide and 240 tall
   320/2 = 160 mid
   240/2 = 120 mid
   
*/

void drawHighlighter(char *inputText, uint16_t y);
void launchApplication();
void blinkConfirm(int j);
void configureUSART();
void printResponse(char response);
void toggleMenu();
void mainMenu();

volatile rectangle highlight;
volatile int rotaryStore;
volatile int changeState = 0;

ISR(INT5_vect, ISR_ALIASOF(INT4_vect));

ISR(INT4_vect)
{
	get_rotary();
	_delay_us(500);

   switch(rotary) {
      case 1: //Toggle main living room lights
         drawHighlighter("1. Toggle main living room lights",74);
         UDR1 = '1';

         break;
      case 2: //Toggle living room lampshade
         drawHighlighter("2. Toggle living room lampshade",89);
         UDR1 = '2';
         break;
      case 3: //Toggle outside light
         drawHighlighter("3. Toggle outside light",104);
         UDR1 = '3';
         break;
      case 4: //Return energy usage today/cost
         drawHighlighter("4. Return energy usage",119);
         UDR1 = '4';
         break;
      case 5: //Check serial interface
         drawHighlighter("5. Toggle main living room lights",134);
         UDR1 = '5';
         break;
      case 6: //Options
         drawHighlighter("6. Options",149);
         UDR1 = '6';
         break;
   }
}

ISR(USART_RXC_vect) {
   char ReceivedByte;
   ReceivedByte = UDR1;
   printResponse(ReceivedByte);

}

int main(void) {

   CLKPR = (1 << CLKPCE); /* no prescaling for F_CPU */
   CLKPR = 0;

   LED_INIT;
   init_lcd();
   init_rotary();
   configureUSART();

   set_frame_rate_hz(61);
   EIMSK |= _BV(INT7);
   EIMSK |= _BV(INT4) | _BV(INT5);
   launchApplication();
}

/*
 * Configure USART()
 * Pulled from https://www.avrfreaks.net/forum/tut-soft-using-usart-interrupt-driven-serial-comms?page=all
*/
void configureUSART() {
   uint16_t ubrr_value = 103;
   UBRR1 = ubrr_value;
   UCSR1B = (1<<RXEN1)|(1<<TXEN1);
   UCSR1C = (1<<UCSZ10)|(1<<UCSZ11);
}

void printResponse(char response) {
   switch(response) {
      case '1':
         clear_screen();
         display_string_xy("Request Completed: 1", 100, 200);
         break;
      case '2':
         clear_screen();
         display_string_xy("Request Completed: 2", 90, 200);
         break;
      case '3':
         clear_screen();
         display_string_xy("Request Completed: 3", 90, 200);
         break;
      case '4':
         clear_screen();
         display_string_xy("Request Completed: 4", 90, 200);
         break;
      case '5':
         clear_screen();
         display_string_xy("Request Completed: 5", 90, 200);
         break;
      case '6':
         clear_screen();
         display_string_xy("Request Completed: 6", 90, 200);
         break;
   }
}
 
void toggleMenu() {
   clear_screen();
   fill_rectangle(highlight, display.background);
   display_string_xy("Request Received", 100, 200);
   _delay_ms(500);
   clear_screen();
   changeState = 0;
   mainMenu();

}

void mainMenu() {
   display_string_xy("FortunaHome", 120, 20);
   display_string_xy("1. Toggle main living room lights", 60, 75);
   display_string_xy("2. Toggle living room lampshade", 60, 90);
   display_string_xy("3. Toggle outside light", 60, 105);
   display_string_xy("4. Toggle bedroom led lights", 60, 120);
   display_string_xy("5. Return total daily energy usage", 60, 135);
   display_string_xy("6. Options", 60, 150);
}

void launchApplication() {
   int flickerFix = 0;
   mainMenu();
   for(;;){
      sei();
      if(get_middle() == 1 && flickerFix == 0) {
         toggleMenu();
         _delay_ms(25);
         flickerFix = 1;
      }
      flickerFix = 0;
   }
}

void blinkConfirm(int j) {
   for(int i = 0; i < j; i++) {
      LED_ON;
      _delay_ms(50);
      LED_OFF;
   }
}

void drawHighlighter(char *inputText, uint16_t y) {
   
   fill_rectangle(highlight, display.background);
   
   long int stringLength = strlen(inputText);
   uint16_t highlighterLength = (uint16_t) stringLength*(CHAR_X+2);

   highlight.top = y+4+6;
   highlight.bottom = y+5+6;
   highlight.left = 60;
   highlight.right = highlighterLength;
   fill_rectangle(highlight, RED);
}



