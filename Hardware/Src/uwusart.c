#include "uwusart.h"
#include "uwallinit.h"
#include "uwprintf.h"

struct USART_ u1, u2, u3, u4, u5; //5串口定义

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);

    /* NOTE: This function Should not be modified, when the callback is needed,
             the HAL_UART_TxCpltCallback could be implemented in the user file
     */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if((huart->Instance == USART1) && u1.flag)
    {
        //printf_(&huart5, "\xFA\xDE\x0A");
        //HAL_UART_Transmit(&huart5, (uint8_t *)&u1.aRxBuffer, 1,0xFFFF); //将收到的信息发送出去
        //while(HAL_UART_GetState(&huart5) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束

        if (u1.Usart_Rx_Cnt >= 255)
        {
            u1.Usart_Rx_Cnt = 0;
            memset(u1.RxBuffer, 0x00, sizeof(u1.RxBuffer));
            //HAL_UART_Transmit(&huart1, (uint8_t *) "数据溢出", 10, 0xFFFF);
        }
        else
        {
            u1.RxBuffer[u1.Usart_Rx_Cnt++] = u1.aRxBuffer;

            if ((u1.RxBuffer[0] == 0xAA) && (u1.RxBuffer[u1.Usart_Rx_Cnt - 1] == 0xBB))
            {
                u1.Usart_Rx_Cnt = 0;
                memset(u1.RxBuffer, 0x00, sizeof(u1.RxBuffer));
            }
        }

        HAL_UART_Receive_DMA(&huart1, (uint8_t *) &u1.aRxBuffer, 1);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    if((huart->Instance == USART2) && u2.flag)
//    {
//        if (u2.Usart_Rx_Cnt >= 255)
//        {
//            u2.Usart_Rx_Cnt = 0;
//            memset(u2.RxBuffer, 0x00, sizeof(u2.RxBuffer));
//            //HAL_UART_Transmit(&huart2, (uint8_t *) "数据溢出", 10, 0xFFFF);
//        }
//        else
//        {
//            u2.RxBuffer[u2.Usart_Rx_Cnt++] = u2.aRxBuffer;

//            if ((u2.RxBuffer[u2.Usart_Rx_Cnt - 1] == 0x0A) && (u2.RxBuffer[u2.Usart_Rx_Cnt - 2] == 0x0D))
//            {
//                if(u2.RxBuffer[9]=='-') depth = -((u2.RxBuffer[12]-'0')*0.1f + (u2.RxBuffer[13]-'0')*0.01f);
//                else if(u2.RxBuffer[9]=='0') depth = (u2.RxBuffer[11]-'0')*0.1f + (u2.RxBuffer[12]-'0')*0.01f;
//                else if(u2.RxBuffer[9]=='1') depth = 1 + (u2.RxBuffer[11]-'0')*0.1f + (u2.RxBuffer[12]-'0')*0.01f;
//                else u2_error = true;

//                depth -= depth_error; //整合误差后转换为深度相对值

//                u2.Usart_Rx_Cnt = 0;
//                memset(u2.RxBuffer, 0x00, sizeof(u2.RxBuffer));
//            }
//        }

//        HAL_UART_Receive_DMA(&huart2, (uint8_t *) &u2.aRxBuffer, 1);
//    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    if((huart->Instance == USART3) && u3.flag)
//    {
//        if (u3.Usart_Rx_Cnt >= 255)
//        {
//            u3.Usart_Rx_Cnt = 0;
//            memset(u3.RxBuffer, 0x00, sizeof(u3.RxBuffer));
//            //HAL_UART_Transmit(&huart3, (uint8_t *) "数据溢出", 10, 0xFFFF);
//        }
//        else
//        {
//            u3.RxBuffer[u3.Usart_Rx_Cnt++] = u3.aRxBuffer;

//            if ((u3.RxBuffer[u3.Usart_Rx_Cnt - 1] == 0x0A) && (u3.RxBuffer[u3.Usart_Rx_Cnt - 2] == 0x0D))
//            {                
//                u3.Usart_Rx_Cnt = 0;
//                memset(u3.RxBuffer, 0x00, sizeof(u3.RxBuffer));
//            }
//        }

//        HAL_UART_Receive_IT(&huart3, (uint8_t *) &u3.aRxBuffer, 1);
//    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    if((huart->Instance == UART4) && u4.flag)
//    {
//        if (u4.Usart_Rx_Cnt >= 255)
//        {
//            u4.Usart_Rx_Cnt = 0;
//            memset(u4.RxBuffer, 0x00, sizeof(u4.RxBuffer));
//            //HAL_UART_Transmit(&huart4, (uint8_t *) "数据溢出", 10, 0xFFFF);
//        }
//        else
//        {
//            u4.RxBuffer[u4.Usart_Rx_Cnt++] = u4.aRxBuffer;

//            if ((u4.RxBuffer[0] == 0x55) && (u4.RxBuffer[1] == 0x53) && (u4.Usart_Rx_Cnt >= 11))
//            {
//                roll = ((short)(u4.RxBuffer[3] << 8 | u4.RxBuffer[2])) / 32768.0f * 180;
//                pitch = ((short)(u4.RxBuffer[5] << 8 | u4.RxBuffer[4])) / 32768.0f * 180;
//                yaw = ((short)(u4.RxBuffer[7] << 8 | u4.RxBuffer[6])) / 32768.0f * 180;

//                u4.Usart_Rx_Cnt = 0;
//                memset(u4.RxBuffer, 0x00, sizeof(u4.RxBuffer));
//            }
//            else if ((u4.RxBuffer[0] != 0x55))
//            {
//                u4.Usart_Rx_Cnt = 0;
//                u4.RxBuffer[0] = 0;
//            }
//        }

//        HAL_UART_Receive_IT(&huart4, (uint8_t *) &u4.aRxBuffer, 1);
//    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    if((huart->Instance == UART5) && u5.flag)
//    {
//        if (u5.Usart_Rx_Cnt >= 255)
//        {
//            u5.Usart_Rx_Cnt = 0;
//            memset(u5.RxBuffer, 0x00, sizeof(u5.RxBuffer));
//            //HAL_UART_Transmit(&huart5, (uint8_t *) "数据溢出", 10, 0xFFFF);
//        }
//        else
//        {
//            u5.RxBuffer[u5.Usart_Rx_Cnt++] = u5.aRxBuffer;

//            if ((u5.RxBuffer[u5.Usart_Rx_Cnt - 1] == 0x0A) && (u5.RxBuffer[u5.Usart_Rx_Cnt - 2] == 0x0D))
//            {
//                switch(u5.RxBuffer[0])
//                {
//                    case '1':
//                        if(u5.RxBuffer[1] == '+') EM1 += bluetooth_change;
//                        else EM1 -= bluetooth_change;

//                        break;

//                    case '2':
//                        if(u5.RxBuffer[1] == '+') EM2 += bluetooth_change;
//                        else EM2 -= bluetooth_change;

//                        break;

//                    case '3':
//                        if(u5.RxBuffer[1] == '+') EM3 += bluetooth_change;
//                        else EM3 -= bluetooth_change;

//                        break;

//                    case '4':
//                        if(u5.RxBuffer[1] == '+') EM4 += bluetooth_change;
//                        else EM4 -= bluetooth_change;

//                        break;

//                    case '5':
//                        if(u5.RxBuffer[1] == '+') EM5 += bluetooth_change;
//                        else EM5 -= bluetooth_change;

//                        break;

//                    case '6':
//                        if(u5.RxBuffer[1] == '+') EM6 += bluetooth_change;
//                        else EM6 -= bluetooth_change;

//                        break;

//                    case '7':
//                        if(u5.RxBuffer[1] == '+')
//                        {
//                            EM1 -= bluetooth_change;
//                            EM2 += bluetooth_change;
//                            EM3 += bluetooth_change;
//                            EM4 -= bluetooth_change;
//                        }
//                        else
//                        {
//                            EM1 += bluetooth_change;
//                            EM2 -= bluetooth_change;
//                            EM3 -= bluetooth_change;
//                            EM4 += bluetooth_change;
//                        }

//                        break;

//                    case 'a': //深度目标值
//                        if(u5.RxBuffer[1] == '+') depth_aim += 0.01f;
//                        else depth_aim -= 0.01f;

//                        break;

//                    case 'b': //电机初始值
//                        if(u5.RxBuffer[1] == '+') EM_default += 5;
//                        else EM_default -= 5;

//                        break;

//                    case 'c':
//                        if(u5.RxBuffer[1] == '+') EC_depth.Kp += 10;
//                        else EC_depth.Kp -= 10;

//                        break;

//                    case 'd':
//                        if(u5.RxBuffer[1] == '+') EC_depth.Kd += 10;
//                        else EC_depth.Kd -= 10;

//                        break;

//                    case 'e':
//                        if(u5.RxBuffer[1] == '+') EC1_angle.Kp += 0.5f;
//                        else EC1_angle.Kp -= 0.5f;

//                        break;

//                    case 'f':
//                        if(u5.RxBuffer[1] == '+') EC1_angle.Kd += 0.5f;
//                        else EC1_angle.Kd -= 0.5f;

//                        break;

//                    case 'g':
//                        EM_able = !EM_able;
//                        stage_times = 0;

//                        break;

//                    case 'h':
//                        if(u5.RxBuffer[1] == '+') EC_depth.Ki += 0.01f;
//                        else EC_depth.Ki -= 0.01f;

//                        break;

//                    case 'i':
//                        if(u5.RxBuffer[1] == '+') EC2_angle.Kp += 0.5f;
//                        else EC2_angle.Kp -= 0.5f;

//                        break;

//                    case 'j':
//                        if(u5.RxBuffer[1] == '+') EC2_angle.Ki += 0.5f;
//                        else EC2_angle.Ki -= 0.5f;

//                        break;
                    
//                    case 'k':
//                        if(u5.RxBuffer[1] == '+') EC2_angle.Kd += 0.5f;
//                        else EC2_angle.Kd -= 0.5f;

//                        break;
//                }

//                u5.Usart_Rx_Cnt = 0;
//                memset(u5.RxBuffer, 0x00, sizeof(u5.RxBuffer));
//            }
//        }

//        HAL_UART_Receive_IT(&huart5, (uint8_t *) &u5.aRxBuffer, 1);
//    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
