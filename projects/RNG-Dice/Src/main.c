/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "rng.h"
#include "spi.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32l073z_eval_lcd.h"
#include "stdbool.h"
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

void MyLCD_FillRoundedSquare(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r) {
    // Draw + shape to form the square body
    BSP_LCD_FillRect(x + r, y + 0, w-r-r, h); // (r, 0) to (w-r, h)
    BSP_LCD_FillRect(x + 0, y + r, w+1, h-r-r); // (0, r) to (w, h-r)
    // +1 to fix bug of showing one pixel less
    // no idea why that happens

    // Plot circles at 4 corners
    BSP_LCD_FillCircle(x + r, y + r, r);         // (r, r)
    BSP_LCD_FillCircle(x + (w-r), y + r, r);     // (w-r, r)
    BSP_LCD_FillCircle(x + r, y + (h-r), r);     // (r, h-r)
    BSP_LCD_FillCircle(x + (w-r), y + (h-r), r); // (w-r, h-r)
}

void MyLCD_DisplayDice(int number) {
    const uint16_t lcd_x_mid = BSP_LCD_GetXSize() / 2;
    const uint16_t lcd_y_mid = BSP_LCD_GetYSize() * 0.6;

    const uint16_t body_size = 125;
    const uint16_t body_radius = 20;

    const uint16_t dots_size = 65;
    const uint16_t dots_radius = 12;

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    MyLCD_FillRoundedSquare(
            lcd_x_mid - body_size / 2,
            lcd_y_mid - body_size / 2,
            body_size, body_size,
            body_radius);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    const bool dot_mid = number & 0x1; // odd number
    const bool dot_br = number > 3;
    const bool dot_bl = number > 1;
    const bool dot_tr = number > 1;
    const bool dot_tl = number > 3;
    const bool dot_mr = number == 6;
    const bool dot_ml = number == 6;

    // middle
    if (dot_mid)
        BSP_LCD_FillCircle(lcd_x_mid, lcd_y_mid, dots_radius);
    // bottom-right
    if (dot_br)
        BSP_LCD_FillCircle(lcd_x_mid + dots_size / 2, lcd_y_mid + dots_size / 2, dots_radius);
    // bottom-left
    if (dot_bl)
        BSP_LCD_FillCircle(lcd_x_mid - dots_size / 2, lcd_y_mid + dots_size / 2, dots_radius);
    // top-right
    if (dot_tr)
        BSP_LCD_FillCircle(lcd_x_mid + dots_size / 2, lcd_y_mid - dots_size / 2, dots_radius);
    // top-left
    if (dot_tl)
        BSP_LCD_FillCircle(lcd_x_mid - dots_size / 2, lcd_y_mid - dots_size / 2, dots_radius);
    // mid-right
    if (dot_mr)
        BSP_LCD_FillCircle(lcd_x_mid + dots_size / 2, lcd_y_mid, dots_radius);
    // mid-left
    if (dot_ml)
        BSP_LCD_FillCircle(lcd_x_mid - dots_size / 2, lcd_y_mid, dots_radius);
}

bool PC13_Pressed = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_13) {
        PC13_Pressed = true;
    }
}

void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef* hrng, uint32_t random32bit) {
    MyLCD_DisplayDice(random32bit % 6 + 1);

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(0, LINE(2), "Random Dice", CENTER_MODE);
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
  MX_RNG_Init();
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  BSP_LCD_Init();
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  HAL_RNG_GenerateRandomNumber_IT(&hrng);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
      if (PC13_Pressed) {
          PC13_Pressed = false;
          HAL_RNG_GenerateRandomNumber_IT(&hrng);
      }
      HAL_Delay(500);
  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

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

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  while(1) 
  {
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
