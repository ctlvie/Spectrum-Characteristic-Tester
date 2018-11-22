
#ifndef AD9854_H_
#define AD9854_H_
#define  uclong   unsigned long
#define  Uchar    unsigned char
#define LongToBin(n) (((n>>21)&0x80)|((n>>18)&0x40)|((n>>15)&0x20)|((n>>12)&0x10)|((n>>9)&0x08)|((n>>6)&0x04)|((n>>3)&0x02)|((n)&0x01)) 
#define Bin(n) LongToBin(0x##n##l)

/*=======================================================
    接线方式:
    1. CS   -> 
    2. SCLK    -> 
    3. UPDATE  -> 
    4. SDIO  -> 
    5. SDO -> 
    6. RESET -> 
    7. Serial ->
    8. OSC
========================================================*/


//******************************************************************
//端口操作

//第二版硬件
#define AD9854_CS_UP       P1OUT |= BIT6
#define AD9854_CS_DOWN     P1OUT &=~BIT6   	//片选
#define AD9854_SCLK_UP     P2OUT |= BIT6
#define AD9854_SCLK_DOWN   P2OUT &=~BIT6	//数据时钟
#define AD9854_UPDATE_UP   P2OUT |= BIT7
#define AD9854_UPDATE_DOWN P2OUT &=~BIT7	//更新时钟
#define AD9854_UPDATE_OUT   P2DIR |= BIT7
#define AD9854_UPDATE_IN 	P2DIR &=~BIT7	//更新时钟方向
#define AD9854_SDIO_UP     P2OUT |= BIT0
#define AD9854_SDIO_DOWN   P2OUT &=~BIT0	//数据输出
#define AD9854_SDO_IN      (P2IN & 0x02) 	//数据输入
#define AD9854_IO_RESET_UP    P2OUT |= BIT2
#define AD9854_IO_RESET_DOWN  P2OUT &=~BIT2	//SPI总线复位
#define AD9854_RESET_UP    P1OUT |= BIT7
#define AD9854_RESET_DOWN  P1OUT &=~BIT7   	//主复位
#define AD9854_IO_parallel //P6OUT |= BIT7
#define AD9854_IO_serial   //P6OUT &=~BIT7   	//串并选择
#define AD9854_OSC_ON    	P5OUT |= BIT6	
#define AD9854_OSC_OFF  	P5OUT &=~BIT6	//晶振控制
#define  HARDWARE_AD9854   P1DIR |= BIT6+BIT7;P2DIR |= BIT0+BIT2+BIT6;P5DIR |= BIT6;P5OUT &=~BIT6 //AD9854_OSC_OFF


////////////////////////////////////////////////////////////////////
//                                                                //
//                             INSTRUCTION BYTE                   //
//                                                                //
////////////////////////////////////////////////////////////////////

//******************************************************************
//寄存器地址 注意是串行地址
#define	  AD9854_Addr_PHA1       0        	// 2 Bytes
#define	  AD9854_Addr_PHA2       1        	// 2 Bytes 
#define	  AD9854_Addr_FRE1       2        	// 6 Bytes	
#define	  AD9854_Addr_FRE2       3        	// 6 Bytes
#define	  AD9854_Addr_DELTA      4        	// 6 Bytes
#define	  AD9854_Addr_UPDATA     5        	// 4 Bytes
#define	  AD9854_Addr_RAMP_CLK   6        	// 3 Bytes
#define	  AD9854_Addr_CTR_REG    7        	// 4 Bytes
#define	  AD9854_Addr_I_MUL      8        	// 2 Bytes
#define	  AD9854_Addr_Q_MUL      9        	// 2 Bytes
#define	  AD9854_Addr_SHAPED     10       	// 1 Bytes
#define	  AD9854_Addr_Q_DAC      11        	// 2 Bytes
//******************************************************************

//******************************************************************
//寄存器长度
#define	  AD9854_Length_PHA1       2        	// 2 Bytes
#define	  AD9854_Length_PHA2       2        	// 2 Bytes 
#define	  AD9854_Length_FRE1       6        	// 6 Bytes	
#define	  AD9854_Length_FRE2       6        	// 6 Bytes
#define	  AD9854_Length_DELTA      6        	// 6 Bytes
#define	  AD9854_Length_UPDATA     4        	// 4 Bytes
#define	  AD9854_Length_RAMP_CLK   3        	// 3 Bytes
#define	  AD9854_Length_CTR_REG    4        	// 4 Bytes
#define	  AD9854_Length_I_MUL      2        	// 2 Bytes
#define	  AD9854_Length_Q_MUL      2        	// 2 Bytes
#define	  AD9854_Length_SHAPED     1       		// 1 Bytes
#define	  AD9854_Length_Q_DAC      2        	// 2 Bytes

////////////////////////////////////////////////////////////////////
//                                                                //
//                          VARIABLES   DEFINITION                //
//                                                                //
////////////////////////////////////////////////////////////////////

#define  FREQUENCY  8000     // MCLK的频率为8000KHz ,即1秒钟执行8M个时钟周期
#define  LOOPBODY  8         //MSP430中一个for循环体耗费8个周期
#define  LOOPCNT ( unsigned int )  (( FREQUENCY/LOOPBODY ))

void DELAY_AD9854_MS (unsigned int ms);
Uchar sendByte_AD9854(Uchar dat);
void writeSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count);
void readSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count);
void configAD9854(void);
void resetAD9854(void);
void initAD9854(void);
void writeAD9854_Freq1(void);
void writeAD9854_FreqSW(void);

#endif /* AD9854_H_ */
