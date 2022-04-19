/*
 * color_conversion.h
 *
 *  Created on: Apr 13, 2022
 *      Author: J.Bajic
 */

#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

#include "stm32f7xx_hal.h"

uint32_t YUVtoRGB888(int16_t Y, int16_t U, int16_t V);
uint16_t RGB888toRGB565(uint32_t RGB);
uint16_t YUVtoRGB565(int16_t Y, int16_t U, int16_t V);
uint32_t RGB565toRGB888(uint16_t RGB);

#endif /* COLOR_CONVERSION_H */
