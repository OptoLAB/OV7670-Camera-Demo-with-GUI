/*
 * img.c
 *
 *  Created on: Apr 19, 2022
 *      Author: J.Bajic
 */

#include "img.h"
#include "SSD1963.h"
#include "color_conversion.h"

#define X_START	12
#define Y_START	32
#define X_MAX	352 //CIF width
#define Y_MAX	288	//CIF height

static uint32_t image_data[X_MAX*(Y_MAX>>1)];	//Max resolution is CIF(352*288)
static uint8_t image_line[X_MAX*3];

uint32_t* getImageAddress(void)
{
	return image_data;
}

void clearImage(void)
{
	for(int i=0; i<2;i++)
		SSD1963_FillArea(X_START, X_START+X_MAX, Y_START+i*(Y_MAX>>1) ,Y_START+(i+1)*(Y_MAX>>1), RGB888toRGB565(0x00FFFFFF));
}

void drawImage(Image_info image, uint8_t px_size)
{
	uint16_t i,j,k,ph,pw;
	uint32_t temp=0;
	int16_t Y1=0,Y2=0,U=0,V=0;

	for(k=0;k<2;k++) //interleved drawing - image looks more fluent
	{
		for(i=k;i<image.height;i++)
		{
			//Write directly to display (SSD1963), don't use GUI - FASTEST!!!
			SSD1963_SetArea(X_START, X_START+(image.width*px_size)-1, Y_START+i*px_size, Y_START+(i+1)*px_size);
			SSD1963_WriteCommand(SSD1963_WRITE_MEMORY_START);

			for(ph=0;ph<px_size;ph++)
			{
				for(j = 0; j < (((image.width)/2)); j++)
				{
					temp=image.address[j+i*(image.width/2)];  // temp contains 2 pixel values

					if(!image.format)	//YUV422
					{
						//Extract YUV
						  Y2=(temp>>24)&0x00FF;
						  U=((temp>>16)&0x00FF)-128;
						  Y1=(temp>>8)&0x00FF;
						  V=(temp&0x00FF)-128;
						  for(pw=0;pw<px_size;pw++)SSD1963_WriteData(YUVtoRGB565(Y1,U,V));
						  for(pw=0;pw<px_size;pw++)SSD1963_WriteData(YUVtoRGB565(Y2,U,V));
					}
					else				//RGB565
					{
						for(pw=0;pw<px_size;pw++)SSD1963_WriteData(temp&0xFFFF);
						for(pw=0;pw<px_size;pw++)SSD1963_WriteData((temp>>16)&0xFFFF);
					}
				}
			}
			i++; //skip 1 line - interleved drawing
		}
	}
}

static void getImageLine(uint32_t *image, uint16_t width, uint16_t line, uint8_t format)
{
	uint32_t temp=0, RGB1, RGB2;
	int16_t Y1=0,Y2=0,U=0,V=0;

	for(uint16_t x=0; x<(width/2); x++)
	{
		temp=image[x+line*(width/2)]; // d contains 2 pixel values

		if(!format)	//YUV422
		{
			Y2=(temp>>24)&0x00FF;
			U=((temp>>16)&0x00FF)-128;
			Y1=(temp>>8)&0x00FF;
			V=(temp&0x00FF)-128;

			RGB1=YUVtoRGB888(Y1,U,V);
			RGB2=YUVtoRGB888(Y2,U,V);
		}
		else 		//RGB565
		{
			RGB1=RGB565toRGB888(temp&0xFFFF);
			RGB2=RGB565toRGB888((temp>>16)&0xFFFF);
		}
		image_line[x*6+0]=RGB1>>16;
		image_line[x*6+1]=RGB1>>8;
		image_line[x*6+2]=RGB1;
		image_line[x*6+3]=RGB2>>16;
		image_line[x*6+4]=RGB2>>8;
		image_line[x*6+5]=RGB2;
	}
}

static void setImageLine(uint32_t *image, uint16_t width, uint16_t line)
{
	uint32_t RGB1, RGB2;

	for(uint16_t x=0; x<(width/2); x++)
	{
		RGB1=(uint32_t)((image_line[x*6+0]<<16)|(image_line[x*6+1]<<8)|image_line[x*6+2]);
		RGB2=(uint32_t)((image_line[x*6+3]<<16)|(image_line[x*6+4]<<8)|image_line[x*6+5]);

		image[x+line*(width/2)]=(RGB888toRGB565(RGB2)<<16)|RGB888toRGB565(RGB1);
	}
}

void writeJPG(JFILE *file, int quality, Image_info img)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	row_pointer[0]=image_line;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, file);

	cinfo.image_width = img.width;
	cinfo.image_height = img.height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	for(uint16_t i=0; i<img.height; i++)
	{
		getImageLine(img.address, img.width, i, img.format);
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
}

void readJPG(JFILE *file, Image_info *img)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];

	row_pointer[0]=image_line;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, file);

	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	img->width=cinfo.output_width;
	img->height=cinfo.output_height;
	img->format=1; //only RGB565 supported

	for(uint16_t i=0; i<cinfo.output_height; i++)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		setImageLine(img->address,cinfo.output_width, i);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

