#include "uwprintf.h"

void putchar_(UART_HandleTypeDef *huart, const char ch)
{
    char temp = ch;
    HAL_UART_Transmit(huart, (uint8_t *)&temp, 1, 0xffff);
    while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);
}

//void putint_(UART_HandleTypeDef *huart,const int dec)
//{
//    if(dec==0) return;
//    putint_(huart,dec/10);
//    if(dec<0&&dec>-10) putchar_(huart,'-');
//    putchar_(huart,abs_(dec)%10+'0');
//}

void putint_(UART_HandleTypeDef *huart, const int dec)
{
    uint8_t nod;
    if(dec < 0) putchar_(huart, '-');
    for(nod = 1; abs_(dec) % pow_(nod) != abs_(dec); nod++);
    for(; nod != 0; nod--) putchar_(huart, abs_(dec) / pow_(nod - 1) % 10 + '0');
}

void putstr_(UART_HandleTypeDef *huart, const char *ptr)
{
    while(*ptr)
    {
        putchar_(huart, *ptr);
        ptr++;
    }
    //HAL_UART_Transmit(huart, (uint8_t *)ptr, strlen(ptr), 0xffff);
    //while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);
}

uint32_t pow_(uint8_t power)
{
    uint32_t temp;

    switch(power)
    {
        case 0:temp = 1;break;
        case 1:temp = 10;break;
        case 2:temp = 100;break;
        case 3:temp = 1000;break;
        case 4:temp = 10000;break;
        case 5:temp = 100000;break;
        case 6:temp = 1000000;break;
        case 7:temp = 10000000;break;
        case 8:temp = 100000000;break;
        case 9:temp = 1000000000;break;
    }

    return temp;
}

void putfloat_(UART_HandleTypeDef *huart, const float flt, uint8_t dp)
{
    if(flt < 0) putchar_(huart, '-');
    putint_(huart, abs_((int)flt));
    putchar_(huart, '.');
    for(uint8_t i=0;i<dp;i++) putchar_(huart,abs_((int)(flt*pow_(i+1)))%10 + '0');
}

void printf_(UART_HandleTypeDef *huart, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    for(; *format != '\0'; format++)
    {
        if(*format != '%')
        {
            putchar_(huart, *format);
        }
        else
        {
            format++;

            switch(*format)
            {
                case 'c':
                {
                    char valchar = va_arg(ap, int);
                    putchar_(huart, valchar);
                    break;
                }

                case 'd':
                {
                    int valint = va_arg(ap, int);
                    putint_(huart, valint);
                    break;
                }

                case 's':
                {
                    char *valstr = va_arg(ap, char *);
                    putstr_(huart, valstr);
                    break;
                }

                case 'f':
                {
                    float valfloat = va_arg(ap, double);
                    putfloat_(huart, valfloat, 2); //默认保留2位小数，无四舍五入
                    break;
                }

                default:
                {
                    putchar_(huart, *format);
                }
            }
            //va_end(ap);
        }
    }
    va_end(ap);
}
