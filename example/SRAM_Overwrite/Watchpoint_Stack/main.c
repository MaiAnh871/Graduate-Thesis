/*********************************************************************************************************//**
 * @file    SRAM_Overwrite/Watchpoint_Stack/main.c
 * @version $Rev:: 6686         $
 * @date    $Date:: 2023-01-19 #$
 * @brief   Main program.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup SRAM_Overwrite_Examples SRAM_Overwrite
  * @{
  */

/** @addtogroup Watchpoint_Stack
  * @{
  */


/* Private function prototypes -----------------------------------------------------------------------------*/
void recursive_func(s32);

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  s32 depthStart=0;

  RETARGET_Configuration();
  /*To start push variables in the stack section.*/
  recursive_func(depthStart);
  
  while (1); /* Test Passed                                                                                 */
}

/*********************************************************************************************************//**
  * @brief  To constantly push the variable in the stack section.
  * @param  depth: specify the depth of recursion.
  * @retval None
 ************************************************************************************************************/
void recursive_func(s32 depth)
{
  printf("Depth[%d] : %p\r\n", depth, &depth);
  recursive_func(depth+1);
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
