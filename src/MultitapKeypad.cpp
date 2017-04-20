/*
  MultitapKeypad.h - Library for matrix 4x3 phone's keypad
  as well as matrix 4x4 keypad, supports multi-tap and long-tap.
  
  Created by ZulNs, @Gorontalo, Indonesia, 15 April 2017
  Released into the public domain.
*/

#include "Arduino.h"
#include "MultitapKeypad.h"

uint8_t MultitapKeypad::keyPins[8];
uint8_t MultitapKeypad::beforeLastKeyCode = NO_KEY;
uint8_t MultitapKeypad::lastKeyCode = NO_KEY;
uint8_t MultitapKeypad::currentKeyCode;
uint8_t MultitapKeypad::tapCounter = 0;
uint32_t MultitapKeypad::multitapTimeout;
uint32_t MultitapKeypad::longtapTimeout;
bool MultitapKeypad::longtapState = false;
void ( *MultitapKeypad::user_function ) ( void );

MultitapKeypad::MultitapKeypad( uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3 ) {
	keyPins[0] = r0;
	keyPins[1] = r1;
	keyPins[2] = r2;
	keyPins[3] = r3;
	keyPins[4] = c0;
	keyPins[5] = c1;
	keyPins[6] = c2;
	keyPins[7] = c3;
}

Key MultitapKeypad::getKey( void ) {
	uint8_t code;
	Key key;
	do {
		code = scanKeyCode();
		if ( longtapState && millis() >= longtapTimeout ) {
			key.code = currentKeyCode;
			key.lastCode = lastKeyCode;
			key.beforeLastCode = beforeLastKeyCode;
			key.character = getChr( currentKeyCode );
			key.state = LONG_TAP;
			longtapState = false;
			return key;
		}
		if ( user_function )
			user_function();
	}
	while ( code == currentKeyCode );
	beforeLastKeyCode = lastKeyCode;
	lastKeyCode = currentKeyCode;
	currentKeyCode = code;
	key.code = currentKeyCode;
	key.lastCode = lastKeyCode;
	key.beforeLastCode = beforeLastKeyCode;
	if ( currentKeyCode == NO_KEY ) {
		if ( !isMultiKeyDown( lastKeyCode ) && beforeLastKeyCode == NO_KEY )
			key.character = getChr( lastKeyCode );
		key.state = KEY_UP;
		longtapState = false;
	}
	else if ( !isMultiKeyDown( currentKeyCode ) && lastKeyCode == NO_KEY ) {
		longtapTimeout = millis() + LONGTAP_PERIODS;
		longtapState = true;
		key.character = getChr( currentKeyCode );
		if ( currentKeyCode == beforeLastKeyCode && millis() < multitapTimeout ) {
			key.tapCounter = ++tapCounter;
			key.state = MULTI_TAP;
		}
		else {
			tapCounter = 0;
			key.state = KEY_DOWN;
		}
		multitapTimeout = millis() + MULTITAP_PERIODS;
	}
	else {
		key.state = MULTI_KEY_DOWN;
		tapCounter = 0;
		longtapState = false;
	}
	return key;
}

uint8_t MultitapKeypad::scanKeyCode( void ) {
	uint8_t rows = 0, cols = 0, i;
	do {
		// Set all row pins to input mode with pull-up resistor activated
		for ( i = 0; i < 4; i++ )
			pinMode( keyPins[ i ], INPUT_PULLUP );
		// Set all column pins to output low
		for ( ; i < 8; i++ ) {
			if ( keyPins[ i ] == 255 )
				break;
			digitalWrite( keyPins[ i ], LOW );
			pinMode( keyPins[ i ], OUTPUT );
		}
		do {
			// Reads the logic from all row pins
			rows = scanRows();
			// Debouncing time set to every 20 ms
			delay( 20 );
		}
		// Repeat reading while the contact still bouncing
		while ( rows != scanRows() );
		// Reverse the I/O mode for all row and column pins
		// Set all column pins to input mode with pull-up resistor activated
		for ( i = 4; i < 8; i++ ) {
			if ( keyPins[ i ] == 255 )
				break;
			pinMode( keyPins[ i ], INPUT_PULLUP );
		}
		// Set all row pins to output low
		for ( i = 0; i < 4; i++ ) {
			digitalWrite( keyPins[ i ], LOW );
			pinMode( keyPins[ i ], OUTPUT );
		}
		// Reads the logic from all column pins
		for ( ; i < 8; i++ )
			bitWrite( cols, i - 4, ( ( keyPins[ i ] == 255 ) ? HIGH : digitalRead( keyPins[ i ] ) ) );
	}
	// Repeat reading while only rows or columns has active key pressed
	while ( rows == 15 && cols != 15 || rows != 15 && cols == 15 );
	// Set all key pins to high impedance input. Allows sharing these pins with other hardware.
	for ( i = 0; i < 8; i++ ) {
		if ( keyPins[ i ] == 255 )
			break;
		pinMode( keyPins[ i ], INPUT );
	}
	// Every row and column pins normally at high state caused by
	// pull-up resistor and become low if the attached key was pressed.
	// The 8-bits reading code consist off:
	// Bit-7 | Bit-6 | Bit-5 | Bit-4 | Bit-3 | Bit-2 | Bit-1 | Bit-0
	// Col-3 | Col-2 | Col-1 | Col-0 | Row-3 | Row-2 | Row-1 | Row-0
	return cols << 4 | rows;
}

uint8_t MultitapKeypad::scanRows( void ) {
	uint8_t rows = 0;
	for ( uint8_t i = 0; i < 4; i++ )
		bitWrite( rows, i, digitalRead( keyPins[ i ] ) );
	return rows;
}

uint8_t MultitapKeypad::getChr( uint8_t code ) {
	return pgm_read_byte_near(
		KEY_CHARACTER +
		getIndex( code ) * 4 +
		getIndex( code >> 4 )
	);
}

uint8_t MultitapKeypad::getIndex( uint8_t code ) {
	uint8_t idx = 0;
	while ( bitRead( code, 0 ) ) {
		code >>= 1;
		idx++;
	}
	return idx;
}

bool MultitapKeypad::isMultiKeyDown( uint8_t code ) {
	// Normally the key code only has two low state among bit-6 to bit-0,
	// one for each rows (bit-3 to bit-0) and columns (bit-7 to bit-4).
	// If it has more than two low state, then there are more than one key
	// which has been pressed.
	uint8_t ctr = 0;
	for ( uint8_t i = 0; i < 8; i++ ) {
		if ( !bitRead( code, i ) ) ctr++;
	}
	return ctr > 2;
}
