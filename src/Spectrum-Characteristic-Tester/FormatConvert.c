/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ADS1115.h
Date				  :			    2018-11-20
Description			  :				格式转换处理

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181120	     ctlvie		1.0			功能实现
181216      ctlvie      2.0         Final Version
========================================================*/

#include <msp430f5529.h>
#include"FormatConvert.h"
#include<math.h>
#include <stdio.h>

void recurParse(long n, unsigned char** str, unsigned long* restSize) {
        if (n < 10) {
                if (*restSize > 1) {
                        **str = n + '0';
                        (*str)++;
                        *restSize -= 1;
                }
                
        }
        else {
                recurParse(n / 10, str, restSize);
                if (*restSize > 1) {
                        **str = n % 10 + '0';
                        (*str)++;
                        *restSize -= 1;
                }
        }
}


//-------------------------------------------------
//Name:         void convertFloattoCharArray(char* outputChar, unsigned int bufferSize, float inputFloat, unsigned int decCount)
//Description:  float??????
//Input:        outputChar: ?????????
//              bufferSize: ???????????
//              inputFloat: ???????
//              decCount: ????????????
//Output:       ?
//------------------------------------------------- 

void convertFloattoCharArray(unsigned char* outputChar, unsigned long bufferSize, float inputFloat, unsigned long decCount) {
        long n;
        float dec;
        unsigned long restSize = bufferSize;
        //if (restSize == 0) return 0;
        n = (long)inputFloat;
        dec = inputFloat - (float)n;
        if (inputFloat < 0) {
                if (restSize > 1 && !(n == 0 && decCount == 0)) {
                        *outputChar++ = '-';
                        --restSize;
                }
                n = -n;
        }
        if (dec < 0) {
                dec = -dec;
        }
        recurParse(n, &outputChar, &restSize);
        if (decCount != 0) {
                if (restSize > 1) {
                        *outputChar++ = '.';
                        --restSize;
                }
        }
        while (decCount) {
                dec *= 10;
                n = (long)dec;
                dec -= n;
                if (restSize > 1) {
                        *outputChar++ = n + '0';
                        --restSize;
                }
                else {
                        break;
                }
                decCount -= 1;
        }
        *outputChar++ = NULL;
        //return bufferSize - restSize;
}

int convertCharArraytoInt(unsigned char Array[], int Bits)
{
	int Result = 0;
	int i = 0, j = 0;
	int times = 0;
	for(i = Bits - 1; i >= 0 ; i--)
	{
		times = 1;
		for(j = 0 ; j < (Bits - i - 1); j ++)
			times = times * 10;
		Result += (Array[i] - '0')* times;
	}
	return Result;
}

void convertInttoCharArray(unsigned char* outputChar, unsigned int InputInt, unsigned int size)
{
  unsigned int i = 0;
  i = size - 1;
   while(InputInt)
   {
     outputChar[i] = InputInt % 10 + '0';
     i--;
     InputInt /= 10;
   }
}


float abs_f(float x)
{
	float i;
	i=x;
	if(i<0)
		i=-i;
	else
		i=i;
	return i;
}

float SqrtByNewton(float x)
{
	float val = x;//最终
	float last;//保存上一个计算的值
 	do
 	{
		last = val;
		val =(val + x/val) / 2;
	}
	while(abs_f(val-last) > 0.01); //精度控制
	return val;
}

float Arctan(float input)
{
	float output;
	output = (float)atan((double)input);
	output = output * 180 / 3.1415;
	return output;
}

int getNearIndex(float value,float array[],int size)
{
    int ret_index;                        /* 结果索引 */
    int mid_index;                        /* 中位游标 */
    int left_index;                       /* 左位游标 */
    int right_index;                      /* 右位游标 */
    float left_abs;                       /* 左位的值与目标值之间的差的绝对值 */
    float right_abs;                      /* 右位的值与目标值之间的差的绝对值 */

    ret_index = 0;
    left_index = 0;
    right_index = size - 1;    
    mid_index = 0;
    left_abs = 0;
    right_abs = 0;

    while(left_index != right_index){
        mid_index = (right_index + left_index) / 2;
        if (value <= array[mid_index]) {
            right_index = mid_index;
        } else {
            left_index = mid_index;
        }
        if (right_index - left_index < 2) {
            break;
        }
    }

    left_abs = fabs(array[left_index] - value);
    right_abs = fabs(array[right_index] - value);
    ret_index = right_abs <= left_abs ? right_index : left_index;

    return ret_index;
}
