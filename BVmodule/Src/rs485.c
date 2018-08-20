/**
  ******************************************************************************
  * @file    			rs485.c
	* @author				Stretencev A. I.
	* @data					10.08.2018
  * @brief   			RS485 routines and command services
  ******************************************************************************
	*
  *		REQUESTS:
	*
	*			X@3600[CRC]\r						-	Stop all
	*
	*			X@3601[CRC]\r						- Get status
	*
	*			X@3610[Is_A][CRC]\r			- Start measuring
	*												[Io_A] Setup current value in A (hex, 0000, uint16)
	*
	*			X@3611[CRC]\r						- Get measured value
	*			
	*		-------------------------------------------------------------------------
	*
  * 	ANSWERS:
	*
	*			X>3600[CRC]\r						- Stopped
	*
	*			X>3601[Stat][CRC]\r			- Status
	*												[Stat] Status message (00 - nothing
	*																							 01 - measuring
	*																							 02 - finished)
	*
	*			X>3610[CRC]\r						- Measuring started
	*
	*			X>3611[Trel_us][CRC]\r 	- Measured value
	*												[Trel_us] Release time in us (hex, 0000, uint16)
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "t_measure.h"
#include "rs485.h"
#include <stdlib.h>

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern RS485_TypeDef rs485;
extern BVMODULE_TypeDef bv;

/* Private variables ---------------------------------------------------------*/
uint8_t DMABufRx[2];
uint8_t DMABufTx[40];
uint8_t TmpBuf[40];


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	
	if (huart->Instance == USART2) {
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	
	if (huart->Instance == USART2) {
	
		if (rs485.parsing == false) {
		
			if (DMABufRx[0] == 'X') {
				
				// Message start
				rs485.rx_buffer[0] = 'X';
				rs485.buf_point = 1;
				
			} else if (DMABufRx[0] == '\r')	{
				
				// Message end
				sprintf((char *)TmpBuf, "%02X", ChSum(rs485.rx_buffer, rs485.buf_point - 2));
				
				// Check Sum is correct
				if ((TmpBuf[0] == rs485.rx_buffer[rs485.buf_point - 2]) && (TmpBuf[1] == rs485.rx_buffer[rs485.buf_point - 1])) {
					
					// 0x36 Adress
					if ((rs485.rx_buffer[2] == '3') && (rs485.rx_buffer[3] == '6')) {
						rs485.parsing = true;
					}
					
				}
				
			} else if (rs485.buf_point < RX_BUFFER_SIZE)	{
				
				rs485.rx_buffer[rs485.buf_point] = DMABufRx[0];
				rs485.buf_point++;
				
			} else if (rs485.buf_point == RX_BUFFER_SIZE) {
				
				// End of buffer
				rs485.rx_buffer[0] = DMABufRx[0];
				rs485.buf_point = 1;
				
			}
			
		}
		
	}
	
}

// It works
uint8_t ChSum(uint8_t *pcBlock, uint16_t len) {

    uint8_t sum = 0;

    while(len--)
    {
        sum += (uint8_t)(*pcBlock++);
    }

    return sum;
}


void ParseCommand() {
	
	uint8_t len = 0;
	
	if (rs485.parsing == false) {
		return;
	}
	
	switch (rs485.rx_buffer[4]) {
		
		case '0':
			
			switch (rs485.rx_buffer[5]) {
				
				case '0': // Stop all
					if (tmStopAll() == TM_OK) {
						len = sprintf((char *)TmpBuf, "X>3600");
					} else {
						len = sprintf((char *)TmpBuf, "X!3600");
					}
					break;
					
				case '1': // Get status
					if (bv.command == STOP) {
						if (bv.is_value_valid == true) {
							len = sprintf((char *)TmpBuf, "X>360102");
						} else {
							len = sprintf((char *)TmpBuf, "X>360100");
						}
					} else {
						len = sprintf((char *)TmpBuf, "X>360101");
					}
					break;
				
				default:
					len = sprintf((char *)TmpBuf, "X?360%c", rs485.rx_buffer[5]);
					
			}
			
			break;
		
		case '1':
			
			switch (rs485.rx_buffer[5]) {
				
				case '0': // Start measure
					if (tmStartMeasure() == TM_OK) {
						sprintf((char *)TmpBuf, "%c%c ", rs485.rx_buffer[6], rs485.rx_buffer[7]);
						bv.i_setup = strtol((char *)TmpBuf, 0, 16);
						len = sprintf((char *)TmpBuf, "X>3610");
					} else {
						len = sprintf((char *)TmpBuf, "X!3610");
					}
					break;
				
				case '1': // Get measured value
					if (bv.is_value_valid == true) {
						len = sprintf((char *)TmpBuf, "X>3611%04X", bv.t_release);
					} else {
						len = sprintf((char *)TmpBuf, "X!3611");
					}
					break;
				
				default:
					len = sprintf((char *)TmpBuf, "X?361%c", rs485.rx_buffer[5]);
				
			}
		
			break;
			
			default:
				len = sprintf((char *)TmpBuf, "X?36%c%c", rs485.rx_buffer[4], rs485.rx_buffer[5]);
	}
	
	len = sprintf((char *)DMABufTx, "%s%02X\r", (char *)TmpBuf, ChSum(TmpBuf, len));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit_DMA(&huart2, DMABufTx, len);
	
	rs485.parsing = false;
	
}

void SendMeasuredValue() {
	
	uint8_t len = 0;
	len = sprintf((char *)TmpBuf, "X>3611%04X", bv.t_release);
	len = sprintf((char *)DMABufTx, "%s%02X\r", (char *)TmpBuf, ChSum(TmpBuf, len));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit_DMA(&huart2, DMABufTx, len);
	
}
/*****************************END OF FILE****/
