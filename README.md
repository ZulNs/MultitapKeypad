# MultitapKeypad Arduino Library Documentation

[![Library Manager: MultitapKeypad 1.1.0](https://www.ardu-badge.com/badge/MultitapKeypad.svg)](https://www.ardu-badge.com/MultitapKeypad)

## Overview
This library allows interfacing between an Arduino board with matrix 4 x 3 phone's keypad as well as matrix 4 x 4 keypad. It can indicates the key states such as key down, repeatedly key tap, key press and hold, multiple key down, and key release.

## Public Constructor
For the matrix 4 x 3 phone's keypad which consist of four rows and three columns, defines each row and column as constructor arguments in following order:

**`MultitapKeypad(row_0, row_1, row_2, row_3, col_0, col_1, col_2)`**

Whereas for the matrix 4 x 4 keypad simply defined fourth column (**`col_3`**) as last argument of constructor below:

**`MultitapKeypad(row_0, row_1, row_2, row_3, col_0, col_1, col_2, col_3)`**

The determination of the order of each row and column refer to ***Fig. 1.*** First row as **R0**, second row as **R1**, and so on. Similarly first column as **C0**, second column as **C1**, and so forth.

The arguments **`row_0`**...**`row_3`** and **`col_0`**...**`col_3`** are any valid Arduino pins number which connected to related keypad rows and columns number.

![](matrix-keypad.png?raw=true)

***Figure 1.*** *Matrix Keypad Configuration*

## Public Methods
- **`getKey()`**

  Returns `Key` object which consist of:
  - **`Key.character`**

    It is a `byte` type number which correspond with active key i.e.:
    - `NO_KEY` = `0`
    - `KEY_1` = `'1'`
    - `KEY_2` = `'2'`
    - `KEY_3` = `'3'`
    - `KEY_A` = `'A'`
    - `KEY_4` = `'4'`
    - `KEY_5` = `'5'`
    - `KEY_6` = `'6'`
    - `KEY_B` = `'B'`
    - `KEY_7` = `'7'`
    - `KEY_8` = `'8'`
    - `KEY_9` = `'9'`
    - `KEY_C` = `'C'`
    - `KEY_ASTERISK` = `'*'`
    - `KEY_0` = `'0'`
    - `KEY_NUMBER_SIGN` = `'#'`
    - `KEY_D`** = `'D'`
  - **`Key.state`**

    It is a custom `KEY_STATE` number indicates of key event which consist of:
    - **`KEY_DOWN`**

      Indicates there is a key was pressed.
    - **`MULTI_TAP`**

      Indicates that the active key is identical with previous pressed key for less than one second.
    - **`LONG_TAP`**

      Indicates there is a key was pressed and held for at least one second.
    - **`MULTI_KEY_DOWN`**

      Indicates there are multiple keys were pressed.
    - **`KEY_UP`**

      Indicates there is a key was released.
  - **`Key.tapCounter`**

    It is a `byte` type number count from `0` to `255`, indicates the number of multi-tap key minus one. One indicates second tap, two as third tap, and so forth. Max 256 custom multi-tap characters can be accessed.
  - **`Key.code`**

    It is a `byte` type number as scan code of current active key which represented `NO_KEY`, `KEY_0`...`KEY_9`, `KEY_ASTERISK`, `KEY_NUMBER_SIGN`, `KEY_A`...`KEY_D`. If the number is not one of previously mentioned means `MULTI_KEY_DOWN` event was happened.
  - **`Key.lastCode`**

    It is a `byte` type number as scan code of just before current active key.
  - **`Key.beforeLastCode`**

    It is also a `byte` type number as scan code of just before last active key.
- **`resetTapCounter()`**

  Reset multi-tap counter to zero for user desired multi-tap characters array.
- **`attachFunction(userFunction)`**

  While user calling `getKey()` method the process will returned if the new key event reached. To prevent user from loosing processor resources for other processing, this method offers to attach a user function to callback while waiting key state changed.

  For example:

      // creates keypad as MultitapKeypad object
      MultitapKeypad keypad(4, 5, 6, 7, 8, 9, 10, 11);
      // creates key as Key object
      Key key;
 
      void setup() {
        // attaches a user function to callback while waiting key state changed
        keypad.attachFunction(userFunction);
        // put your setup code here, to run once:
      }

      void loop() {
        // get key object
        key = keypad.getKey();
        // put your main code here, to run repeatedly:
      }

      void userFunction(void) {
        // do other process while waiting between two key events (key state changed)
      }
## Examples
This library provides with following examples:
- ### Key States Test on Serial Monitor
  This example displays every key states on Serial Monitor, also the `tapCounter` value. To run the test, open Serial Monitor, don't forget to set the baud rate at [ ***9600 baud*** ] and set the line ending to [ ***Newline*** ]. Hardware wiring can be followed on ***Fig. 2.***

![](keypad-wiring.png?raw=true)

***Figure 2.*** *Matrix Keypad Wiring*

- ### Key States Test on LCD 1602
  Displays every key states on LCD 1602, also the `tapCounter` value. Hardware wiring can be followed on ***Fig. 2.*** and ***Fig. 3.***

![](lcd1602-wiring.png?raw=true)

***Figure 3.*** *LCD 1602 Wiring*

- ### Typewriting Demo on LCD 1602
  This example demonstrates the typewriting style just like on old Nokia phone. ***Fig. 4.*** and ***Fig. 5.*** illustrate all available characters which can be accessed. Character in the parentheses accessed through long-tap or hold the relevant key for at least one second. Hardware wiring also can be followed on ***Fig. 2.*** and ***Fig. 3.***

  

![](abc-mode.png?raw=true)

***Figure 4.*** *Keypad Style in Alphabet Mode*

![](123-mode.png?raw=true)

***Figure 5.*** *Keypad Style in Numeric Mode*

- ### Input String on LCD 1602
  If the previous example demonstrates the typewriting style just like on old Nokia phone, this example captures the entered string from the keypad into a string buffer and displays them to Serial Monitor. ***Fig. 6.*** and ***Fig. 7.*** illustrate all available characters which can be accessed. Character in the parentheses accessed through long-tap or hold the relevant key for at least one second. Hardware wiring also can be followed on ***Fig. 2.*** and ***Fig. 3.***

  

![](abc-mode-2.png?raw=true)

***Figure 6.*** *Keypad Style in Alphabet Mode*

![](123-mode-2.png?raw=true)

***Figure 7.*** *Keypad Style in Numeric Mode*

&nbsp;

&nbsp;

---
#### Created by ZulNs
##### @Gorontalo, Indonesia, 15 April 2017
---
