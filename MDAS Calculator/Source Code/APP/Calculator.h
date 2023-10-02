/*
 * Calculator.h
 *
 *  Created on: Oct 2, 2023
 *      Author: Youssif Hossam
 */

#ifndef APP_CALCULATOR_H_
#define APP_CALCULATOR_H_

#include "../LIB/std_types.h"

/* Index of some custom pattern in the CRRAM */
#define BLANK_PATTERN_CGRAM_INDEX 					0
#define SMILE_FACE_PATTERN_CGRAM_INDDEX				1

/* Calculator limitation */
#define CALCULATOR_MAX_NO_ARITHMATIC_OPERATORS		7

/* My custom patterns */
#define BLANK_PATTERN_5X7			{0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}
#define SMILE_EMOJI_PATTERN_5X7		{0x00, 0x0A, 0x00, 0x1F, 0x11, 0x0E, 0x00}

/* macro to convert ASCII number to its integer value */
#define ASCII_TO_INT(ASCII)	(uint32)(ASCII-48)

void CALCULATOR_init();
int countDigit(uint16 num);

#endif /* APP_CALCULATOR_H_ */
