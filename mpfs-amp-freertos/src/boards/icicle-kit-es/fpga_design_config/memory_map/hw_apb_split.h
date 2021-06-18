/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file hw_apb_split.h
 * @author Microchip-FPGA Embedded Systems Solutions
 *
 *
 * Note 1: This file should not be edited. If you need to modify a parameter
 * without going through regenerating using the MSS Configurator Libero flow 
 * or editing the associated xml file
 * the following method is recommended: 

 * 1. edit the following file 
 * boards/your_board/platform_config/mpfs_hal_config/mss_sw_config.h

 * 2. define the value you want to override there.
 * (Note: There is a commented example in the platform directory)

 * Note 2: The definition in mss_sw_config.h takes precedence, as
 * mss_sw_config.h is included prior to the generated header files located in
 * boards/your_board/fpga_design_config
 *
 */

#ifndef HW_APB_SPLIT_H_
#define HW_APB_SPLIT_H_


#ifdef __cplusplus
extern  "C" {
#endif

#if !defined (LIBERO_SETTING_APBBUS_CR)
/*AMP Mode peripheral mapping register. When the register bit is '0' the
peripheral is mapped into the 0x2000000 address range using AXI bus 5 from the
Coreplex. When the register bit is '1' the peripheral is mapped into the
0x28000000 address range using AXI bus 6 from the Coreplex. */
#define LIBERO_SETTING_APBBUS_CR    0x00000000UL
    /* MMUART0                           [0:1]   RWC */
    /* MMUART1                           [1:1]   RWC */
    /* MMUART2                           [2:1]   RWC */
    /* MMUART3                           [3:1]   RWC */
    /* MMUART4                           [4:1]   RWC */
    /* WDOG0                             [5:1]   RWC */
    /* WDOG1                             [6:1]   RWC */
    /* WDOG2                             [7:1]   RWC */
    /* WDOG3                             [8:1]   RWC */
    /* WDOG4                             [9:1]   RWC */
    /* SPI0                              [10:1]  RWC */
    /* SPI1                              [11:1]  RWC */
    /* I2C0                              [12:1]  RWC */
    /* I2C1                              [13:1]  RWC */
    /* CAN0                              [14:1]  RWC */
    /* CAN1                              [15:1]  RWC */
    /* GEM0                              [16:1]  RWC */
    /* GEM1                              [17:1]  RWC */
    /* TIMER                             [18:1]  RWC */
    /* GPIO0                             [19:1]  RWC */
    /* GPIO1                             [20:1]  RWC */
    /* GPIO2                             [21:1]  RWC */
    /* RTC                               [22:1]  RWC */
    /* H2FINT                            [23:1]  RWC */
#endif

#ifdef __cplusplus
}
#endif


#endif /* #ifdef HW_APB_SPLIT_H_ */

