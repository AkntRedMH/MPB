#ifndef MCP41xx_H
#define MCP41xx_H

#include "stm32f1xx_hal.h"
#include "main.h"

#define uchar unsigned char
#define uint unsigned int

#define DATA_CD 0x11

#define MCP41xx_SPI_CLK_H()     HAL_GPIO_WritePin(MCP_CLK_GPIO_Port, MCP_CLK_Pin, GPIO_PIN_SET)
#define MCP41xx_SPI_CLK_L()     HAL_GPIO_WritePin(MCP_CLK_GPIO_Port, MCP_CLK_Pin, GPIO_PIN_RESET)

#define MCP41xx_SPI_DAT_H()     HAL_GPIO_WritePin(MCP_DAT_GPIO_Port, MCP_DAT_Pin, GPIO_PIN_SET)
#define MCP41xx_SPI_DAT_L()     HAL_GPIO_WritePin(MCP_DAT_GPIO_Port, MCP_DAT_Pin, GPIO_PIN_RESET)

#define MCP41xx_SPI_CS1_H()      HAL_GPIO_WritePin(MCP_CS1_GPIO_Port, MCP_CS1_Pin, GPIO_PIN_SET)
#define MCP41xx_SPI_CS1_L()      HAL_GPIO_WritePin(MCP_CS1_GPIO_Port, MCP_CS1_Pin, GPIO_PIN_RESET)

#define MCP41xx_SPI_CS2_H()      HAL_GPIO_WritePin(MCP_CS2_GPIO_Port, MCP_CS2_Pin, GPIO_PIN_SET)
#define MCP41xx_SPI_CS2_L()      HAL_GPIO_WritePin(MCP_CS2_GPIO_Port, MCP_CS2_Pin, GPIO_PIN_RESET)

void MCP410XXInit(void);
void mcp_delay(uint n);
void MCP41xx_1writedata(uchar dat1);
void MCP41xx_2writedata(uchar dat2);

#endif
