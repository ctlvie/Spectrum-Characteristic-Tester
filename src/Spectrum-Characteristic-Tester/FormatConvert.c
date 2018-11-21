/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ADS1115.c
Date				  :			    2018-11-20
Description			  :				???????

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181120		ctlvie		1.0			float????
========================================================*/

#include <msp430f5529.h>
#include"FormatConvert.h"

#include <stdio.h>

void recurParse(int n, unsigned char** str, unsigned int* restSize) {
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
//Name:         void convertFloattoCharArray(char* outputChar, unsigned int bufferSize, float inputFloart, unsigned int decCount)
//Description:  float??????
//Input:        outputChar: ?????????
//              bufferSize: ???????????
//              inputFloart: ???????
//              decCount: ????????????
//Output:       ?
//------------------------------------------------- 

void convertFloattoCharArray(unsigned char* outputChar, unsigned int bufferSize, float inputFloart, unsigned int decCount) {
        int n;
        float dec;
        unsigned int restSize = bufferSize;
        //if (restSize == 0) return 0;
        n = (int)inputFloart;
        dec = inputFloart - n;
        if (inputFloart < 0) {
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
                n = (int)dec;
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
