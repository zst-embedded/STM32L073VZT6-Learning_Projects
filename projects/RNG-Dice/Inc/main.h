/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define EXT_RESET_Pin GPIO_PIN_2
#define EXT_RESET_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOE
#define MFX_IRQOUT_Pin GPIO_PIN_6
#define MFX_IRQOUT_GPIO_Port GPIOE
#define MFX_IRQOUT_EXTI_IRQn EXTI4_15_IRQn
#define SPI1_MOSI_DIR_Pin GPIO_PIN_9
#define SPI1_MOSI_DIR_GPIO_Port GPIOH
#define MFX_WKUP_Pin GPIO_PIN_10
#define MFX_WKUP_GPIO_Port GPIOH
#define DET_LPTIM_CH1_Pin GPIO_PIN_0
#define DET_LPTIM_CH1_GPIO_Port GPIOC
#define PRESSURE_Pin GPIO_PIN_0
#define PRESSURE_GPIO_Port GPIOA
#define DAC_OUT1_Pin GPIO_PIN_4
#define DAC_OUT1_GPIO_Port GPIOA
#define ADC_IN5_Pin GPIO_PIN_5
#define ADC_IN5_GPIO_Port GPIOA
#define DET_COMP2_OUT_Pin GPIO_PIN_7
#define DET_COMP2_OUT_GPIO_Port GPIOA
#define TST_MOSI_DIR_Pin GPIO_PIN_4
#define TST_MOSI_DIR_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_7
#define LED4_GPIO_Port GPIOE
#define USART4_TX_Pin GPIO_PIN_8
#define USART4_TX_GPIO_Port GPIOE
#define USART4_RX_3V3_Pin GPIO_PIN_9
#define USART4_RX_3V3_GPIO_Port GPIOE
#define TFT_CS_Pin GPIO_PIN_10
#define TFT_CS_GPIO_Port GPIOE
#define SPI1_SCK_Pin GPIO_PIN_13
#define SPI1_SCK_GPIO_Port GPIOE
#define SPI1_MISO_3V3_Pin GPIO_PIN_14
#define SPI1_MISO_3V3_GPIO_Port GPIOE
#define SPI1_MOSI_Pin GPIO_PIN_15
#define SPI1_MOSI_GPIO_Port GPIOE
#define LPUART_TX_Pin GPIO_PIN_8
#define LPUART_TX_GPIO_Port GPIOD
#define LPUART_RX_3V3_Pin GPIO_PIN_9
#define LPUART_RX_3V3_GPIO_Port GPIOD
#define LPUART_CTS_3V3_Pin GPIO_PIN_11
#define LPUART_CTS_3V3_GPIO_Port GPIOD
#define LPUART_RTS_Pin GPIO_PIN_12
#define LPUART_RTS_GPIO_Port GPIOD
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define MicroSD_CS_Pin GPIO_PIN_0
#define MicroSD_CS_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_1
#define LED3_GPIO_Port GPIOD
#define USART2_CTS_3V3_Pin GPIO_PIN_3
#define USART2_CTS_3V3_GPIO_Port GPIOD
#define USART2_RTS_Pin GPIO_PIN_4
#define USART2_RTS_GPIO_Port GPIOD
#define USART2_TX_Pin GPIO_PIN_5
#define USART2_TX_GPIO_Port GPIOD
#define USART2_RX_3V3_Pin GPIO_PIN_6
#define USART2_RX_3V3_GPIO_Port GPIOD
#define DET_PULSE_Pin GPIO_PIN_7
#define DET_PULSE_GPIO_Port GPIOD
#define DET_COMP2_INM_Pin GPIO_PIN_3
#define DET_COMP2_INM_GPIO_Port GPIOB
#define DET_COMP2_INP_Pin GPIO_PIN_4
#define DET_COMP2_INP_GPIO_Port GPIOB
#define TempSensor_INT_Pin GPIO_PIN_5
#define TempSensor_INT_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
