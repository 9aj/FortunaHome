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
void blinkConfirm();

volatile rectangle highlight;
volatile int rotaryStore;
volatile int changeState = 0;


ISR(TIMER1_COMPA_vect) {

   if(get_middle() == 1) {
      changeState = 1;
      toggleMenu();
      while(changeState == 1) {
         if(changeState == 0) {
            break;
         }
      }
   }

   
   rotaryStore = rotary;
   
}

ISR(INT5_vect, ISR_ALIASOF(INT4_vect));

ISR(INT4_vect)
{
	get_rotary();
	_delay_us(500);

   switch(rotary) {
      case 1: //Toggle main living room lights
         drawHighlighter("1. Toggle main living room lights",74);
         //highlight.top = 76;
         //highlight.bottom = 79;
         break;
      case 2: //Toggle living room lampshade
         drawHighlighter("2. Toggle living room lampshade",89);

         //highlight.top = 91;
         //highlight.bottom = 94;
         //blinkConfirm();
         break;
      case 3: //Toggle outside light
         drawHighlighter("3. Toggle outside light",104);

         //highlight.top = 106;
         //highlight.bottom = 109;
         break;
      case 4: //Return energy usage today/cost
         drawHighlighter("4. Return energy usage",119);

         //highlight.top = 121;
         //highlight.bottom = 124;
         //blinkConfirm();
         break;
      case 5: //Check serial interface
         drawHighlighter("5. Toggle main living room lights",134);

         //highlight.top = 136;
         //highlight.bottom = 139;

         //blinkConfirm();
         break;
      case 6: //Options
         drawHighlighter("6. Options",149);
         //highlight.top = 151;
         //highlight.bottom = 154;
         break;
   }
}

int main(void) {

   CLKPR = (1 << CLKPCE); /* no prescaling for F_CPU */
   CLKPR = 0;

   LED_INIT;
   init_lcd();
   init_rotary();

   set_frame_rate_hz(61);
   EIMSK |= _BV(INT7);
   EIMSK |= _BV(INT4) | _BV(INT5);
   //TCCR1A = 0;
	//TCCR1B = _BV(WGM12);
	//TCCR1B |= _BV(CS10);
	//TIMSK1 |= _BV(OCIE1A);
   launchApplication();
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
   mainMenu();
   for(;;){
      sei();
      if(get_middle() == 1) {
         toggleMenu();
      }
   }
}

void blinkConfirm() {
   for(int i = 0; i < 2; i++) {
      LED_ON;
      _delay_ms(400);
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



