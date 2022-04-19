/*
 * color_conversion.c
 *
 *  Created on: Apr 13, 2022
 *      Author: J.Bajic
 */

#include "color_conversion.h"

#define LIMIT(x) (x > 255 ? 255 : (x < 0 ? 0 : x))

uint32_t YUVtoRGB888(int16_t Y, int16_t U, int16_t V)
{
	uint8_t R=0, G=0, B=0;
	R = LIMIT(Y + V + ((V * 103) >> 8));					//RED
	G = LIMIT(Y - ((U * 88) >> 8) - ((V * 183) >> 8));		//GREEN
	B = LIMIT(Y + U + ((U * 198) >> 8));					//BLUE

	return (uint32_t) ((R<<16) | (G<<8) | B);
}

uint16_t RGB888toRGB565(uint32_t RGB)
{
	uint8_t R=0, G=0, B=0;

	R = ((RGB >> 19) & 0x1f);
	G = ((RGB >> 10) & 0x3f);
	B = (RGB >> 3) & 0x1f;

	return (uint16_t) ((R<<11) | (G<<5) | B);
}

uint16_t YUVtoRGB565(int16_t Y, int16_t U, int16_t V)
{
	uint32_t RGB;

	RGB=YUVtoRGB888(Y,U,V);		// -> RGB888
	RGB=RGB888toRGB565(RGB);	// -> RGB565

    return (uint16_t)RGB;
}

uint32_t RGB565toRGB888(uint16_t RGB)
{
	uint8_t R=0, G=0, B=0;

	R = ((RGB >> 11) << 3);
	G = ((RGB >> 5) << 2);
	B = ((RGB & 0x1F)<<3);

	return (uint32_t) ((R<<16) | (G<<8) | B);
}
