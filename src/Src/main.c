/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "libjpeg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//STemWin
#include "WM.h"
#include "GUI.h"
//Hardware
#include "SSD1963.h"
#include "STMPE610.h"
#include "OV7670.h"
//Windows
#include "MainWindowDLG.h"
#include "SettingsDLG.h"
//Middleware
#include "img.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma_dcmi;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

LPTIM_HandleTypeDef hlptim1;

SD_HandleTypeDef hsd1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

WM_HWIN hWin;
WM_HWIN hWin1;

#define I2C_pins	1
#define DCMI_pins	0

#define STREAM 		0
#define STOP 		1
#define PROCESS_TP 	2
#define START 		3
#define SETTING 	4

uint8_t menu=0, enable_tp=0;
int keyPressed = 0;
uint8_t scale=1;

Image_info image;

Camera_settings OV7670_settings={
		QVGA, 		//Resolution
		YUV422, 	//Format
		NORMAL, 	//Effect
		ON,			//AEC
		ON, 		//AGC
		ON, 		//AWB
		OFF,		//Color bar
		OFF,		//vertical flip
		OFF,		//Horizontal flip
		OFF,		//Night mode
		OFF,		//ASC
		ON,			//De-noise
		ON,			//Banding filter
		HISTOGRAM,	//AEC algorithm
		QUARTER_FPS,//Min. fps in night mode
		F_AUTO,		//Auto detect banding freq.
		256, 		//Exposure - 2 bytes
		4, 			//Gain	[0-7]=[1-128]
		160,		//Brightness - byte
		64, 		//Contrast - byte
		80, 		//Saturation - byte
		2,			//Sharpness	- [0-31]
		0,			//De-noise strength - byte
		x16, 		//Gain ceiling
		77, 		//R channel gain - byte
		103, 		//G channel gain - byte
		153			//B channel gain - byte
};

FATFS myFAT;
FIL myFile;
DIR dr;
FILINFO fno={0,0,0,0,{0}};
int file_index=1;
uint16_t image_count=0;

/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_I2C2_Init(void);
static void MX_DCMI_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void use_PB6_PB7_as(uint8_t AF_pins);
void updateCameraSettings(Camera_settings *OV7670);
void updateSettingsWindow(Camera_settings OV7670);
void saveJPGtoSD(void);
void loadJPGfromSD(void);
/* Private function prototypes -----------------------------------------------*/

extern volatile GUI_TIMER_TIME OS_TimeMS;
void HAL_SYSTICK_Callback(void)
{
	OS_TimeMS++;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_I2C2_Init();
  MX_DCMI_Init();
  MX_LPTIM1_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_LIBJPEG_Init();
  /* USER CODE BEGIN 2 */

  //Notice: PB6 and PB7 are shared between camera module (DCMI) and touch panel controller (I2C1)
  //Power down camera module and config PB6 and PB7 as I2C1 pins for touch panel controller
  OV7670_Power(DISABLE);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9); //PB8 & PB9 are set as I2C1 pins with CubeMX
  use_PB6_PB7_as(I2C_pins);	//Switch I2C1 to PB6 and PB7

  //Init and map peripherals
  Init_LCD_GPIO();
  Init_TOUCH_GPIO(hi2c1);
  STMPE610_Init();
  STMPE610_read_xyz();
  //I2C2 is used for SCCB and LPTIM1 for XLCK generation
  OV7670_Init(&hdcmi, &hdma_dcmi, &hi2c2, &hlptim1);

  HAL_Delay(100);

  //init GUI

  WM_SetCreateFlags(WM_CF_MEMDEV);
  GUI_Init();
  GUI_Clear();
  hWin = CreateMainWindow();
  GUI_Exec();

  //default state: stop capture
  menu=STOP;

  //UNLINK PATH
  FATFS_UnLinkDriver(SDPath);
  HAL_Delay(10);

  //TIM3 checks for touch detection - if touched capture is stopped
  HAL_TIM_Base_Start_IT(&htim3);

  //Due to JPG recording GUI_NUMBYTES had to be reduced
  for(int i=0; i<12;i++)
  {
	  GUI_RECT rect = {0, i*40, 800, (i+1)*40};
	  WM_InvalidateRect(hWin,&rect);
	  GUI_Exec();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  switch(menu)
	  {
	  	  case STREAM://draw captured data
	  		  	  	  drawImage(image, scale);
	  	  	  	  	  break;
	  	  case STOP:  //stop capture and power down camera so touch panel can be used
	  		  	  	  OV7670_Stop();
	  		  	  	  OV7670_PowerDown();

	  	  	  	  	  use_PB6_PB7_as(I2C_pins);
	  	  	  	  	  enable_tp=1;

	  	  	  	  	  menu=PROCESS_TP;
	  	  case PROCESS_TP://process touch panel
			  	  	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

			  	  	  STMPE610_read_xyz();

					  keyPressed=GUI_GetKey();

					  switch(keyPressed)
					  {
					  	  case BTN_CANCEL://Cancel - Discard changed settings
					  		  WM_DeleteWindow(hWin1);
					  		  WM_InvalidateWindow(hWin);
					  		  break;
					  	  case BTN_OK://Ok - Save settings
					  		  updateCameraSettings(&OV7670_settings);
					  		  scale=SPINBOX_GetValue(WM_GetDialogItem(hWin1, SBOX_SCALE));
							  WM_DeleteWindow(hWin1);
							  WM_InvalidateWindow(hWin);
					  		  break;
					  	  case BTN_STREAM://Start camera
					  		  menu=START;
					  		  break;
					  	  case BTN_SETTINGS: // Run settings window and update values
					  		  hWin1 = CreateSettings();
					  		  updateSettingsWindow(OV7670_settings);
							  SPINBOX_SetValue(WM_GetDialogItem(hWin1, SBOX_SCALE), scale);
					  		  break;
					  	  case BTN_CAPTURE://Capture - save JPG
					  		  saveJPGtoSD();
					  		  break;
					  	  case BTN_GALLERY://Read - load JPG
					  		  loadJPGfromSD();
					  		  clearImage();
					  		  drawImage(image, 1);
					  		  break;
					  }
	  		  	  	  break;
			case START: //power up, config camera and start capture
						enable_tp=0;
						STMPE610_read_xyz();
						use_PB6_PB7_as(DCMI_pins);

						OV7670_PowerUp();
						OV7670_UpdateSettings(OV7670_settings);
						OV7670_SetFrameRate(XCLK_DIV(1), PLL_x4);//seems less stable for higher pll
						HAL_Delay(10);

						image.address=getImageAddress();
						OV7670_Start(CONTINUOUS,image.address);
						OV7670_getImageInfo(&image.width, &image.height, &image.format);

						clearImage();
						menu=STREAM;
						break;
	  }

  	  GUI_TOUCH_Exec();
  	  GUI_Exec();



}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DCMI Initialization Function
  * @param None
  * @retval None
  */
static void MX_DCMI_Init(void)
{

  /* USER CODE BEGIN DCMI_Init 0 */

  /* USER CODE END DCMI_Init 0 */

  /* USER CODE BEGIN DCMI_Init 1 */

  /* USER CODE END DCMI_Init 1 */
  hdcmi.Instance = DCMI;
  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;
  hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_HIGH;
  hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;
  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
  hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
  hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
  hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
  hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
  if (HAL_DCMI_Init(&hdcmi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCMI_Init 2 */

  /* USER CODE END DCMI_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20404768;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10606AA0;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 27;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(STMPE610_IsTouched()&&(enable_tp==0))
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET);
		menu=STOP;
	}
}

void use_PB6_PB7_as(uint8_t AF_pins)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(AF_pins)
	{
	///	__HAL_RCC_DCMI_CLK_DISABLE();
		/**I2C1 GPIO Configuration
	    PB6     ------> I2C1_SCL
	    PB7     ------> I2C1_SDA
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	    GPIO_InitStruct.Pull = GPIO_PULLUP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    /* Peripheral clock enable */
	 //   __HAL_RCC_I2C1_CLK_ENABLE();
	}
	else
	{
	//	__HAL_RCC_I2C1_CLK_DISABLE();
	    /**DCMI GPIO Configuration
	    PB6     ------> DCMI_D5
	    PB7     ------> DCMI_VSYNC
	    */
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral clock enable */
	//	__HAL_RCC_DCMI_CLK_ENABLE();
	}
}

void updateCameraSettings(Camera_settings *OV7670)
{
	OV7670->resolution=DROPDOWN_GetSel(WM_GetDialogItem(hWin1, DDWN_RESOLUTION));
	OV7670->format=DROPDOWN_GetSel(WM_GetDialogItem(hWin1, DDWN_FORMAT));
	OV7670->effect=DROPDOWN_GetSel(WM_GetDialogItem(hWin1, DDWN_EFFECT));
	OV7670->exposure=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_EXPOSURE));
	OV7670->AEC=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_EXPOSURE));
	OV7670->algorithm=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_HIST_AEC));
	OV7670->gain=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_GAIN));
	OV7670->AGC=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_GAIN));
	OV7670->brightness=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_BRIGHTNESS));
	OV7670->contrast=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_CONTRAST));
	OV7670->saturation=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_SATURATION));
	OV7670->gain_ceiling=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_GAIN_CEIL));
	OV7670->rch_gain=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_RCH_GAIN));
	OV7670->gch_gain=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_GCH_GAIN));
	OV7670->bch_gain=SLIDER_GetValue(WM_GetDialogItem(hWin1, SLDR_BCH_GAIN));
	OV7670->AWB=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_AWB));
	OV7670->h_flip=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_HFLIP));
	OV7670->v_flip=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_VFLIP));
	OV7670->cbar=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_CBAR));
	OV7670->night_mode=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_NIGHT_MODE));
	OV7670->ADN=CHECKBOX_GetState(WM_GetDialogItem(hWin1,CBOX_DENOISE));
}
void updateSettingsWindow(Camera_settings OV7670)
{
	DROPDOWN_SetSel(WM_GetDialogItem(hWin1, DDWN_RESOLUTION), OV7670.resolution);
	DROPDOWN_SetSel(WM_GetDialogItem(hWin1, DDWN_FORMAT), OV7670.format);
	DROPDOWN_SetSel(WM_GetDialogItem(hWin1, DDWN_EFFECT), OV7670.effect);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_EXPOSURE), OV7670.exposure);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_EXPOSURE), OV7670.AEC);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_HIST_AEC), OV7670.algorithm);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_GAIN), OV7670.gain);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_GAIN), OV7670.AGC);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_BRIGHTNESS), OV7670.brightness);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_CONTRAST), OV7670.contrast);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_SATURATION), OV7670.saturation);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_GAIN_CEIL), OV7670.gain_ceiling);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_RCH_GAIN), OV7670.rch_gain);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_GCH_GAIN), OV7670.gch_gain);
	SLIDER_SetValue(WM_GetDialogItem(hWin1, SLDR_BCH_GAIN), OV7670.bch_gain);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_AWB), OV7670.AWB);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_HFLIP), OV7670.h_flip);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_VFLIP), OV7670.v_flip);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_CBAR), OV7670.cbar);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_NIGHT_MODE), OV7670.night_mode);
	CHECKBOX_SetState(WM_GetDialogItem(hWin1,CBOX_DENOISE), OV7670.ADN);

	//Notice labels are updated in SettingsDLG.c based on slider values
}

void saveJPGtoSD(void)
{
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==GPIO_PIN_RESET)//SD card present?
	{
		HAL_TIM_Base_Stop_IT(&htim3); //temporarily stop timer
		FATFS_LinkDriver(&SD_Driver, SDPath);
		f_mount(&myFAT, SDPath, 1);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

		char file_name[20] = {0};
		sprintf(file_name, "JPG/IMG_%d.jpg",image_count);

		f_open(&myFile, file_name, FA_WRITE | FA_CREATE_ALWAYS);
		writeJPG(&myFile, 100, image);

		sprintf(file_name, "IMG_%d.JPG",image_count);
		TEXT_SetText(WM_GetDialogItem(hWin, TXT_JPG), file_name);

		image_count++;

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

		f_close(&myFile);
		f_mount(0, SDPath, 1);
		FATFS_UnLinkDriver(SDPath);
		HAL_TIM_Base_Start_IT(&htim3);//start timer
	}
}
void loadJPGfromSD(void)
{
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==GPIO_PIN_RESET)//SD card present?
	{
		HAL_TIM_Base_Stop_IT(&htim3); 	//temporarily stop timer
		FATFS_LinkDriver(&SD_Driver, SDPath);
		f_mount(&myFAT, SDPath, 1);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);

		f_opendir(&dr,"JPG");
		if(fno.fname[0])
		{
			for(uint8_t i=0;i<=file_index;i++)f_findnext(&dr, &fno); //go to file
			file_index++;
		}
		if(!fno.fname[0])
		{
			f_findfirst(&dr, &fno, "JPG", "*.jpg");
			file_index=1;
		}

		char file_name[20] = {0};

		sprintf(file_name, "%s/%s", "JPG", fno.fname);
		f_open(&myFile, file_name, FA_READ);
		image.address=getImageAddress();
		readJPG(&myFile, &image);

		TEXT_SetText(WM_GetDialogItem(hWin, TXT_JPG), fno.fname);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

		f_closedir(&dr);
		f_close(&myFile);
		f_mount(0, SDPath, 1);
		FATFS_UnLinkDriver(SDPath);
		HAL_TIM_Base_Start_IT(&htim3); //start timer
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
