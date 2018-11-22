
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
    8. IORESET -> 
========================================================*/


//******************************************************************
//端口操作
#define IO_AD9854_1_OUT     P8OUT
#define IO_AD9854_1_DIR     P8DIR
#define IO_AD9854_2_OUT     P4OUT
#define IO_AD9854_2_DIR     P4DIR
#define IO_AD9854_2_IN      P4IN
#define IO_AD9854_3_OUT     P7OUT
#define IO_AD9854_3_DIR     P7DIR

#define IO_AD9854_1_BIT_CS      BIT6
#define IO_AD9854_1_BIT_RESET   BIT7
#define IO_AD9854_2_BIT_SCLK    BIT6
#define IO_AD9854_2_BIT_UPDATE  BIT7
#define IO_AD9854_2_BIT_SDIO    BIT0
#define IO_AD9854_2_BIT_SDO    BIT1
#define IO_AD9854_2_BIT_IORESET BIT2
#define IO_AD9854_3_BIT_OSC     BIT6


//第二版硬件
#define AD9854_CS_UP       IO_AD9854_1_OUT |= IO_AD9854_1_BIT_CS
#define AD9854_CS_DOWN     IO_AD9854_1_OUT &=~IO_AD9854_1_BIT_CS   	//片选
#define AD9854_SCLK_UP     IO_AD9854_2_OUT |= IO_AD9854_2_BIT_SCLK
#define AD9854_SCLK_DOWN   IO_AD9854_2_OUT &=~IO_AD9854_2_BIT_SCLK	//数据时钟
#define AD9854_UPDATE_UP   IO_AD9854_2_OUT |= IO_AD9854_2_BIT_UPDATE
#define AD9854_UPDATE_DOWN IO_AD9854_2_OUT &=~IO_AD9854_2_BIT_UPDATE	//更新时钟
#define AD9854_UPDATE_OUT   IO_AD9854_2_DIR |= IO_AD9854_2_BIT_UPDATE
#define AD9854_UPDATE_IN 	IO_AD9854_2_DIR &=~IO_AD9854_2_BIT_UPDATE	//更新时钟方向
#define AD9854_SDIO_UP     IO_AD9854_2_OUT |= IO_AD9854_2_BIT_SDIO
#define AD9854_SDIO_DOWN   IO_AD9854_2_OUT &=~IO_AD9854_2_BIT_SDIO	//数据输出
#define AD9854_SDO_IN      (IO_AD9854_2_IN & IO_AD9854_2_BIT_SDO) 	//数据输入
#define AD9854_IO_RESET_UP    IO_AD9854_2_OUT |= IO_AD9854_2_BIT_IORESET
#define AD9854_IO_RESET_DOWN  IO_AD9854_2_OUT &=~IO_AD9854_2_BIT_IORESET	//SPI总线复位
#define AD9854_RESET_UP    IO_AD9854_1_OUT |= IO_AD9854_1_BIT_RESET
#define AD9854_RESET_DOWN  IO_AD9854_1_OUT &=~IO_AD9854_1_BIT_RESET   	//主复位
#define AD9854_IO_parallel //P6OUT |= BIT7
#define AD9854_IO_serial   //P6OUT &=~BIT7   	//串并选择
#define AD9854_OSC_ON    	IO_AD9854_3_OUT |= IO_AD9854_3_BIT_OSC	
#define AD9854_OSC_OFF  	IO_AD9854_3_OUT &=~IO_AD9854_3_BIT_OSC	//晶振控制
#define  HARDWARE_AD9854   IO_AD9854_1_DIR |= IO_AD9854_1_BIT_CS+IO_AD9854_1_BIT_RESET; IO_AD9854_2_DIR |= IO_AD9854_2_BIT_SDIO+IO_AD9854_2_BIT_IORESET+IO_AD9854_2_BIT_SCLK; IO_AD9854_3_DIR |= IO_AD9854_3_BIT_OSC;IO_AD9854_3_OUT &=~IO_AD9854_3_BIT_OSC //AD9854_OSC_OFF


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
