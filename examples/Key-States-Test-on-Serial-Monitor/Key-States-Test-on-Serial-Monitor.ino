/*
 Key States Test on Serial Monitor
 
 
 Created 15 April 2017
 @Gorontalo, Indonesia
 by ZulNs
 */

#include <MultitapKeypad.h>

const byte ROW0 = 4;
const byte ROW1 = 5;
const byte ROW2 = 6;
const byte ROW3 = 7;
const byte COL0 = 8;
const byte COL1 = 9;
const byte COL2 = 10;
const byte COL3 = 11;
const byte BEEP = 2;

// creates kpd as MultitapKeypad object
// for matrix 4 x 3 keypad
// MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2 );
// for matrix 4 x 4 keypad
MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2, COL3 );
// creates key as Key object
Key key;

void setup() {
  Serial.begin(9600);
  Serial.println( F( "Key States Test on Serial Monitor" ) );
  Serial.println( F( "by ZulNs" ) );
  Serial.println();
  Serial.println( F( "Press * key to begin..." ) );
  // wait until [*] key has been pressed
  do {
    key = kpd.getKey();
  } while ( key.beforeLastCode != NO_KEY || key.lastCode != KEY_ASTERISK || key.code != NO_KEY );
  tone( BEEP, 4000, 50 );
  Serial.println();
  Serial.println( F( "Now you can test any key states..." ) );
  Serial.println();
}

void loop() {
  key = kpd.getKey();
  Serial.print( F( "Key " ) );
  if ( key.character > 0 )
    Serial.print( char( key.character ) );
  else
    Serial.print( F( "??" ) );
  switch ( key.state ) {
  case KEY_DOWN:
  case MULTI_TAP:
    tone( BEEP, 5000, 20 );
    Serial.println( F( " down" ) );
    if ( key.state == MULTI_TAP ) {
      Serial.print( F( "TapCounter: " ) );
      if ( key.tapCounter < 10 )
        Serial.println( key.tapCounter, DEC );
      else {
        kpd.resetTapCounter();
        Serial.println( 0 );
      }
    }
    break;
  case LONG_TAP:
    tone( BEEP, 5000, 20 );
    Serial.println( F( " hold" ) );
    break;
  case MULTI_KEY_DOWN:
    tone( BEEP, 4000, 100 );
    Serial.println( F( " down" ) );
    break;
  case KEY_UP:
    Serial.println( F( " up" ) );
  }
}
