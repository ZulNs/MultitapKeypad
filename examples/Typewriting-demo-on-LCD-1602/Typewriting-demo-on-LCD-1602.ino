/*
 Typewriting Demo on LCD 1602
 
 
 Created 17 April 2017
 @Gorontalo, Indonesia
 by ZulNs
 */

#include <MultitapKeypad.h>
#include <LiquidCrystal.h>

const uint16_t BACKLIGHT_PERIODS = 10000;
const uint16_t MODE_FLASHING_PERIODS = 1500;

const byte ROW0 = 4;
const byte ROW1 = 5;
const byte ROW2 = 6;
const byte ROW3 = 7;
const byte COL0 = 8;
const byte COL1 = 9;
const byte COL2 = 10;
const byte COL3 = 11;

const byte LCD_RS = A4;
const byte LCD_EN = A5;
const byte LCD_D4 = A0;
const byte LCD_D5 = A1;
const byte LCD_D6 = A2;
const byte LCD_D7 = A3;
const byte BACKLIGHT = 3;
const byte BEEP = 2;

const byte UPPER_A = 0;
const byte UPPER_B = 1;
const byte LOWER_A = 2;
const byte LOWER_B = 3;
const byte NUMBER_1 = 4;
const byte NUMBER_2 = 5;
const byte CR = 6;

const byte CHR_BOUND = 3;
const byte BACKSPACE = 8;
const byte CLEARSCREEN = 12;
const byte CARRIAGE_RETURN = 13;
const uint8_t CAPSLOCK_ON = 17;
const uint8_t CAPSLOCK_OFF = 18;
const uint8_t NUMLOCK_ON = 19;
const uint8_t NUMLOCK_OFF = 20;

const char SYMBOL[] PROGMEM = {
  ',', '.', ';', ':', '!',
  '?', '\'', '"','-', '+',
  '*', '/', '=', '%', '^',
  '(', ')', '#', '@', '$',
  '[', ']', '{', '}', CHR_BOUND
};
const char ALPHABET[] PROGMEM = {
  'A', 'B', 'C', CHR_BOUND, CHR_BOUND,
  'D', 'E', 'F', CHR_BOUND, CHR_BOUND,
  'G', 'H', 'I', CHR_BOUND, CHR_BOUND,
  'J', 'K', 'L', CHR_BOUND, CHR_BOUND,
  'M', 'N', 'O', CHR_BOUND, CHR_BOUND,
  'P', 'Q', 'R', 'S',       CHR_BOUND,
  'T', 'U', 'V', CHR_BOUND, CHR_BOUND,
  'W', 'X', 'Y', 'Z',       CHR_BOUND
};
byte letter_A[8] = {
  0b11111,
  0b11001,
  0b10110,
  0b10110,
  0b10000,
  0b10110,
  0b10110,
  0b11111
};
byte letter_B[8] = {
  0b11111,
  0b10001,
  0b10110,
  0b10001,
  0b10110,
  0b10110,
  0b10001,
  0b11111
};
byte letter_a[8] = {
  0b11111,
  0b11111,
  0b11000,
  0b11110,
  0b11000,
  0b10110,
  0b11000,
  0b11111
};
byte letter_b[8] = {
  0b11111,
  0b10111,
  0b10111,
  0b10001,
  0b10110,
  0b10110,
  0b10001,
  0b11111
};
byte number_1[8] = {
  0b11111,
  0b11011,
  0b10011,
  0b11011,
  0b11011,
  0b11011,
  0b10001,
  0b11111
};
byte number_2[8] = {
  0b11111,
  0b11001,
  0b10110,
  0b11110,
  0b11101,
  0b11011,
  0b10000,
  0b11111
};
byte carriage_return[8] = {
  0b00000,
  0b00000,
  0b00101,
  0b01001,
  0b11111,
  0b01000,
  0b00100,
  0b00000
};

char modeBuf[] = "  ";
byte cursorPos = 0;
boolean alphaMode = true;
boolean upperCaseMode = true;
boolean isEndOfDisplay = false;
boolean autoOffBacklight = false;
boolean isModeFlashing = false;
unsigned long backlightTimeout;
unsigned long modeFlashingTimeout;

// creates lcd as LiquidCrystal object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// creates kpd as MultitapKeypad object
// for matrix 4 x 3 keypad
// MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2 );
// for matrix 4 x 4 keypad
MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2, COL3 );
// creates key as Key object
Key key;

void setup() {
  kpd.attachFunction( otherProcess );
  pinMode( BACKLIGHT, OUTPUT );
  digitalWrite( BACKLIGHT, LOW );
  pinMode( LED_BUILTIN, OUTPUT );
  // create a new character
  lcd.createChar( UPPER_A, letter_A );
  lcd.createChar( UPPER_B, letter_B );
  lcd.createChar( LOWER_A, letter_a );
  lcd.createChar( LOWER_B, letter_b );
  lcd.createChar( NUMBER_1, number_1 );
  lcd.createChar( NUMBER_2, number_2 );
  lcd.createChar( CR, carriage_return );
  lcd.begin( 16, 2 );
  lcd.print( F( "Multitap Keypad" ) );
  lcd.setCursor( 0, 1 );
  lcd.print( F( "by ZulNs" ) );
  delay( 5000 );
  lcd.clear();
  lcd.print( F( "Press [*] key..." ) );
  // wait until [*] key has been pressed
  do {
    key = kpd.getKey();
  }
  while ( key.beforeLastCode != NO_KEY || key.lastCode != KEY_ASTERISK || key.code != NO_KEY );
  tone( BEEP, 4000, 50 );
  lcd.clear();
  lcd.cursor();
  lcd.blink();
}

void loop() {
  char chr;
  key = kpd.getKey();
  switch ( key.state ) {
  case KEY_DOWN:
  case MULTI_TAP:
    tone( BEEP, 5000, 20 );
    digitalWrite( LED_BUILTIN, HIGH );
    switch ( key.code ) {
    case KEY_ASTERISK:
      chr = BACKSPACE;
      break;
    case KEY_NUMBER_SIGN:
      if ( alphaMode ) {
        upperCaseMode = !upperCaseMode;
        chr = upperCaseMode ? CAPSLOCK_ON : CAPSLOCK_OFF;
      }
      else
        chr = CARRIAGE_RETURN;
      break;
    case KEY_0:
      chr = alphaMode ? ' ' : '0';
      break;
    case KEY_1:
      chr = alphaMode ? getSymbol( key.tapCounter ) : '1';
      break;
    case KEY_A:
    case KEY_B:
    case KEY_C:
    case KEY_D:
      chr = key.character;
      break;
    default:
      chr = alphaMode ? getAlphabet( key.character, key.tapCounter ) : key.character;
    }
    if ( !upperCaseMode && chr >= 'A' )
      chr += 32; // makes lower case
    if ( key.state == MULTI_TAP && alphaMode && key.character >= '1' && key.character <= '9' )
      printToLcd( BACKSPACE );
    printToLcd( chr );
    autoOffBacklight = false;
    digitalWrite(BACKLIGHT, LOW);
    break;
  case LONG_TAP:
    switch ( key.code ) {
    case KEY_ASTERISK:
      chr = CLEARSCREEN;
      break;
    case KEY_NUMBER_SIGN:
      if ( alphaMode )
        upperCaseMode = !upperCaseMode;
      alphaMode = !alphaMode;
      chr = alphaMode ? NUMLOCK_OFF : NUMLOCK_ON;
      break;
    default:
      chr = key.character;
    }
    if ( chr < ' ' || alphaMode && chr >= '0' && chr <= '9' ) {
      tone( BEEP, 5000, 20 );
      if ( chr >= '0' || chr == NUMLOCK_OFF )
        printToLcd( BACKSPACE );
      printToLcd( chr );
    }
    break;
  case MULTI_KEY_DOWN:
    tone( BEEP, 4000, 100 );
    digitalWrite( LED_BUILTIN, LOW );
    break;
  case KEY_UP:
    digitalWrite( LED_BUILTIN, LOW );
    backlightTimeout = millis() + BACKLIGHT_PERIODS;
    autoOffBacklight = true;
  }
}

void otherProcess( void ) {
  if ( autoOffBacklight && millis() >= backlightTimeout ) {
    digitalWrite( BACKLIGHT, HIGH );
    autoOffBacklight = false;
  }
  if ( isModeFlashing && millis() >= modeFlashingTimeout ) {
    lcd.setCursor( 14, 0 );
    lcd.print( modeBuf );
    setCursorPos();
    lcd.cursor();
    lcd.blink();
    isModeFlashing = false;
  }
}

void printToLcd( char chr ) {
  switch ( chr ) {
    case BACKSPACE:
      if ( !isEndOfDisplay )
        decCursorPos();
      setCursorPos();
      lcd.print( F( " " ) );
      setCursorPos();
      isEndOfDisplay = false;
      if ( cursorPos >= 14 && cursorPos <= 15 )
        modeBuf[ cursorPos - 14 ] = ' ';
      break;
    case CLEARSCREEN:
      lcd.clear();
      cursorPos = 0;
      modeBuf[ 0 ] = ' ';
      modeBuf[ 1 ] = ' ';
      break;
    case CARRIAGE_RETURN:
      lcd.write( byte( CR ) );
      incCursorPos();
      setCursorPos();
      break;
    case CAPSLOCK_ON:
    case CAPSLOCK_OFF:
    case NUMLOCK_ON:
    case NUMLOCK_OFF:
      lcd.setCursor( 14, 0 );
      lcd.noCursor();
      lcd.noBlink();
      if ( chr == NUMLOCK_ON ) {
        lcd.write( byte( NUMBER_1 ) );
        lcd.write( byte( NUMBER_2 ) );
      }
      else {
        if ( upperCaseMode ) {
          lcd.write( byte( UPPER_A ) );
          lcd.write( byte( UPPER_B ) );
        }
        else {
          lcd.write( byte( LOWER_A ) );
          lcd.write( byte( LOWER_B ) );
        }
      }
      setCursorPos();
      modeFlashingTimeout = millis() + MODE_FLASHING_PERIODS;
      isModeFlashing = true;
      break;
    default:
      if ( cursorPos == 31 )
        isEndOfDisplay = true;
      lcd.print( chr );
      if ( cursorPos >= 14 && cursorPos <= 15 )
        modeBuf[ cursorPos - 14 ] = chr;
      incCursorPos();
      setCursorPos();
  }
}

void incCursorPos( void ) {
  if (cursorPos < 31) cursorPos++;
}

void decCursorPos( void ) {
  if ( cursorPos > 0 ) cursorPos--;
}

void setCursorPos( void ) {
  lcd.setCursor( cursorPos % 16, cursorPos / 16 );
}

byte getSymbol( byte ctr ) {
  byte sym = pgm_read_byte_near( SYMBOL + ctr );
  if ( sym == CHR_BOUND ) {
    sym = pgm_read_byte_near( SYMBOL );
    kpd.resetTapCounter();
  }
  return sym;
}

byte getAlphabet( byte chr, byte ctr ) {
  chr = ( chr - '2' ) * 5;
  byte alpha = pgm_read_byte_near( ALPHABET + chr + ctr );
  if ( alpha == CHR_BOUND ) {
    alpha = pgm_read_byte_near( ALPHABET + chr );
    kpd.resetTapCounter();
  }
  return alpha;
}

