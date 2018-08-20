/**
  ******************************************************************************
  * @file		           t_measure.h
	* @author						 Stretencev A. I.
	* @date							 10.08.2018
  * @description       This file contains defines, includes and function
	*											prototypes for t_measure.c
  ******************************************************************************
  ** 
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef T_MEASURE_H
#define T_MEASURE_H

/* Includes ------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
#define TM_OK 0
#define TM_DAC_ERROR 1

/* Private define ------------------------------------------------------------*/
#define Quant (3.3/4096)
#define MaxI 5000
#define ShuntMax 3

#define Tconv 1

/* Exported functions ------------------------------------------------------- */
uint8_t tmStopAll (void);
uint8_t tmStartMeasure (void);

#endif /* T_MEASURE_H */
/*****************************END OF FILE****/
