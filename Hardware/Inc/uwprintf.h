//
// Created by Red_M_H on 2022/3/2.
//

#ifndef UW_PRINTF_H
#define UW_PRINTF_H

#include <stdarg.h>
#include "usart.h"

#define abs_(x) (x>0?x:-x)

void putchar_(UART_HandleTypeDef *huart,const char ch);
void putint_(UART_HandleTypeDef *huart,const int dec);
void putstr_(UART_HandleTypeDef *huart,const char *ptr);
uint32_t pow_(uint8_t power);
void putfloat_(UART_HandleTypeDef *huart,const float flt,uint8_t dp);
void printf_(UART_HandleTypeDef *huart,const char *format,...);

#endif //TEST_MYPRINTF_H
