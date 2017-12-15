#pragma once

#include "stm32l073z_eval_lcd.h"
#include "math.h"

void MyLCD_Init();

void MyLCD_ClearDiagram();

void MyLCD_DrawPixelThick(uint16_t Xpos, uint16_t Ypos, uint16_t amount);

void MyLCD_DrawLineThick(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t amount);

void MyLCD_DrawSineWave(int color_diagram);

void MyLCD_DrawTriangleWave(int color_diagram);

void MyLCD_FooterText(uint8_t *pText, int color_text);

void MyLCD_HeaderText(uint8_t *pText, int color_text);

