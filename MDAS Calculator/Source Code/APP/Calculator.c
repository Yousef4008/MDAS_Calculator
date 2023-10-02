/*
 * Calculator.c
 *
 *  Created on: Oct 2, 2023
 *      Author: Youssif Hossam
 */

#include "calculator.h"

#include "../HAL/LCD/lcd.h"
#include "../HAL/KEYPAD/keypad.h"
#include <util/delay.h>

void CALCULATOR_init(){
	uint8 KEYPAD_PressedKey ;		/* Variable to receive the ASCII of pressed button */

	uint8 LCD_Cursor_Counter=0 ;

	uint8 counter , counter2 ;					/* General purpose Counters */

	uint8 result_flag = 0 ;			/* flag to determine if the user entered = or not*/

	/* Counter to use it in the operands array */
	uint8 CALC_OperandsCounter = 0 ;

	/* Counter to use it in the operators array */
	uint8 CALC_ArithOpCounter = 0;

	/* Arrays of custom patterns */
	uint8 LCD_BlankPattern[]= BLANK_PATTERN_5X7;
	uint8 LCD_SmileEmoji[] = SMILE_EMOJI_PATTERN_5X7;

	/* Calculator operands array with zero initial values */
	uint16 CALC_OperandsArr[CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS+1] = {0} ;

	/* Calculator operators array with + initial values */
	uint8 CALC_ArithOperatorssArr[CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS] = {'+','+','+','+','+','+','+'};

	uint16 result = 0;		/* Variable to save the result value */


	LCD_init();

	/* Storing the custom patterns on the CGRAM */
	LCD_StoreCustomPtrn(LCD_BlankPattern , BLANK_PATTERN_CGRAM_INDEX);
	LCD_StoreCustomPtrn(LCD_SmileEmoji , SMILE_FACE_PATTERN_CGRAM_INDDEX);

	LCD_displayStringRowColumn(0,3,"Calculator");
	LCD_displayStringRowColumn(1,4,"Project");


	LCD_DisplayCustomPtrn(SMILE_FACE_PATTERN_CGRAM_INDDEX , 0 ,0);
	LCD_DisplayCustomPtrn(SMILE_FACE_PATTERN_CGRAM_INDDEX , 0 ,15);
	_delay_ms(1000);

	LCD_clearScreen();

	LCD_displayStringRowColumn(0,4,"Loading..");

	for(counter = 0 ; counter < 8 ; counter++){
		LCD_DisplayCustomPtrn(BLANK_PATTERN_CGRAM_INDEX,1,counter);
		_delay_ms(50);
	}
	for(; counter < 16 ; counter++){
		LCD_DisplayCustomPtrn(BLANK_PATTERN_CGRAM_INDEX,1,counter);
		_delay_ms(150);
	}
	_delay_ms(300);

	LCD_clearScreen();

	while(1)
	{
		/* Get the pressed key number, if any switch pressed for more than 250 ms it will considered more than one press */
		KEYPAD_PressedKey = KEYPAD_getPressedKey();
		_delay_ms(250);


		if( LCD_Cursor_Counter < 16 ) {		/* If the number doesn't exceed the first row */

			if(!result_flag){	/* if the user still haven't pressed = */

				LCD_displayCharacter(KEYPAD_PressedKey);

				LCD_Cursor_Counter++;

				switch(KEYPAD_PressedKey){

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':

					/* Shifting the number to left and put the new digit on zero digit */
					CALC_OperandsArr[CALC_OperandsCounter] =				\
					CALC_OperandsArr[CALC_OperandsCounter]*10				\
					+ ASCII_TO_INT(KEYPAD_PressedKey);
					break;

				case '+':
				case '-':
				case 'x':
				case '/':
					/* Storing the arithmetic operator on the array and increment both counters */
					CALC_ArithOperatorssArr[CALC_ArithOpCounter] = KEYPAD_PressedKey;
					CALC_ArithOpCounter++;
					CALC_OperandsCounter++;
					break;

				case '=':
					result_flag = 1;

					/*Calculating the multiplication and shifting all operands after it to the left */
					for(counter = 0 ; counter < CALC_ArithOpCounter ; counter++){
						if(CALC_ArithOperatorssArr[counter] == 'x'){

							CALC_OperandsArr[counter] *= CALC_OperandsArr[counter+1];

							for(counter2 = counter ; counter2 < CALC_ArithOpCounter-1 ; counter2++){
								CALC_ArithOperatorssArr[counter2] = CALC_ArithOperatorssArr[counter2 + 1];
								CALC_OperandsArr[counter2 + 1] = CALC_OperandsArr[counter2 + 2];
							}

							CALC_ArithOpCounter--;
							counter--;
						}
						/*Calculating the division and shifting all operands after it to the left */
						else if(CALC_ArithOperatorssArr[counter] == '/'){

							CALC_OperandsArr[counter] = CALC_OperandsArr[counter] / CALC_OperandsArr[counter+1];

							for(counter2 = counter ; counter2 < CALC_ArithOpCounter-1 ; counter2++){
								CALC_ArithOperatorssArr[counter2] = CALC_ArithOperatorssArr[counter2 + 1];
								CALC_OperandsArr[counter2 + 1] = CALC_OperandsArr[counter2 + 2];
							}

							CALC_ArithOpCounter--;
							counter--;

						}
					}
					result = CALC_OperandsArr[0];	/* 1st operand is initially the result */

					/* completing the calculations of the result */
					for(counter = 0 ; counter < CALC_ArithOpCounter ; counter++){

						if(CALC_ArithOperatorssArr[counter] == '+')
							CALC_OperandsArr[0] += CALC_OperandsArr[counter+1];

						else
							CALC_OperandsArr[0] -= CALC_OperandsArr[counter+1];
					}

					result = CALC_OperandsArr[0];

					/* Displaying the result */
					LCD_displayStringRowColumn(1,0,"RESULT =");
					LCD_moveCursor(1,9);
					LCD_intgerToString(result);
					break;

				case 'N':
					/* Resting the calculator when the user press ON/C */
					LCD_Cursor_Counter = 0;
					LCD_clearScreen();
					CALC_ArithOpCounter = 0;
					CALC_OperandsCounter = 0;
					result_flag = 0;
					for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS ; counter++){

						CALC_ArithOperatorssArr[counter] = '+';
					}
					for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS+1 ; counter++){
						CALC_OperandsArr[counter] = 0;
					}
					break;

				}

			}

			else{		/* If the user have pressed = */

				switch(KEYPAD_PressedKey){

				case '+':
				case '-':
				case 'x':
				case '/':

					/* This step handling when the user pressed = and the result have been showed up ,
					 * then the the user need to make another calculations to the result
					 */

					/* Increasing the cursor counter based on the size of the result */
					LCD_Cursor_Counter = countDigit(result);

					LCD_clearScreen();

					/* Storing the result on the first element of the operands array */


					/* Displaying the result on the start of the LCD */
					LCD_intgerToString(CALC_OperandsArr[0]);

					LCD_displayCharacter(KEYPAD_PressedKey);

					result_flag = 0;

					/* Reseting the calculator */
					for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS ; counter++){
						CALC_ArithOperatorssArr[counter] = '+';
					}
					for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS+1 ; counter++){
						CALC_OperandsArr[counter] = 0;
					}

					CALC_OperandsArr[0] = result;

					CALC_ArithOperatorssArr[0] = KEYPAD_PressedKey;

					CALC_OperandsCounter = 1;
					CALC_ArithOpCounter = 1;

					break;

				case 'N':

					/* Resting the calculator when the user press ON/C */
					LCD_Cursor_Counter = 0;
					LCD_clearScreen();
					CALC_ArithOpCounter = 0;
					CALC_OperandsCounter = 0;
					result_flag = 0;
					for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS ; counter++){

						CALC_ArithOperatorssArr[counter] = '+';
					}
					for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS+1 ; counter++){
						CALC_OperandsArr[counter] = 0;
					}
					break;
				}
			}
		}

		else{	/* If the numbers exceeded the first row */

			/* Reseting the calculator and show up a error message */
			LCD_Cursor_Counter = 0;
			LCD_clearScreen();
			LCD_displayString("Error:Many Num.");
			_delay_ms(1000);
			LCD_clearScreen();
			CALC_ArithOpCounter = 0;
			CALC_OperandsCounter = 0;
			result_flag = 0;
			for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS ; counter++){

				CALC_ArithOperatorssArr[counter] = '+';
			}
			for(counter = 0 ; counter < CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS+1 ; counter++){
				CALC_OperandsArr[counter] = 0;
			}
		}

	}
}
int countDigit(uint16 num){	/* Simple function to count the digit of a number */
	if (num == 0)
		return 1;
	int count = 0;
	while (num != 0) {
		num = num / 10;
		++count;
	}
	return count;
}
