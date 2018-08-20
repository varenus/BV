/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * 
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{

}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
 
  while (1)
  {
  }
 
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
 
  while (1)
  {
  }
 
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
 
  while (1)
  {
  }

}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{

  while (1)
  {
  }

}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{

}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{

}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{

}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
 
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
 
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/
/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{

  HAL_TIM_IRQHandler(&htim2);

}
/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
 
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
 
}

/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{

  HAL_UART_IRQHandler(&huart2);

}

/**
* @brief This function handles DMA2 stream0 global interrupt.
*/
void DMA2_Stream0_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_adc1);
 
}

/**
  * @brief  This function handles DMA interrupt request (UART Rx)
  * @param  None
  * @retval None     
  */
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(huart2.hdmarx);
}

/**
  * @brief  This function handles DMA interrupt request (UART Tx)
  * @param  None
  * @retval None  
  */
void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(huart2.hdmatx);
}


/***************************END OF FILE****/
