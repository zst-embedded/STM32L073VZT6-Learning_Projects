/**
  @page Demo   Demo STM32L073Z_EVAL
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32L073Z_EVAL Demo
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
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
  @endverbatim

@par Demo Description

The provided demonstration firmware based on STM32Cube helps you to discover STM32 
Cortex-M devices that can be plugged on a STM32L073Z_EVAL board. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 32 MHz (HSE + PLL).
 
 
Below you find the different items of demonstration :

 - Six icons are shown on the TFT LCD to propose the execution of 5 demonstrations + 1 help

 - LOW-POWER   : allows to enter in the low power modes(RUN, LPRUN, SLEEP, LPSLEEP, STOP, STANDBY) 
                 and the wakeup can be order by an EXTI or an alarm.
                 After wake-up, the current consumption measured by a companion chip (mfx) is displayed
 - THERMOMETER    : Displays the temperature (sensor Stlm75) in radiator thermostat way. Thanks to SMBUS,
                    a management of temperature setpoints tracking is done with interruption events which.
                    display messages and Min/Max temperature values.
                    - Requirements: the LCD glass(MB979) has to moved down in IO position on CN10 
                    and CN14 connectors for correct application behaviour.    
 - LPUART WAKE-UP : Demonstrates the wake-up of the platform from a terminal connected to CN7 (RS232)
                    in different modes. After wake-up, the current consumption measured by a companion
                     chip (mfx) during Stop mode is displayed.
                    - Requirements: a PC with hyperterminal software installed is needed and
                    correctly set according to instructions displayed on TFT screen (ie BaudRate = 9600,
                    Data = 7 bits, Parity = ODD, Stop = 1 bit, HwFlowCtl = NONE for recall).
                    Moreover, the LCD glass    (MB979) has to moved down in IO position on CN10 
                    and CN14 connectors. The jumper JP10 has to be moved on IDD position and
                    the jumper JP12 to +3V3 position for correct application behaviour.    

 - LC SENSOR METERING : This application shows how is managed the LC Sensor.Two modes are available : 
                        - Standard mode shows the LC sensor sensitivity to metal graphically on the display.
                        - Low Power mode minimizes power consumption of the detector to achieve years operation.
                        In both cases, metal strip must be placed over the LC sensor.  

 - PRESSURE  : Demonstrates the capability of ADC 16 bits oversampling mode from the ADC 12 bits available 
               in the STL32L073xx component. The current air pressure value is measured from the sensor (U1).
              
               - The first panel displays the curve of pressure = f(time).
               Each 200ms, a 12-bit A/D conversion is done and a black point is added.
               A 16-bit A/D conversion (oversampling) is also done and a red point is added.
               To modify the pressure, move up the board to 1.5 meters then move it down to the floor.

               - Press SEL to see the statistical distribution of 2 000 acqusitions 12-bit.
               The average value and the standard deviation are calculated for each series of measure.
               Min = average � standart deviation;
               Max = average + standart deviation;
               
               - Press SEL to see the statistical distribution of 2 000 acquisitions 16-bit (oversampling).
               The average value and the standard deviation are calculated for each series of measure.
               Min = average � standart deviation;
               Max = average + standart deviation;
               
               - Press SEL to display again the curve pressure = f(time).
               
               - Press DOWN at any time to exit the demo.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.


@par Hardware and Software environment

  - This example runs on STM32L073Z_EVAL devices..
    
  - This example has been tested with STM32L073Z_EVAL board and can be
    easily tailored to any other supported device and development board.      

  - A SDSC microSD card (capacity up to 4GB), must contain several.bmp files
    available under the FW package /Firmware/Utilities/Media/Pictures folder.


    Here is the list of the mandatory files which need to be copied into the SD card: 

     ---------------------------------------------------------------------------------------------- 
     |  FW package files to copy                                 |  Location inside SD card files |
     ----------------------------------------------------------------------------------------------
     | /Utilities/Media/Pictures/BMP_64x64/Help.bmp     |  STFILES/Help.bmp                       |
     | /Utilities/Media/Pictures/BMP_64x64/LCsensor.bmp |  STFILES/LCsensor.bmp                   |
     | /Utilities/Media/Pictures/BMP_64x64/Power.bmp    |  STFILES/Power.bmp                      |
     | /Utilities/Media/Pictures/BMP_64x64/Pressure.bmp |  STFILES/Pressure.bmp                   |
     | /Utilities/Media/Pictures/BMP_64x64/Temp.bmp     |  STFILES/Temp.bmp                       |
     | /Utilities/Media/Pictures/BMP_64x64/Uart.bmp     |  STFILES/Uart.bmp                       |
     | /Utilities/Media/Pictures/BMP_128x160/STLogo.bmp |  STFILES/STLogo.bmp                     |  
     ----------------------------------------------------------------------------------------------
 
    To do these copies, you must create a /STFILES folder at SD card's root directory, and copy the 
    different bmp files as described in the above table. 
    
    
  - On STM32L073Z-EVAL-RevB, the jumpers must be configured as described
    in this section. Starting from the top left position up to the bottom 
    right position, the jumpers on the Board must be set as follow:

     CN2:     ADC_IN5     [On] 
     CN3:     DAC_OUT     [On]
     JP2:     DAC_OUT_Det [Right position]
     JP3:     BL_BOOT     [Off]
     JP4:     VDDA        [On]
     JP5:     VREF        [On]
     JP6:     BL_RST      [Off]
     JP7:     VDD         [Right position]
     JP17:    DET_LCD     [Left position]
     JP15:    DET_LCD     [Left position]
     JP1:     DET_LCD     [Left position]
     JP8:                 [On]
     JP18:                [Off]
     JP9:     USART       [Left position]
     CN8:     EXT_LC      [Off]
     JP16:    DetLCD_1    [Off]
     JP14:    DetLCD_2    [Off]
     JP11:                3th position (starting from the left) upon the 4 available
     JP10:    IDD         [Down position]
     JP12:    +3V3        [Up position]
     JP13:    Power_ext   [Off]

 - On STM32L073Z-EVAL-RevB, the LCDGlass must be put in IO position.



@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
