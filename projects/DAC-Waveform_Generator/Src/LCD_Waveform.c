#include "LCD_Waveform.h"

void MyLCD_Init() {
    BSP_LCD_Init();
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

    BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(0, LINE(3), "Waveform Generator", CENTER_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, LINE(8), "Press any key to begin", CENTER_MODE);
}

void MyLCD_ClearDiagram() {
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(0, 40, 320, 200);
}


void MyLCD_DrawPixelThick(uint16_t Xpos, uint16_t Ypos, uint16_t amount) {
    for (int x = Xpos - amount; x < Xpos + amount; x++) {
        for (int y = Ypos - amount; y < Ypos + amount; y++) {
            BSP_LCD_DrawPixel(x, y);
        }
    }

}

void MyLCD_DrawLineThick(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t amount) {
    for (int x = -amount; x < amount; x++) {
        for (int y = -amount; y < +amount; y++) {
            BSP_LCD_DrawLine(X1 + x, Y1 + y, X2 + x, Y2 + y);
        }
    }
}

void MyLCD_DrawSineWave(int color_diagram) {
    BSP_LCD_SetTextColor(color_diagram);

    const int width = 125;
    const int height = 50;
    const int x_pos = 320 / 2 - width / 2;
    const int y_pos = 240 / 2;

    const float step = 2*M_PI / width / 2;

    for (float t = 0; t < 2*M_PI; t += step) {
        int y = sinf(t) * height + y_pos;
        int x = x_pos + (t * 0.5 * M_1_PI) * width;
        MyLCD_DrawPixelThick(x, y, 1);
    }
}

void MyLCD_DrawTriangleWave(int color_diagram) {
    BSP_LCD_SetTextColor(color_diagram);

    const int width = 125;
    const int height = 50;
    int x_pos = 320 / 2 - width / 2;
    const int y_pos = 240 / 2;

    MyLCD_DrawLineThick(
            x_pos,
            y_pos,
            x_pos + width/4,
            y_pos + height,
            1);

    MyLCD_DrawLineThick(
            x_pos + width/4,
            y_pos + height,
            x_pos + width*3/4,
            y_pos - height,
            1);

    MyLCD_DrawLineThick(
                x_pos + width*3/4,
                y_pos - height,
                x_pos + width,
                y_pos,
                1);
}

void MyLCD_DrawSawtoothWave(int color_diagram) {
    BSP_LCD_SetTextColor(color_diagram);

    const int width = 125;
    const int height = 30;

    int x_mid = 320 / 2;
    int x_left = x_mid - width / 2;
    int x_right = x_mid + width / 2;

    int y_min = 240 / 2 + height;
    int y_max = 240 / 2 - height;

    MyLCD_DrawLineThick(
            x_left,
            y_min,
            x_mid,
            y_max,
            1);

    MyLCD_DrawLineThick(
            x_mid,
            y_max,
            x_mid,
            y_min,
            1);

    MyLCD_DrawLineThick(
            x_mid,
            y_min,
            x_right,
            y_max,
            1);

    MyLCD_DrawLineThick(
            x_right,
            y_max,
            x_right,
            y_min,
            1);
}

void MyLCD_HeaderText(uint8_t *pText, int color_text) {
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_SetTextColor(color_text);
    BSP_LCD_DisplayStringAt(0, LINE(1), pText, CENTER_MODE);
}


void MyLCD_FooterText(uint8_t *pText, int color_text) {
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_SetTextColor(color_text);
    BSP_LCD_DisplayStringAt(0, LINE(10), pText, CENTER_MODE);
}


