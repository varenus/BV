/**
  ******************************************************************************
  * @file		           main.h
	* @author						 Stretencev A. I.
	* @date							 10.08.2018
  * @description       This file contains the common defines of the application
  ******************************************************************************
  ** 
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
 /* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

/* Private define ------------------------------------------------------------*/
#define RX_BUFFER_SIZE 32

// tm_mode 
#define COMPARATOR 0
#define ANALOG_ONE_CHANNEL 1
#define ANALOG_TWO_CHANNELS 2
#define MIXED 3

typedef enum cmd{
	
	STOP,
	MEASURING
	
} cmd;

typedef struct {
	
	uint8_t buf_point;
	uint8_t rx_buffer[RX_BUFFER_SIZE];
	bool parsing;
	
	
} RS485_TypeDef;

typedef struct {
	
	cmd command;
	uint16_t t_release;
	uint16_t i_setup;
	bool is_value_valid;
	uint8_t tm_status;
	uint8_t tm_mode;
	
} BVMODULE_TypeDef;



/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/*****************************END OF FILE****/
