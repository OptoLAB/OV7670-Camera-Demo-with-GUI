/*
 * 	OV7670.h
 *
 *  Created on: 2022
 *  Author: J. Bajic
 *
 */

#ifndef OV7670_H
#define OV7670_H

#include "stm32f7xx_hal.h"

#define OV7670_RST_PORT		GPIOD
#define OV7670_RST_PIN		GPIO_PIN_12
#define OV7670_PWDN_PORT	GPIOD
#define OV7670_PWDN_PIN		GPIO_PIN_14
#define OV7670_ADDR 		(0x42)
#define OV7670_TIMEOUT 		1000

//-------------------------------------------------
#define F_XCLK 		8 		// 8MHz set by LPTIM1
#define PLL_VAL		4		// x4 	set using SetFrameRate
#define CLKRC_VAL 	0 		// 		set using SetFrameRate
#define F_INT 		(F_XCLK*PLL_VAL)/(CLKRC_VAL+1)
#define DUMMY_PX	0
#define T_ROW		(2*(784+DUMMY_PX)/F_INT)
#define BF_50Hz_VALUE (1000000L/(T_ROW*100)) //50Hz banding filter value
#define BF_60Hz_VALUE (1000000L/(T_ROW*120)) //60Hz banding filter
//-------------------------------------------------

#define BYPASS_PLL 	0x00
#define PLL_x4		0x40
#define PLL_x6		0x80
#define PLL_x8 		0xC0
#define XCLK_DIV(x)	(x-1)

typedef enum {CONTINUOUS=0, SNAPSHOT} Capture_mode;

typedef enum {OFF=0, ON} Camera_state;
typedef enum {YUV422=0, RGB565} Camera_format;
typedef enum {F_50Hz=0, F_60Hz, F_AUTO} Camera_filter;
typedef enum {AVERAGE=0, HISTOGRAM} Camera_algorithm;
typedef enum {x2=0,x4,x8,x16,x32,x64,x128} Camera_gain;
typedef enum {NORMAL_FPS=0, HALF_FPS, QUARTER_FPS, EIGHTH_FPS} Camera_fps;
typedef enum {VGA=0, QVGA, QQVGA, QQQVGA, CIF, QCIF, QQCIF} Camera_resolution;
typedef enum {NORMAL=0, ANTIQUE, BW, NEGATIVE, BW_NEGATIVE, BLUISH, GREENISH, REDDISH} Camera_effect;

typedef struct{
	Camera_resolution resolution; 	// 0-VGA, 1-QVGA, 2-QQVGA, 3-CIF, 4-QCIF, 5-QQCIF
	Camera_format format;			// 0-YUV422, 1-RGB565
	Camera_effect effect;			// 0-NORMAL, 1-ANTIQUE, 2-BW, 3-NEGATIVE, 4-BW_NEGATIVE, 5-BLUISH, 6-GREENISH, 7-REDISH
	Camera_state AEC;				// Auto exposure mode, 1-Enabled, 0-Disabled
	Camera_state AGC;				// Auto gain mode, 1-Enabled, 0-Disabled
	Camera_state AWB; 				// Auto white balance mode, 1-Enabled, 0-Disabled
	Camera_state cbar;				// Display color bar, 1-Enabled, 0-Disabled
	Camera_state v_flip;			// Flip image horizontal, 1-flip, 0-normal
	Camera_state h_flip;			// Flip image vertical, 1-flip, 0-normal
	Camera_state night_mode;		// Enable night mode, 1-Enable, 0-Disable
	Camera_state ASC;				// Auto sharpness control, 1-Enable, 0-Disable
	Camera_state ADN;				// Auto de-noise control, 1-Enable, 0-Disable
	Camera_state BDF;				// Banding filter, 1-Enable, 0-Disable
	Camera_algorithm algorithm;		// AEC algorithm, 0-Average-based, 1-Histogram-based
	uint8_t night_mode_fps;			// Minimum frame rate of night_mode 0-NORMAL_FPS, 1-HALF_FPS, 2-QUARTER_FPS, 3-EIGHTH_FPS
	uint8_t bd_filter_value;		// Banding filter value 0-F_50Hz=0, 1-F_60Hz, 2-F_AUTO
	uint16_t exposure;      		// Exposure (AEC=0), range=[0,65535]
	uint8_t gain;					// Gain (AGC=0), 0-1x, 1-2x, 2-4x, 3-8x, 4-16x, 5-32x, 6-64x, 7-128x
	uint8_t brightness;				// Brightness, range=[0,255]
	uint8_t	contrast;				// Contrast,  range=[0,255]
	uint8_t saturation;				// Saturation, range=[0,100]
	uint8_t sharpness;				// Sharpness, range=[0,31]
	uint8_t strength;				// De-noise strength, range=[0,255]
	Camera_gain gain_ceiling;		// Gain ceiling, 0-2x, 1-4x, 2-8x, 3-16x, 4-32x, 5-64x, 6-128x
	uint8_t rch_gain;				// Red Channel Gain (AWB=0), range=[0,255]
	uint8_t gch_gain;				// Green Channel Gain (AWB=0), range=[0,255]
	uint8_t bch_gain;				// Blue Channel Gain (AWB=0), range=[0,255]
}Camera_settings;

void OV7670_Init(DCMI_HandleTypeDef *p_hdcmi, DMA_HandleTypeDef *p_hdma_dcmi, I2C_HandleTypeDef *p_hi2c, LPTIM_HandleTypeDef *p_hlptim);

void OV7670_Power(uint8_t en);
void OV7670_ResetHW(void);
void OV7670_XCLK(uint8_t en);

void OV7670_WriteSCCB(uint8_t regAddr, uint8_t val);
void OV7670_ReadSCCB(uint8_t regAddr, uint8_t *data);

void OV7670_Start(Capture_mode mode, uint32_t *capture_address);
void OV7670_Stop();

void OV7670_ResetSW(void);
void OV7670_Config(const uint8_t params[][2]);

void OV7670_SetBrightness(uint8_t brightness);
void OV7670_SetContrast(uint8_t contrast);
void OV7670_SetSaturation(uint8_t saturation);
void OV7670_SetResolution(Camera_resolution resolution);
void OV7670_SetColorFormat(Camera_format format);
void OV7670_AutoExposureMode(Camera_state en);
void OV7670_SetAECAlgorithm(Camera_algorithm algorithm);
void OV7670_SetExposureTime(uint16_t time);
void OV7670_SetGainCeiling(Camera_gain value);
void OV7670_AutoGainMode(Camera_state en);
void OV7670_SetGain(uint8_t gain);
void OV7670_AutoWhiteBalanceMode(Camera_state en);
void OV7670_SetChannelsGain(uint8_t r_ch, uint8_t g_ch, uint8_t b_ch);
void OV7670_ShowColorBar(Camera_state en);
void OV7670_FlipHorizontal(Camera_state flipH);
void OV7670_FlipVertical(Camera_state flipV);
void OV7670_NightMode(Camera_state en,	Camera_fps min_fps);
void OV7670_AutoSharpness(Camera_state en);
void OV7670_SetSharpness(uint8_t sharpness);
void OV7670_AutoDeNoise(Camera_state en);
void OV7670_SetDeNoiseStrength(uint8_t strength);
void OV7670_SpecialEffect(Camera_effect effect);

void OV7670_PowerDown(void);
void OV7670_PowerUp(void);
void OV7670_UpdateSettings(Camera_settings OV7670);

void OV7670_SetFrameRate(uint8_t div, uint8_t mul);
void OV7670_SetFrameControl(uint16_t hstart, uint16_t hstop, uint16_t vstart, uint16_t vstop);

void OV7670_getImageInfo(uint16_t *width, uint16_t *height, uint8_t *format);

void calculateExposureTimeMS(uint16_t aec_reg_val,uint16_t *time_num, uint8_t *time_fract);

#endif /* OV7670_H*/
