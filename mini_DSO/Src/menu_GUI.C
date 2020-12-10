#include "menu_GUI.h"
#include "stdio.h"
#include "stm32_dsp.h"
#include "math.h"

extern uint16_t DataBuffer[STORAGE];
extern int value;
uint16_t PointBuffer[POINT];
uint8_t ex_PointBuffer[POINT+1];
int n=0;
int start = 0;
uint32_t FFT_SourceData[STORAGE] = {0};	//fft输入序列
uint32_t FFT_OutData[STORAGE] = {0};		//fft输出序列
uint32_t FFT_Mag[STORAGE/2] = {0};		//幅频特性序列
float tempMAX = 0;
float tempMIN = 2000;
//屏幕显示
void ShowOnScreen(void)
{
	int i;
	int j = 0;
	int temp = 0;
//	cr4_fft_256_stm32(FFT_OutData,(uint32_t* )DataBuffer,STORAGE);
//	GetPowerMag();
//	for(i=0; i<STORAGE/2; i++)
//	{
//		if(temp < FFT_Mag[i])
//		{
//			temp = FFT_Mag[i];
//			j=i;
//		}
//	}
	for(i=0; i<POINT; i++)
	{
		GUI_DrawLine(i, 0, i, 61, WHITE);
		VoltageToPoint(DataBuffer[i+start],BASELINE,i);
	}
	LCD_Fill(0,TIMEBASE_Y,50,80,WHITE);
	Amplitude();
	LCD_ShowFloatNum(1,65,tempMIN,4,12);
	GUI_DrawLine(23,70,27,70,BLACK);
	LCD_ShowFloatNum(30,65,tempMAX,4,12);
	Show_Str(51,61,BLACK,WHITE,"v",1,1);
	memset(DataBuffer,0,sizeof(DataBuffer));
	
	n=0;
	start = 0;
	temp = 0;

}

//修改采样率
void SetCollectRate(int rate)
{
	switch(rate)
	{
		case RATE_1000MS: TIM3->PSC = 89; TIM3->ARR = 7199; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,1000,4,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_500MS: TIM3->PSC = 39; TIM3->ARR = 7199; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,500,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_200MS: TIM3->PSC = 15; TIM3->ARR = 7199; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,200,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_100MS: TIM3->PSC = 8; TIM3->ARR = 7199; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,100,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_50MS: TIM3->PSC = 3; TIM3->ARR = 7199; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,50,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_20MS: TIM3->PSC = 15; TIM3->ARR = 719; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,20,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_10MS: TIM3->PSC = 8; TIM3->ARR = 719; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,10,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_5MS: TIM3->PSC = 3; TIM3->ARR = 719; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,5,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_2MS: TIM3->PSC = 15; TIM3->ARR = 71; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,2,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		case RATE_1MS: TIM3->PSC = 1; TIM3->ARR = 280; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,1,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"ms",2,1);break;
		
		case RATE_500US: TIM3->PSC = 3; TIM3->ARR = 71; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,500,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"us",2,1);break;
		case RATE_200US: TIM3->PSC = 15; TIM3->ARR = 6; 
		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,200,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"us",2,1);break;
//		case RATE_100US: TIM3->PSC = 11; TIM3->ARR = 7; 
//		LCD_Fill(TIMEBASE_X,TIMEBASE_Y,TIMEBASEEND_X,100,WHITE); LCD_ShowNum(TIMEBASE_X,TIMEBASE_Y,100,3,12); Show_Str(110,TIMEBASE_Y-4,BLACK,WHITE,"us",2,1);break;
	}
	
}

//电压->像素点
void VoltageToPoint(uint16_t voltage,uint8_t row,uint8_t column)
{
	int i=0;
	int m=0;
	
	for(;i>=0;i++)
	{
		if((int)voltage >= m && (int)voltage < (m+GAP))
			break;
		else
			m+=GAP;
	}
	//上次的像素值
	ex_PointBuffer[n] = row-i;
	if(n>0)
	{
		GUI_DrawLine(column-1,ex_PointBuffer[n-1],column,ex_PointBuffer[n],BLACK);
	}
//	GUI_DrawPoint(column,row-i,BLACK);
	n++;
}


int trigger = TRIGGER_UP;		//触发方式
int trigger_VOL = 500; //触发电平
int j = 0;
void DataProcess(void)
{
	int i;
	//获取触发方式
	//上升沿触发
	if(trigger == TRIGGER_UP)
	{
		//存储深度300,STORAGE-173是为了保证后面的126位都有效
		for(i=0; i<POINT; i++)
		{
			if(((int)DataBuffer[i] <= trigger_VOL) && ((int)DataBuffer[i+1] > trigger_VOL))
			{
				start = i;
				j=1;
				break;
			}
		}
	}
	//下降沿触发
	else
	{
		
	}
	
	
	//
}
//绘制波形
//void Plot()
//{
//	
//}
//清空像素点
void ClearPoints(void)
{
	int i;
	for(i=0; i<POINT; i++)
	GUI_DrawPoint(i,ex_PointBuffer[i],WHITE);
	n=0;
}


void GetPowerMag(void)
{
	signed short lX,lY;
	float X,Y,Mag;
	unsigned short i;

	for(i=0; i<STORAGE/2; i++)
	{
		lX = (FFT_OutData[i] << 16) >> 16;
		lY = (FFT_OutData[i] >> 16);
		
		X = STORAGE * ((float)lX) / 32768;
		Y = STORAGE * ((float)lY) / 32768;
		
		Mag = sqrt(X * X + Y * Y) / STORAGE;
		
		if(i == 0)
			FFT_Mag[i] = (unsigned long)(Mag * 32768);
		else
			FFT_Mag[i] = (unsigned long)(Mag * 65536);
		//printf("%ld\r\n",lBufMagArray[i]);
	}
	//printf("\r\n\r\n");
}

void PlotInterface(void)
{
	GUI_DrawLine(0,COOR_X,POINT,COOR_X,BLACK);
	GUI_DrawPoint(31,COOR_X-1,BLACK);
	GUI_DrawPoint(63,COOR_X-1,BLACK);
	GUI_DrawPoint(95,COOR_X-1,BLACK);
	GUI_DrawPoint(127,COOR_X-1,BLACK);
}

//获取赋值

void Amplitude(void)
{
	int i = 0;
	tempMIN = DataBuffer[0];
	tempMAX = DataBuffer[0];
	for(i=0; i<STORAGE; i++)
	{
		if(tempMAX<DataBuffer[i])
		{
			tempMAX = DataBuffer[i];
		}
		if(tempMIN>DataBuffer[i])
		{
			tempMIN = DataBuffer[i];
		}
	}
	tempMIN = (float)tempMIN*3.3/4096;
	tempMAX = (float)tempMAX*3.3/4096;
}

