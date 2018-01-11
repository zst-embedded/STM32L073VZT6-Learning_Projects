/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "fatfs.h"
#include "spi.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32l073z_eval_lcd.h"
// BSP SD IO Drivers
#include "stm32l073z_eval_sd.h"
#include "sd_diskio.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

FRESULT f_open_append(FIL* fp, const char* path) {
    FRESULT fr;

    /* FA_OPEN_ALWAYS: Opens file if existing. If not, a new file is created. */
    /* FA_WRITE: Write access to file */
    fr = f_open(fp, path, FA_OPEN_ALWAYS | FA_WRITE);
    if (fr != FR_OK)
        return fr;

    /* Move file write pointer to the end of the file*/
    fr = f_lseek(fp, f_size(fp));
    if (fr != FR_OK)
        f_close(fp);
    return fr;
}

void WrapText(char *fromText, char *toText, int fromLength, int toLength, int offset) {
    for(int i = 0; i < (toLength - 1); i++) {
        int pos = (i + offset) % fromLength;
        if (fromText[pos] == '\0') {
            // ignore null chars & wrap around
            i--;
            offset++;
            continue;
        }
        toText[i] = fromText[pos];
    }
    toText[toLength - 1] = 0;
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI1_Init();
  MX_FATFS_Init();

  /* USER CODE BEGIN 2 */

  // Unlink driver from MX_FATFS_Init
  // Else, the SD card will not init or have no work area (error 12)
  FATFS_UnLinkDriver(USERPath);

  // Turn off LEDs
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  // Setup LCD
  BSP_LCD_Init();
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_DisplayStringAt(0, LINE(0), "FATFS SD Card Testing", CENTER_MODE);

  // Setup SD Card
  BSP_SD_Init();
  while(BSP_SD_IsDetected() == SD_NOT_PRESENT) {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(0, LINE(1), "Please Insert SD Card", CENTER_MODE);
      HAL_Delay(1000);
  }
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_ClearStringLine(1);
  BSP_LCD_DisplayStringAt(0, LINE(1), "SD Card Detected!", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);


  /********************************************************/
  // SD CARD SETUP
  /********************************************************/
  FRESULT SDres;    /* Return value for SD */
  char SDPath[4];   /* SD logical drive path */
  FATFS SDFatFS;    /* File system object for SD logical drive */
  FIL SDFile;       /* File object for SD */

  // Link the micro SD disk I/O driver
  SDres = FATFS_LinkDriver(&SD_Driver, SDPath);
  if (SDres != 0) { // Returns 0 if success
      Error_Handler();
  }

  // Register/Init file system object to the FatFs
  SDres = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
  if (SDres != FR_OK) {
      Error_Handler();
  }

  /********************************************************/
  // SD CARD WRITING
  /********************************************************/
  SDres = f_open(&SDFile, "WRITE_TEST.TXT", FA_CREATE_ALWAYS | FA_WRITE);
  if (SDres != FR_OK) {
      char buf[15];
      sprintf(buf, "Error: f_open. SDres = %d", SDres);
      BSP_LCD_DisplayStringAtLine(5, buf);
      Error_Handler();
  }

  uint32_t byteswritten;
  uint8_t wtext[] = "Hello world.\nSuccessfully wrote to file using f_write.\n"; /* File write buffer */
  SDres = f_write(&SDFile, wtext, strlen(wtext), &byteswritten);
  if(SDres != FR_OK || byteswritten == 0) {
      f_close(&SDFile);
      Error_Handler();
  }

  SDres = f_puts("Successfully wrote to file using f_puts.\n", &SDFile);
  if(SDres == EOF) {
      f_close(&SDFile);
      Error_Handler();
  }

  SDres = f_printf(&SDFile, "Successfully wrote to file using %s.\n", "f_printf");
  if(SDres == EOF) {
      f_close(&SDFile);
      Error_Handler();
  }

  // Close file after writing
  SDres = f_close(&SDFile);
  if (SDres != FR_OK) {
      Error_Handler();
  }

  /********************************************************/
  // SD CARD READING
  /********************************************************/
  SDres = f_open(&SDFile, "READ_TEST.TXT", FA_READ);
  if (SDres != FR_OK) {
      Error_Handler();
  }

  uint32_t bytesread;
  DWORD filesize = f_size(&SDFile); // Alternatively: DWORD filesize = SDFile.fsize;
  char *rbuffer = calloc(filesize, sizeof(char)); /* File read buffer */

  SDres = f_read(&SDFile, rbuffer, filesize, &bytesread);
  if (SDres != FR_OK || bytesread == 0) {
      f_close(&SDFile);
      Error_Handler();
  }

  // Close file after reading
  SDres = f_close(&SDFile);
  if (SDres != FR_OK) {
      Error_Handler();
  }

  /********************************************************/
  // SD CARD APPEND
  /********************************************************/
  SDres = f_open_append(&SDFile, "COUNT.TXT");
  if (SDres != FR_OK) {
      Error_Handler();
  }

  SDres = f_putc('A', &SDFile);
  if(SDres == EOF) {
      f_close(&SDFile);
      Error_Handler();
  }

  DWORD count = f_size(&SDFile);

  // Close file after reading
  SDres = f_close(&SDFile);
  if (SDres != FR_OK) {
      Error_Handler();
  }

  /* A character is appended on each boot.
   * The number of boots is thus the size of the file. */
  char buf1[100];
  sprintf(buf1, "Number of boots: %d", count);
  BSP_LCD_DisplayStringAt(0, LINE(14), buf1, RIGHT_MODE);

  /********************************************************/
  // SD CARD DE-INIT
  /********************************************************/
  FATFS_UnLinkDriver(SDPath);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  BSP_LCD_SetFont(&Font20);

  uint8_t chars_horizontal = (BSP_LCD_GetXSize() / BSP_LCD_GetFont()->Width) + 1;
  char *wrapped_text = calloc(chars_horizontal, sizeof(char));

  while (1) {
      for (int i = 0; i < filesize; i++) {
          WrapText(rbuffer, wrapped_text, filesize, chars_horizontal, i);
          BSP_LCD_DisplayStringAt(0, LINE(5), wrapped_text, LEFT_MODE);
          HAL_Delay(75);
      }
  }

  free(rbuffer);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    char buf[100];
    sprintf(buf, "Error at line %d", line);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAtLine(10, buf);
    BSP_LCD_DisplayStringAtLine(11, file);

    while(1) {
        BSP_LED_Toggle(LED_ORANGE);
        HAL_Delay(250);
    }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
