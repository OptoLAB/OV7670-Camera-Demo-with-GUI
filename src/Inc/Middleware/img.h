/*
 * img.h
 *
 *  Created on: Apr 19, 2022
 *      Author: J.Bajic
 */

#ifndef IMG_H
#define IMG_H

#include "stm32f7xx_hal.h"
#include "jpeglib.h"

#define X_START	12
#define Y_START	32
#define X_MAX	352 //CIF width
#define Y_MAX	288	//CIF height

typedef struct
{
	uint32_t 	*address;	// address/pointer to image data
	uint16_t 	width;		// image width
	uint16_t 	height;		// image height
	uint8_t 	format;		// format in which image data are stored
}Image_info;

uint32_t* getImageAddress(void);
void clearImage(void);
void drawImage(Image_info image, uint8_t px_size);
void writeJPG(JFILE *file, int quality, Image_info img);
void readJPG(JFILE *file, Image_info *img);

#endif /* IMG_H */
