# 基于MSP430的频率特性测试仪

## 摘要

频率特性测试仪用于测量并显示被测网络的频率特性，本设计由一个正交扫频DDS信号源产生两路正交的正弦信号，正弦信号的频率为1kHz～1MHz，且步进频率可调。其中一路正弦信号作为待测网络的激励，待测网络的输出分别与原先的两路正交信号相乘，经过低通滤波器后即可得到直流分量，再经过ADS1115采样并由MSP430F5529计算待测网络的幅值和相频特性曲线。待测网络为截止频率分别为10kHz和100kHz的二阶有源高通和低通滤波器串联而成。

The spectrum characteristic tester is used to measure and display the frequency characteristics of the network under test. This design generates two orthogonal sinusoidal signals from a quadrature swept DDS signal source. The frequency of the sinusoidal signal is 1 kHz to 1 MHz, and the step frequency can be Tune. One of the sinusoidal signals is used as the excitation of the network to be tested. The output of the network to be tested is respectively multiplied by the original two orthogonal signals. After passing through the low-pass filter, the DC component can be obtained, and then sampled by ADS1115 and calculated by MSP430F5529. The amplitude and phase-frequency characteristics of the network. The network to be tested is a series of second-order active high-pass and low-pass filters with cutoff frequencies of 10 kHz and 100 kHz, respectively.

## 作品报告

![作品报告](https://github.com/ctlvie/Spectrum-Characteristic-Tester/wiki/%E5%9F%BA%E4%BA%8EMSP430%E7%9A%84%E9%A2%91%E7%8E%87%E7%89%B9%E6%80%A7%E6%B5%8B%E8%AF%95%E4%BB%AA)

## 程序清单

|文件名   | 功能描述     |
|-----------|------|
|main.c  | 主函数     |
| AD9854.c |DDS模块的驱动 |
|ADS1115.c|外接ADC模块的驱动|
|Board.c|MSP430F5529LP板上资源的配置|
|Button.c|四个按键的配置|
|FormatConvert.c|数据类型转换|
|GlobalTimer.c|定时器TimerB(记录测量用时)的配置|
|KeyBoard.c|外接4*4矩阵键盘的配置|
|LCD12864.c|液晶屏的驱动|
|ProcessModule.c|进行数据处理、计算、绘图|

## 系统结构

![系统结构](https://i.loli.net/2019/01/20/5c43d403dfee9.png)

## 实物图

![实物图](https://i.loli.net/2019/01/20/5c43d4616b4e6.jpg)

![曲线图](https://i.loli.net/2019/01/20/5c43d77f66409.jpg)
