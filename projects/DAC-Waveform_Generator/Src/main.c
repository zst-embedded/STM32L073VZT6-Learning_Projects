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
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32l073z_eval.h"
#include "LCD_Waveform.h"
#include <stdbool.h>
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

typedef enum Modes {
    MODE_SINE = 0,
    MODE_TRIANGLE,
    MODE_SAWTOOTH,
} Mode_TypeDef;


#define DAC_DMA_SIZE (64)

// Explictly state volatile variables (especially in Keil)
volatile bool PC13_Pressed = false;
volatile JOYState_TypeDef Joystick_State = JOY_NONE;

volatile Mode_TypeDef mode_type = MODE_SINE;
volatile bool mode_updated = true;

uint16_t DAC_Array[DAC_DMA_SIZE];

void DAC_SetFrequency(TIM_HandleTypeDef *htim, int frequency, int dma_array_size) {
    /**
     * DMA is triggered by the timer. If the array size is n,
     * then n timer tiggers are needed to cycle through DMA.
     *
     * For whole cycle:
     *   1/SystemCoreClock * Timer Ticks * dma_array_size = Period
     *   SystemCoreClock / (dma_array_size * Timer Ticks) = Frequency
     *   Timer Ticks = SystemCoreClock / (dma_array_size * Frequency)
     */
    uint32_t period = SystemCoreClock / (dma_array_size * frequency);

    /**
     * If Period if larger than max 16-bits, increment the prescalar
     * and divide by 2 until it is within 16-bits.
     */
    uint32_t prescalar = 0;
    while (period > 0xFFFF) {
        period /= 2;
        prescalar++;
    }

    htim->Instance->ARR = period - 1;
    htim->Instance->PSC = prescalar;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // PC13
    if (GPIO_Pin == GPIO_PIN_13) {
        PC13_Pressed = true;
    }

    // PE6
    if (GPIO_Pin == GPIO_PIN_6) {
        JOYState_TypeDef joystick = BSP_JOY_GetState();
        if (joystick == JOY_UP ||
                joystick == JOY_DOWN ||
                joystick == JOY_LEFT ||
                joystick == JOY_RIGHT) {
            Joystick_State = joystick;
        }
        BSP_IO_ITClear();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        HAL_TIM_Base_Stop_IT(htim);
        switch (mode_type) {
        case MODE_SINE:
            MyLCD_DrawSineWave(LCD_COLOR_YELLOW);
            MyLCD_DrawSineWave(LCD_COLOR_RED);
            break;
        case MODE_TRIANGLE:
            MyLCD_DrawTriangleWave(LCD_COLOR_YELLOW);
            MyLCD_DrawTriangleWave(LCD_COLOR_RED);
            break;
        case MODE_SAWTOOTH:
            MyLCD_DrawSawtoothWave(LCD_COLOR_YELLOW);
            MyLCD_DrawSawtoothWave(LCD_COLOR_RED);
            break;
        }
        htim->Instance->CNT = 0; // Reset Count
        HAL_TIM_Base_Start_IT(htim);
    }

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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_DAC_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */

  // Init all LEDs
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  // Joystick
  BSP_JOY_Init(JOY_MODE_EXTI);

  // Turn on LCD
  MyLCD_Init();

  // Wait until key is pressed to begin
  while (!PC13_Pressed && Joystick_State == JOY_NONE) {
      __NOP();
  }
  PC13_Pressed = false;
  Joystick_State = JOY_NONE;
  MyLCD_ClearDiagram();

  // Enable TIM6 interrupt for updating SPI LCD
  HAL_TIM_Base_Start_IT(&htim6);

  // Start DAC on PA4 with TIM7 trigger DMA
  HAL_TIM_Base_Start(&htim7);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  uint32_t freq_digit = 1;
  uint32_t freq_multiplier = 100;

  while (1) {
      // Handle Button
      if (PC13_Pressed) {
          PC13_Pressed = false;
          mode_type++;
          mode_updated = true;
          MyLCD_ClearDiagram();
      }

      if (mode_updated) {
          mode_updated = false;
          HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);

          __disable_irq();
          switch (mode_type) {
          case MODE_SINE:
              MyLCD_HeaderText("     1. Sine wave     ", LCD_COLOR_CYAN);
              MyLCD_DrawSineWave(LCD_COLOR_RED);
              for (int i = 0; i < DAC_DMA_SIZE; i++) {
                  float rad = sin(1.0 * i/DAC_DMA_SIZE * 2*M_PI);
                  DAC_Array[i] = (uint16_t) (rad * 2047 + 2047);
                  // + 2047 because because range is 0 - 4095
              }
              break;
          case MODE_TRIANGLE:
              MyLCD_HeaderText("     2. Triangle wave     ", LCD_COLOR_CYAN);
              MyLCD_DrawTriangleWave(LCD_COLOR_RED);
              for (int i = 0; i < DAC_DMA_SIZE; i++) {
                  // percent is from -1.0 to 1.0
                  float percent = (2.0 * i / (DAC_DMA_SIZE - 1)) - 1.0;
                  DAC_Array[i] = abs(percent * 4095);
              }
              break;
          case MODE_SAWTOOTH:
              MyLCD_HeaderText("     3. Sawtooth wave     ", LCD_COLOR_CYAN);
              MyLCD_DrawSawtoothWave(LCD_COLOR_RED);
              for (int i = 0; i < DAC_DMA_SIZE; i++) {
                  // percent is from 0.0 to 1.0
                  float percent = 1.0 * i / (DAC_DMA_SIZE - 1);
                  DAC_Array[i] = round(percent * 4095);
              }
              break;
          default:
              mode_type = MODE_SINE;
              mode_updated = true;
              continue;
          }
          HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,
                  (uint32_t*) DAC_Array, DAC_DMA_SIZE, DAC_ALIGN_12B_R);
          __enable_irq();
      }

      // Handle Joystick
      if (Joystick_State != JOY_NONE) {
          if (Joystick_State == JOY_UP) {
              if (freq_multiplier < 1000) {
                  freq_multiplier *= 10;
              }
          } else if (Joystick_State == JOY_DOWN) {
              if (freq_multiplier > 1) {
                  freq_multiplier /= 10;
              }
          } else if (Joystick_State == JOY_LEFT) {
              if (freq_digit-- <= 1) {
                  freq_digit = 9;
              }
          } else if (Joystick_State == JOY_RIGHT) {
              if (freq_digit++ >= 9) {
                  freq_digit = 1;
              }
          }
          Joystick_State = JOY_NONE;
      }

      __disable_irq();
      uint32_t frequency = freq_digit * freq_multiplier;
      DAC_SetFrequency(&htim7, frequency, DAC_DMA_SIZE);
      char text[50];
      sprintf(text, "     %d Hz     ", frequency);
      MyLCD_FooterText(text, LCD_COLOR_CYAN);
      __enable_irq();
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  }
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
