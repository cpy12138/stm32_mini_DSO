#include "stm32_dsp.h"
#include "math.h"
#include "menu_GUI.h"
uint32_t FFT_SourceData[STORAGE] = {0};	//fft输入序列
uint32_t FFT_OutData[STORAGE] = {0};		//fft输出序列
uint32_t FFT_Mag[STORAGE/2] = {0};		//幅频特性序列
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