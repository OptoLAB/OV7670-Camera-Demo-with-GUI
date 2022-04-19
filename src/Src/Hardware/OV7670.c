/*
 * 	OV7670.c
 *
 *  Created on: 2022
 *  Author: J. Bajic
 *
 */

#include "OV7670.h"
#include "OV7670_REG.h"
#include <stdio.h>

// Internal Static Variables
static DCMI_HandleTypeDef *sp_hdcmi;
static DMA_HandleTypeDef *sp_hdma_dcmi;
static I2C_HandleTypeDef *sp_hi2c;
static LPTIM_HandleTypeDef *sp_hlptim;

static uint8_t capture_mode=0;
static uint32_t img_address=0;
static uint16_t img_width=320;
static uint16_t img_height=240;
static uint8_t img_format=YUV422;

static uint8_t mtx_yuv[]={0x80, 0x80, 0x00, 0x22, 0x5E, 0x80};
static uint8_t mtx_rgb[]={0xB3, 0xB3, 0x00, 0x3D, 0xB0, 0xE4};


void OV7670_Init(DCMI_HandleTypeDef *p_hdcmi, DMA_HandleTypeDef *p_hdma_dcmi, I2C_HandleTypeDef *p_hi2c, LPTIM_HandleTypeDef *p_hlptim)
{
	sp_hdcmi=p_hdcmi;
	sp_hdma_dcmi=p_hdma_dcmi;
	sp_hi2c=p_hi2c;
	sp_hlptim=p_hlptim;
}

void OV7670_Power(uint8_t en)
{
	if(en)
		HAL_GPIO_WritePin(OV7670_PWDN_PORT, OV7670_PWDN_PIN, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(OV7670_PWDN_PORT, OV7670_PWDN_PIN, GPIO_PIN_SET);
	HAL_Delay(50);
}

void OV7670_ResetHW(void)
{
	HAL_GPIO_WritePin(OV7670_RST_PORT, OV7670_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(OV7670_RST_PORT, OV7670_RST_PIN, GPIO_PIN_SET);
	HAL_Delay(50);
}

void OV7670_XCLK(uint8_t en)
{
	if(en)
		HAL_LPTIM_PWM_Start(sp_hlptim, 6,3);//6 3 8mhz
	else
		HAL_LPTIM_PWM_Stop(sp_hlptim);
}

void OV7670_WriteSCCB(uint8_t regAddr, uint8_t val)
{
	uint8_t	txbuf[2]={regAddr, val};
	HAL_I2C_Master_Transmit(sp_hi2c, OV7670_ADDR, (uint8_t *)txbuf, 2, OV7670_TIMEOUT);
}

void OV7670_ReadSCCB(uint8_t regAddr, uint8_t *data)
{
	HAL_I2C_Master_Transmit(sp_hi2c, OV7670_ADDR, &regAddr, 1, OV7670_TIMEOUT);
	HAL_I2C_Master_Receive(sp_hi2c, OV7670_ADDR, data, 1, OV7670_TIMEOUT);
}

void OV7670_Start(Capture_mode mode, uint32_t *capture_address)
{
	capture_mode=mode;
	img_address=(uint32_t)capture_address;

	if(capture_mode == SNAPSHOT)
		HAL_DCMI_Start_DMA(sp_hdcmi, DCMI_MODE_SNAPSHOT, img_address, img_width * img_height/2);
	else
		HAL_DCMI_Start_DMA(sp_hdcmi, DCMI_MODE_CONTINUOUS, img_address, img_width * img_height/2);
}

void OV7670_Stop()
{
	HAL_DCMI_Stop(sp_hdcmi);
}

void OV7670_ResetSW(void)
{
	OV7670_WriteSCCB(REG_COM7, 0x80);
	HAL_Delay(30);
	OV7670_Config(defaults);
	HAL_Delay(10);
}

void OV7670_Config(const uint8_t params[][2])
{
	for(int i = 0; params[i][0] != 0xFF; i++)
	{
		OV7670_WriteSCCB(params[i][0], params[i][1]);
		HAL_Delay(1);
	}
}

void OV7670_SetBrightness(uint8_t brightness)
{
	if(brightness>=127)
		brightness=brightness-127;
	else
		brightness=255-brightness;

	OV7670_WriteSCCB(REG_BRIGHT, brightness);
}

void OV7670_SetContrast(uint8_t contrast)
{
	OV7670_WriteSCCB(REG_CONTRAS, contrast);
}

void OV7670_SetSaturation(uint8_t saturation)//0-100, default 80
{
	float temp=0.0f;
	uint8_t i;

	saturation+=20;

	for(i=0;i<6;i++)
	{
		if(img_format==YUV422)temp=(float)mtx_yuv[i];
		else temp=(float)mtx_rgb[i];
		temp*=saturation;
		temp/=100;
		OV7670_WriteSCCB(REG_MTX1+i, (uint8_t)temp);
	}
	OV7670_WriteSCCB(REG_MTX_SIGN, 0x9E);
}

void OV7670_SetResolution(Camera_resolution resolution)
{

	switch(resolution)
	{
		case VGA:	//NOT TESTED
			OV7670_Config(RES_VGA);
			OV7670_SetFrameControl(158,14,10,490);
			img_width=640;
			img_height=480;
			break;
		case QVGA:	//OK
			OV7670_Config(RES_QVGA);
			OV7670_SetFrameControl(168,24,12,492);
			img_width=320;
			img_height=240;
			break;
		case QQVGA:	//OK
			OV7670_Config(RES_QQVGA);
			OV7670_SetFrameControl(174,30,12,492);
			img_width=160;
			img_height=120;
			break;
		case QQQVGA:	//OK
			OV7670_Config(RES_QQQVGA);
			OV7670_SetFrameControl(196,52,12,492);//(196+640)%784=52
			img_width=80;
			img_height=60;
			break;
		case CIF:	//OK
			OV7670_Config(RES_CIF);
			OV7670_SetFrameControl(174,94,12,489); //for vstop=492 image moves out
			img_width=352;
			img_height=288;
			break;
		case QCIF:	//OK
			OV7670_Config(RES_QCIF);
			OV7670_SetFrameControl(454,22,12,492); //for hstart=454, htop=24 incorect last vertical line
			img_width=176;
			img_height=144;
			break;
		case QQCIF: //OK
			OV7670_Config(RES_QQCIF);
			OV7670_SetFrameControl(474,42,12,492); //for hstart=454, htop=24 incorrect first line, incorrect colors
			img_width=88;
			img_height=72;
			break;
	}
}

void OV7670_SetColorFormat(Camera_format format)
{
	uint8_t temp[2];

	OV7670_ReadSCCB(REG_COM7, &temp[0]);
	temp[0]&=0b11111010;
	OV7670_ReadSCCB(REG_COM15, &temp[1]);
	temp[1]&=0b00001111;
	HAL_Delay(10);
	switch(format)
	{
	//According to OV7670/7171 implementation guide v1.0 - Table 2-1
		case YUV422:
			OV7670_WriteSCCB(REG_COM7, temp[0] | 0x00);
			OV7670_WriteSCCB(REG_COM15, temp[1] | 0x00);
			img_format=YUV422;
			break;
		case RGB565: //Poor (greenish) image - ???
			OV7670_WriteSCCB(REG_COM7, temp[0] | 0x04);//RGB
			OV7670_WriteSCCB(REG_COM15, temp[1] | 0x10);//RGB565
			//???
			img_format=RGB565;
			break;
	}
}

void OV7670_AutoExposureMode(Camera_state en)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM8, &temp);

	if(en)
		OV7670_WriteSCCB(REG_COM8, temp | 0x01);	//enable AEC
	else
		OV7670_WriteSCCB(REG_COM8, temp & 0xFE);	//disable AEC
}

void OV7670_SetAECAlgorithm(Camera_algorithm algorithm)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_HAECC7, &temp);

	if(algorithm)

		OV7670_WriteSCCB(REG_HAECC7, temp | 0x80);// Histogram-based
	else
		OV7670_WriteSCCB(REG_HAECC7, temp & 0x7F);// Average-based
}

void OV7670_SetExposureTime(uint16_t time) //AEC must be off
{
	OV7670_WriteSCCB(REG_AECHH, (time>>10) & 0x3F);
	OV7670_WriteSCCB(REG_AECH, (time>>2) & 0xFF);
	OV7670_WriteSCCB(REG_COM1, (time) & 0x03);
}

void OV7670_SetGainCeiling(Camera_gain value)//value 0-6 [2x-128x]
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM9, &temp);
	temp&=0x8F;
	OV7670_WriteSCCB(REG_COM9, temp | (value<<4));
}

void OV7670_AutoGainMode(Camera_state en)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM8, &temp);

	if(en)
		OV7670_WriteSCCB(REG_COM8, temp | 0x04); //enable AGC
	else
		OV7670_WriteSCCB(REG_COM8, temp & 0xFB); //disable AGC
}

void OV7670_SetGain(uint8_t gain)//value 0-7 [1x-128x], AGC must be off
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_VREF, &temp);
	temp&=0x3F;

	//According to OV7670/7171 implementation guide v1.0 - Table 4-1
	switch(gain)
	{
		case 0: //1x
			OV7670_WriteSCCB(REG_GAIN, 0x00);
			OV7670_WriteSCCB(REG_VREF,  temp | 0x00);
			break;
		case 1: //2x
			OV7670_WriteSCCB(REG_GAIN, 0x10);
			OV7670_WriteSCCB(REG_VREF,  temp | 0x00);
			break;
		case 2: //4x
			OV7670_WriteSCCB(REG_GAIN, 0x30);
			OV7670_WriteSCCB(REG_VREF,  temp | 0x00);
			break;
		case 3: //8x
			OV7670_WriteSCCB(REG_GAIN, 0x70);
			OV7670_WriteSCCB(REG_VREF,  temp | 0x00);
			break;
		case 4: //16x
			OV7670_WriteSCCB(REG_GAIN, 0xF0);
			OV7670_WriteSCCB(REG_VREF,  temp | 0x00);
			break;
		case 5: //32x
			OV7670_WriteSCCB(REG_GAIN, 0xF0);
			OV7670_WriteSCCB(REG_VREF,  temp | 0x40);
			break;
		case 6: //64x
			OV7670_WriteSCCB(REG_GAIN, 0xF0);
			OV7670_WriteSCCB(REG_VREF,  temp | 0xC0);
			break;
		case 7: //~128x
			OV7670_WriteSCCB(REG_GAIN, 0xFF);
			OV7670_WriteSCCB(REG_VREF,  temp | 0xC0);
			break;
	}
}

void OV7670_AutoWhiteBalanceMode(Camera_state en)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM8, &temp);

	if(en)
		OV7670_WriteSCCB(REG_COM8, temp | 0x02); //enable AWB
	else
		OV7670_WriteSCCB(REG_COM8, temp & 0xFD); //disable AWB
}

void OV7670_SetChannelsGain(uint8_t r_ch, uint8_t g_ch, uint8_t b_ch) //AWB must be off
{
	OV7670_WriteSCCB(REG_RED, r_ch);
	OV7670_WriteSCCB(REG_GGAIN, g_ch);
	OV7670_WriteSCCB(REG_BLUE, b_ch);
}

void OV7670_ShowColorBar(Camera_state en)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM17, &temp);

	if(en)
		OV7670_WriteSCCB(REG_COM17, temp | 0x08);
	else
		OV7670_WriteSCCB(REG_COM17, temp & 0xF7);
}

void OV7670_FlipHorizontal(Camera_state flipH)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_MVFP, &temp);

	if(flipH)
		OV7670_WriteSCCB(REG_MVFP, temp | 0x20);
	else
		OV7670_WriteSCCB(REG_MVFP, temp & 0xDF);
}

void OV7670_FlipVertical(Camera_state flipV)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_MVFP, &temp);

	if(flipV)
		OV7670_WriteSCCB(REG_MVFP, temp | 0x10);
	else
		OV7670_WriteSCCB(REG_MVFP, temp & 0xEF);
}

void OV7670_NightMode(Camera_state en,	Camera_fps min_fps)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM11, &temp);

	if(en)
	{
		temp|=(min_fps<<5);
		OV7670_WriteSCCB(REG_COM11, temp | 0x80);
	}
	else
		OV7670_WriteSCCB(REG_COM11, temp & 0x7F);
}

void OV7670_AutoSharpness(Camera_state en)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM16, &temp);

	if(en)
		OV7670_WriteSCCB(REG_COM16, temp | 0x20);// enable Auto Sharpness
	else
		OV7670_WriteSCCB(REG_COM16, temp & 0xDF);// disable Auto Sharpness
}

void OV7670_SetSharpness(uint8_t sharpness)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_EDGE, &temp);
	temp&=0xE0;
	OV7670_WriteSCCB(REG_EDGE, temp | sharpness);
}

void OV7670_AutoDeNoise(Camera_state en)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM16, &temp);

	if(en)
		OV7670_WriteSCCB(REG_COM16, temp | 0x10);// enable Auto Sharpness
	else
		OV7670_WriteSCCB(REG_COM16, temp & 0xEF);// disable Auto Sharpness
}

void OV7670_SetDeNoiseStrength(uint8_t strength)
{
	OV7670_WriteSCCB(REG_DNSTH, strength);
}

void OV7670_BandingFilter(Camera_state en, Camera_filter filter)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_COM8, &temp);
	if(en)
	{
		OV7670_WriteSCCB(REG_BD50ST, BF_50Hz_VALUE); //50Hz banding filter
		OV7670_WriteSCCB(REG_BD60ST, BF_60Hz_VALUE); //60Hz banding filter

		OV7670_WriteSCCB(REG_COM8, temp | 0x20); //banding filter enable

		OV7670_ReadSCCB(REG_COM11, &temp);
		temp&=0xE7;
		switch(filter)
		{
			case F_50Hz:
				OV7670_WriteSCCB(REG_COM11, temp | 0x0A);
				break;
			case F_60Hz:
				OV7670_WriteSCCB(REG_COM11, temp | 0x02);
				break;
			case F_AUTO:
				OV7670_WriteSCCB(REG_COM11, temp | 0x12);
				break;
		}
	}
	else
		OV7670_WriteSCCB(REG_COM8, temp & 0xDF);
}

void OV7670_SpecialEffect(Camera_effect effect)
{
	uint8_t temp=0;

	OV7670_ReadSCCB(REG_TSLB, &temp);
	temp&=0xCB;

	switch(effect)
	{
		case NORMAL:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x04);
			break;
		case ANTIQUE:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x14);
			OV7670_WriteSCCB(REG_MANU, 0xA0);
			OV7670_WriteSCCB(REG_MANV, 0x40);
			break;
		case BW:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x14);
			OV7670_WriteSCCB(REG_MANU, 0x80);
			OV7670_WriteSCCB(REG_MANV, 0x80);
			break;
		case NEGATIVE:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x24);
			break;
		case BW_NEGATIVE:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x34);
			OV7670_WriteSCCB(REG_MANU, 0x80);
			OV7670_WriteSCCB(REG_MANV, 0x80);
			break;
		case BLUISH:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x14);
			OV7670_WriteSCCB(REG_MANU, 0x80);
			OV7670_WriteSCCB(REG_MANV, 0xC0);
			break;
		case GREENISH:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x14);
			OV7670_WriteSCCB(REG_MANU, 0x40);
			OV7670_WriteSCCB(REG_MANV, 0x40);
			break;
		case REDDISH:
			OV7670_WriteSCCB(REG_TSLB, temp | 0x14);
			OV7670_WriteSCCB(REG_MANU, 0xC0);
			OV7670_WriteSCCB(REG_MANV, 0x80);
			break;
	}
}

void OV7670_SetFrameRate(uint8_t div, uint8_t mul)
{
	OV7670_WriteSCCB(REG_CLKRC, 0x80 | div);
	HAL_Delay(1);
	OV7670_WriteSCCB(REG_DBLV, 0x08 | mul);
	HAL_Delay(1);
}

void OV7670_PowerDown(void)
{
	OV7670_XCLK(OFF);
	OV7670_Power(OFF);
}

void OV7670_PowerUp(void)
{
	OV7670_Power(ON);
	OV7670_XCLK(ON);
	OV7670_ResetHW();
	OV7670_ResetSW();
}

void OV7670_UpdateSettings(Camera_settings OV7670)
{
	OV7670_SetResolution(OV7670.resolution);

	if(OV7670.format) OV7670_SetColorFormat(RGB565);
	else OV7670_SetColorFormat(YUV422);

	OV7670_SpecialEffect(OV7670.effect);
	OV7670_AutoExposureMode(OV7670.AEC);
	if(!OV7670.AEC) OV7670_SetExposureTime(OV7670.exposure);
	OV7670_SetAECAlgorithm(OV7670.algorithm);

	OV7670_AutoGainMode(OV7670.AGC);
	if(!OV7670.AGC) OV7670_SetGain(OV7670.gain);

	OV7670_AutoWhiteBalanceMode(OV7670.AWB);
	if(!OV7670.AWB) OV7670_SetChannelsGain(OV7670.rch_gain, OV7670.gch_gain, OV7670.bch_gain);

	OV7670_SetBrightness(OV7670.brightness);
	OV7670_SetContrast(OV7670.contrast);
	OV7670_SetSaturation(OV7670.saturation);

	OV7670_AutoSharpness(OV7670.ASC);
	if(!OV7670.ASC) OV7670_SetSharpness(OV7670.sharpness);

	OV7670_SetGainCeiling(OV7670.gain_ceiling);

	OV7670_FlipHorizontal(OV7670.h_flip);
	OV7670_FlipVertical(OV7670.v_flip);
	OV7670_ShowColorBar(OV7670.cbar);
	OV7670_NightMode(OV7670.night_mode, OV7670.night_mode_fps);

	OV7670_AutoDeNoise(OV7670.ADN);
	if(!OV7670.ADN)OV7670_SetDeNoiseStrength(OV7670.strength);

	OV7670_BandingFilter(OV7670.BDF, OV7670.bd_filter_value);
}

void OV7670_SetFrameControl(uint16_t hstart, uint16_t hstop, uint16_t vstart, uint16_t vstop)
{
	OV7670_WriteSCCB(REG_HSTART, (hstart >> 3) & 0xff);
	OV7670_WriteSCCB(REG_HSTOP, (hstop >> 3) & 0xff);
	OV7670_WriteSCCB(REG_HREF, ((hstop & 0x7) << 3) | (hstart & 0x7));

	OV7670_WriteSCCB(REG_VSTART, (vstart >> 2) & 0xff);
	OV7670_WriteSCCB(REG_VSTOP, (vstop >> 2) & 0xff);
	OV7670_WriteSCCB(REG_VREF,((vstop & 0x3) << 2) | (vstart & 0x3));
}

void calculateExposureTimeMS(uint16_t aec_reg_val,uint16_t *time_num, uint8_t *time_fract)
{
	//According to OV7670/7171 implementation guide v1.0 - Section 3.3.1
	float time=((float)aec_reg_val)*T_ROW/1000;
	*time_num=(int)(time);
	*time_fract=(int)((time-(*time_num))*10);
}

void OV7670_getImageInfo(uint16_t *width, uint16_t *height, uint8_t *format)
{
	*width=img_width;
	*height=img_height;
	*format=img_format;
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	if(capture_mode==CONTINUOUS)
		HAL_DMA_Start_IT(hdcmi->DMA_Handle, (uint32_t)&hdcmi->Instance->DR, img_address, img_width * img_height/2);

}

/*void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{}*/
/*
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{}*/
