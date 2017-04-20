/*
  MultitapKeypad.h - Library for matrix 4x3 phone's keypad
  as well as matrix 4x4 keypad, supports multi-tap and long-tap.
  
  Created by ZulNs, @Gorontalo, Indonesia, 15 April 2017
  Released into the public domain.
*/

#ifndef MultitapKeypad_h
#define MultitapKeypad_h

#include "Arduino.h"

const uint16_t MULTITAP_PERIODS = 1000;
const uint16_t LONGTAP_PERIODS = 1000;
const uint8_t NO_KEY = 0xFF;
const uint8_t KEY_1 = 0xEE;
const uint8_t KEY_2 = 0xDE;
const uint8_t KEY_3 = 0xBE;
const uint8_t KEY_A = 0x7E;
const uint8_t KEY_4 = 0xED;
const uint8_t KEY_5 = 0xDD;
const uint8_t KEY_6 = 0xBD;
const uint8_t KEY_B = 0x7D;
const uint8_t KEY_7 = 0xEB;
const uint8_t KEY_8 = 0xDB;
const uint8_t KEY_9 = 0xBB;
const uint8_t KEY_C = 0x7B;
const uint8_t KEY_ASTERISK = 0xE7;
const uint8_t KEY_0 = 0xD7;
const uint8_t KEY_NUMBER_SIGN = 0xB7;
const uint8_t KEY_D = 0x77;
enum KEY_STATE { KEY_DOWN, MULTI_TAP, LONG_TAP, MULTI_KEY_DOWN, KEY_UP };

const char KEY_CHARACTER[] PROGMEM = { '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D' };

struct Key {
	uint8_t code;
	uint8_t lastCode;
	uint8_t beforeLastCode;
	uint8_t character = 0;
	uint8_t tapCounter = 0;
	KEY_STATE state;
};

class MultitapKeypad {
	public:
		MultitapKeypad( uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3 = 255 );
		static Key getKey( void );
		inline static void resetTapCounter( void ) { tapCounter = 0; }
  		inline static void attachFunction( void ( *funct ) ( void ) ) { user_function = funct; }

	private:
		static uint8_t keyPins[];
		static uint8_t beforeLastKeyCode;
		static uint8_t lastKeyCode;
		static uint8_t currentKeyCode;
		static uint8_t tapCounter;
		static uint32_t multitapTimeout;
		static uint32_t longtapTimeout;
		static bool longtapState;
		
		static uint8_t scanKeyCode( void );
		static uint8_t scanRows( void );
		static uint8_t getChr( uint8_t );
		static uint8_t getIndex( uint8_t );
		static bool isMultiKeyDown( uint8_t code );
		static void ( *user_function ) ( void );
};

#endif
