#ifndef __MENU_GUI_H
#define __MENU_GUI_H
#include "main.h"
#include "gui.h"
#include "lcd.h"
#include "string.h"
//����
#define GAP 82
#define STORAGE 256
#define POINT 127
#define ADDNUMBERS 1
#define BASELINE 60
#define RATE 5
#define TRIGGER_UP 1
#define TRIGGER_DOWN 2

//ʱ��
#define RATE_1000MS 0
#define RATE_500MS 1
#define RATE_200MS 2
#define RATE_100MS 3
#define RATE_50MS  4
#define RATE_20MS  5
#define RATE_10MS  6
#define RATE_5MS   7
#define RATE_2MS   8
#define RATE_1MS   9

#define RATE_500US 10
#define RATE_200US 11
#define RATE_100US 12

//ʱ��������ʾλ��
#define TIMEBASE_X 90
#define TIMEBASE_Y 65
#define TIMEBASEEND_X 128

//������λ��
#define COOR_X 63

void SetCollectRate(int rate);
void VoltageToPoint(uint16_t voltage,uint8_t row,uint8_t column);
void DataProcess(void);
void ShowOnScreen(void);
void ClearPoints(void);
void GetPowerMag(void);
void PlotInterface(void);
void Amplitude(void);
#endif

