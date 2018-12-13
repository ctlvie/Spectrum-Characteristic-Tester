/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ADS1115.h
Date				  :			    2018-11-20
Description			  :				???????

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181120		ctlvie		1.0			float????
========================================================*/

#ifndef FORMATCONVERT_H_
#define FORMATCONVERT_H_


void recurParse(long n, unsigned char** str, unsigned long* restSize);
void convertFloattoCharArray(unsigned char* outputChar, unsigned long bufferSize, float inputFloart, unsigned long decCount);
int convertCharArraytoInt(unsigned char Array[], int Bits);
void convertInttoCharArray(unsigned char* outputChar, unsigned int InputInt, unsigned int size);
float abs_f(float x);
float SqrtByNewton(float x);
float Arctan(float input);
int getNearIndex(float value,float array[],int size);
#endif /* FORMATCONVERT_H_ */
