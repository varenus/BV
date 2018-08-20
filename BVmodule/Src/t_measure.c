/**
  ******************************************************************************
  * @file		           t_measure.c
	* @author						 Stretencev A. I.
	* @date							 13.08.2018
  * @description       Time measuring functions
  ******************************************************************************
  ** This file provides release time measuring functions and other subs that
	*	helps to process the signal
  * 
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "t_measure.h"

/* External variables --------------------------------------------------------*/
extern BVMODULE_TypeDef bv;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim2;

/* External functions --------------------------------------------------------*/
extern void SendMeasuredValue(void);

/* Private variables ---------------------------------------------------------*/


uint8_t tmStopAll () {
	
	/* Stop DAC convertion */
	if (HAL_DAC_Stop(&hdac, DAC_CHANNEL_1) != HAL_OK) {
		bv.tm_status = TM_DAC_ERROR;
		return TM_DAC_ERROR;
	}
	
	if (bv.tm_mode == COMPARATOR) {
		
		/* Disable interrupt from comparaor */
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		
		/* TIM2 counter = 0 */
		__HAL_TIM_SetCounter(&htim2, 0);
		
	}
	
	bv.command = STOP;
	
	return TM_OK;
	
}

uint8_t tmStartMeasure () {
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Start DAC convertion */
	if (HAL_DAC_Start(&hdac, DAC_CHANNEL_1) != HAL_OK) {
		bv.tm_status = TM_DAC_ERROR;
		return TM_DAC_ERROR;
	}
	if (HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint16_t)(bv.i_setup/MaxI)*(ShuntMax/Quant)) != HAL_OK) {
		bv.tm_status = TM_DAC_ERROR;
		return TM_DAC_ERROR;
	}
	
	
	if (bv.tm_mode == COMPARATOR) {
		
		/* Both polarity for interrupt */
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		/* Enable interrupt from comparator */
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		
		/* TIM2 counter = 0 */
		__HAL_TIM_SetCounter(&htim2, 0);
	}
	
	bv.command = MEASURING;
	
	return TM_OK;
	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	
	if ((GPIO_Pin == GPIO_PIN_0)&&(bv.command == MEASURING)) {
		
		/* Start measuring if PA0 = 1 */
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
			
			HAL_TIM_Base_Start_IT(&htim2);
			
		} else {
		/* Stop measuring if PA0 = 0 */
			HAL_TIM_Base_Stop_IT(&htim2);
			HAL_NVIC_DisableIRQ(EXTI0_IRQn);
			bv.t_release = __HAL_TIM_GetCounter(&htim2);
			bv.is_value_valid = true;
			tmStopAll();
			SendMeasuredValue();
			
		}
		
	}
	
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	
	
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	
	if (htim->Instance == TIM2) {
		/* If release time if >= 65535 us */
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		bv.t_release = __HAL_TIM_GetCounter(&htim2);
		bv.is_value_valid = true;
		tmStopAll();
		SendMeasuredValue();
		
	}
	
}

/*****************************END OF FILE****/
