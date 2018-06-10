/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
//Includes
#include "main.h"
#include "Marlin_export.h"
//Private defines
#ifdef STM32_USE_BOOTLOADER
#define APPLICATION_ADDRESS     (uint32_t)0x08002000
//#define APPLICATION_ADDRESS		  (uint32_t)0x08000000
//Private variables
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));

static inline void remapVectorTable(void)
{
	for(uint8_t i = 0; i < 48; i++)
	{
	    VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}
	  /* Enable the SYSCFG peripheral clock*/
	__HAL_RCC_APB2_FORCE_RESET();
	  /* Remap SRAM at 0x00000000 */
	__HAL_SYSCFG_REMAPMEMORY_SRAM();
	RCC->AHBENR = 1<<2 |1<<4 | 1<<18;
	 if((WWDG->CR == 0xC0) && (WWDG->CR & 0x80))
		 WWDG->CR = 0x000000ff;
//			 HAL_WWDG_Refresh(&WwdgHandle);
	 if(IWDG->RLR)
		 IWDG->RLR = 0x0000AAAAU;
	 __HAL_RCC_APB2_RELEASE_RESET();
}
#endif

int main(void)
{
#ifdef STM32_USE_BOOTLOADER
	remapVectorTable();
#endif
	setup();
	for(;;)
	{
		loop();
	}
}
