//
// Created by Red_M_H on 2022/3/3.
//

#ifndef UW_USART_H
#define UW_USART_H

#include <string.h>
#include "usart.h"
#include "uwallinit.h"

#define RXBUFFERSIZE  256

struct USART_
{
    bool flag; //串口使能，但感觉有点问题
    uint8_t RxBuffer[RXBUFFERSIZE]; //char -> uint8_t 2022.5.15
    uint8_t aRxBuffer;
    uint8_t Usart_Rx_Cnt;
    uint8_t i; //需要for循环整合数据时使用
    uint16_t data; //接收整合数据
};

extern struct USART_ u1,u2,u3,u4,u5;

#endif /* __USART_H__ */
