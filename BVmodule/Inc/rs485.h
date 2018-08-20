/**
  ******************************************************************************
  * @file		           rs485.h
	* @author						 Stretencev A. I.
	* @date							 10.08.2018
  * @description       This file contains defines, includes and function
	*											prototypes for RS485
  ******************************************************************************
  ** 
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef RS485_H
#define RS485_H

/* Includes ------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */
uint8_t ChSum(uint8_t *pcBlock, uint16_t len);
void ParseCommand(void);
void SendMeasuredValue(void);

#endif /* RS485_H */
/*****************************END OF FILE****/
