/*	Author: Benjamin Shu
 *  Partner(s) Name: 
 *	Lab Section: 22 
 *	Assignment: Final Project Part #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"
#include "scheduler.h"
char patterns[8] = {0x80,0x40, 0x20, 0x10, 0x08, 0x04, 0x02,0x01};
//char patterns[15] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
//char rows[4] = {0x1B, 0x1B, 0x1B, 0x1B};
unsigned char p1_movement[3] = {0x03, 0x11, 0x18};
unsigned char p1_index = 1;
unsigned char p1_col = 0x80;

unsigned char mid_row[8] = {0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B};
unsigned char mid_bounce[8] = {0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char rows[8] = {0x1E, 0x1D, 0x1B, 0x17, 0x0F, 0x17, 0x1B, 0x1D};

unsigned char p2_movement[3] = {0x03, 0x11, 0x18};
unsigned char p2_index = 1;
unsigned char p2_col = 0x01;

unsigned char p = 0;
unsigned char r = 0;
unsigned char i = 0;


//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum p1_states {start, wait, up, uprelease, down, downrelease};
int p1_tick(int state) {
	switch (state) {
		case start:
			state = wait;
			break;
		case wait:
			if((~PINA & 0x01) == 0x01) {
				state = up;
			}
			else if((~PINA & 0x02) == 0x02) {
				state = down;
			}
			else {	
				state = wait;
			}
			break;
		case up:
			state = uprelease;
			break;
		case uprelease:
			if((~PINA & 0x01) == 0x01) {
				state = uprelease;
			}
			else {
				state = wait;
			}
			break;
		case down:
			if((~PINA & 0x02) == 0x02) {
				state = downrelease;
			}
			else {
				state = down;
			}
			break;
		case downrelease:
			if((~PINA & 0x02) == 0x02) {
				state = downrelease;
			}
			else {
				state = wait;
			}
			break;
		default:	
			state = start;
			break;
	}	
	// Actions
	switch (state) {
		case start:	
			break;
		/*case two:
			pattern = 0x01;
			row = 0x11;
			break;
		*/
		case wait:
			break;
		case up:
			if(p1_index < 2) {
				p1_index++;
			}
			break;
		case uprelease:
			break;
		case down:
			if(p1_index > 0) {
				p1_index--;
			}
			break;
		case downrelease:
			break;

		default:
			break;
	}
	return state;
}
enum p2_states {p2_start, p2_wait, p2_up, p2_uprelease, p2_down, p2_downrelease} p2_state;

int p2_tick(int p2_state) {
	switch(p2_state) {
		case p2_start:
			p2_state = p2_wait;
			break;
		case p2_wait:
			if((~PINA & 0x04) == 0x04) {
				p2_state = p2_up;
			}
			else if((~PINA & 0x08) == 0x08) {
				p2_state = p2_down;
			}
			else {
				p2_state = p2_wait;
			}
			break;
		case p2_up:
			if((~PINA & 0x04) == 0x04) {
				p2_state = p2_uprelease;
			}
			else {
				p2_state = p2_up;
			}
			break;
		case p2_uprelease:
			if((~PINA & 0x04) == 0x04) {
				p2_state = p2_uprelease;
			}
			else {
				p2_state = p2_wait;
			}
			break;
		case p2_down:
			if((~PINA & 0x08) == 0x08) {
				p2_state = p2_downrelease;
			}
			else {
				p2_state = p2_down;
			}
			break;
		case p2_downrelease:
			if((~PINA & 0x08) == 0x08) {
				p2_state = p2_downrelease;
			}
			else {
				p2_state = p2_wait;
			}
			break;
		default:
			p2_state = p2_start;
			break;
	}
	// Actions
	switch (p2_state) {
		case p2_start:	
			break;
		/*case two:
			pattern = 0x01;
			row = 0x11;
			break;
		*/
		case p2_wait:
			break;
		case p2_up:
			if(p2_index < 2) {
				p2_index++;
			}
			break;
		case p2_uprelease:
			break;
		case p2_down:
			if(p2_index > 0) {
				p2_index--;
			}
			break;
		case p2_downrelease:
			break;

		default:
			break;
	}
	return p2_state;
}


enum led_display {waitp1, p1update, waitmove, waitp2, p2update, ballupdate} led;
unsigned char row_movement = 0;
int led_tick(int led) {
	switch(led) {
		case waitp1:
			PORTD = p1_movement[row_movement];
			PORTC = 0x80;
			p1_col = 0x80;
			p2_col = 0x01;
			led = p1update;
			break;
		case waitp2:
			PORTC = 0x01;
			PORTD = p1_movement[row_movement];
			p1_col = 0x80;
			p2_col = 0x01;
			led = p1update;
			break;
		case p1update:
			PORTD = p1_movement[p1_index];
			PORTC = p1_col;
			led = p2update;
			break;
		case p2update:
			PORTC = p2_col;
			PORTD = p2_movement[p2_index];
			led = ballupdate;
			break;
		case ballupdate:
			PORTC = patterns[i];
			PORTD = rows[i];
			PORTC = mid_

			led = p1update;
			break;
		case waitmove:
			if((~PINA & 0x01) == 0x01 || ((~PINA & 0x02) == 0x02)) {
				led = p1update;
			}
			else {
				led = ballupdate;
			}
			break;
	}
	return led;
}

enum ball_states {begin,move} state1;
unsigned char p1_score = 0;
unsigned char p2_score = 0;

int ball_tick(int state1) {
	switch(state1) {
		case begin:
			state1 = move;
			break;
		case move:
			state1 = move;
		default:
			state1 = move;
			break;
	}
	switch(state1) {
		case begin:
			if((~PINA & 0x01) == 0x01) {
				state1 = begin;
			}
			else {
				state1 = move;
			}
		case move:
			if(i < 15) {
				p = patterns[i];
				r = rows[i];
				i++;
				if(r = 0x
			}
			else {
				i = 0;
			}
			break;
	}
	return state1;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0X00;
    /* Insert your solution below */
	//static task task1;
	//task *tasks[] = {&task1};
	static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	const char start = 0;

	//task 1 paddle 1
	task1.state = start;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &p1_tick;
	
	//task 2 led display
	task2.state = start;
	task2.period = 3;
	task2.elapsedTime = task2.period;
	task2.TickFct = &led_tick;
	
	//task 3 paddle 2
	task3.state = start;
	task3.period = 1;
	task3.elapsedTime = task3.period;
	task3.TickFct = &p2_tick;
	
	//task 4 ball
	task4.state = start;
	task4.period = 200;
	task4.elapsedTime = task4.period;
	task4.TickFct = &ball_tick;
	TimerSet(1);
	TimerOn();
	unsigned short i;

    while (1) {
		for(i = 0; i < numTasks; i++) { //scheduler code
			if(tasks[i]->elapsedTime == tasks[i]->period) { //task ready to tick
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state); //set next state
				tasks[i]->elapsedTime = 0; //reset elapsed time for next tick
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
