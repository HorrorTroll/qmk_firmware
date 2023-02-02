/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio
    ChibiOS - Copyright (C) 2022 Artery Technology
    ChibiOS - Copyright (C) 2022 HorrorTroll

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef MCUCONF_H
#define MCUCONF_H

#define AT32F403A_MCUCONF

/*
 * AT32F403A drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

/*
 * HAL driver system settings.
 */
#define AT32_NO_INIT                        FALSE
#define AT32_HICK_ENABLED                   TRUE
#define AT32_LICK_ENABLED                   TRUE
#define AT32_HEXT_ENABLED                   FALSE
#define AT32_LEXT_ENABLED                   FALSE
#define AT32_SCLKSEL                        AT32_SCLKSEL_PLL
#define AT32_PLLRCS                         AT32_PLLRCS_HICK
#define AT32_HICK_TO_SCLK                   AT32_HICK_TO_SCLK_SEL1
#define AT32_HICKDIV                        AT32_HICKDIV_DIV6
#define AT32_HICK_TO_USB                    AT32_HICK_TO_USB_PLL
#define AT32_HEXT_DIV                       AT32_HEXTDIV_DIV2
#define AT32_PLLHEXTDIV                     AT32_PLLHEXTDIV_DIV1
#define AT32_PLLMULT_VALUE                  12
#define AT32_AHBDIV                         AT32_AHBDIV_DIV1
#define AT32_APB1DIV                        AT32_APB1DIV_DIV2
#define AT32_APB2DIV                        AT32_APB2DIV_DIV2
#define AT32_ADCDIV                         AT32_ADCDIV_DIV2
#define AT32_USB_CLOCK_REQUIRED             TRUE
#define AT32_USBDIV                         AT32_USBDIV_DIV1
#define AT32_CLKOUT_SEL                     AT32_CLKOUT_SEL_NOCLOCK
#define AT32_RTCSEL                         AT32_RTCSEL_LICK
#define AT32_PVM_ENABLE                     FALSE
#define AT32_PVMSEL                         AT32_PVMSEL_LEV1

/*
 * IRQ system settings.
 */
#define AT32_IRQ_EXINT0_PRIORITY            6
#define AT32_IRQ_EXINT1_PRIORITY            6
#define AT32_IRQ_EXINT2_PRIORITY            6
#define AT32_IRQ_EXINT3_PRIORITY            6
#define AT32_IRQ_EXINT4_PRIORITY            6
#define AT32_IRQ_EXINT5_9_PRIORITY          6
#define AT32_IRQ_EXINT10_15_PRIORITY        6

/*
 * ADC driver system settings.
 */
#define AT32_ADC_USE_ADC1                   FALSE
#define AT32_ADC_ADC1_DMA_PRIORITY          2
#define AT32_ADC_ADC1_IRQ_PRIORITY          6

/*
 * CAN driver system settings.
 */
#define AT32_CAN_USE_CAN1                   FALSE
#define AT32_CAN_CAN1_IRQ_PRIORITY          11

/*
 * GPT driver system settings.
 */
#define AT32_GPT_USE_TMR1                   FALSE
#define AT32_GPT_USE_TMR2                   FALSE
#define AT32_GPT_USE_TMR3                   FALSE
#define AT32_GPT_USE_TMR4                   FALSE
#define AT32_GPT_USE_TMR5                   FALSE
#define AT32_GPT_USE_TMR8                   FALSE
#define AT32_GPT_TMR1_IRQ_PRIORITY          7
#define AT32_GPT_TMR2_IRQ_PRIORITY          7
#define AT32_GPT_TMR3_IRQ_PRIORITY          7
#define AT32_GPT_TMR4_IRQ_PRIORITY          7
#define AT32_GPT_TMR5_IRQ_PRIORITY          7
#define AT32_GPT_TMR8_IRQ_PRIORITY          7

/*
 * I2C driver system settings.
 */
#define AT32_I2C_USE_I2C1                   FALSE
#define AT32_I2C_USE_I2C2                   FALSE
#define AT32_I2C_BUSY_TIMEOUT               50
#define AT32_I2C_I2C1_IRQ_PRIORITY          5
#define AT32_I2C_I2C2_IRQ_PRIORITY          5
#define AT32_I2C_I2C1_DMA_PRIORITY          3
#define AT32_I2C_I2C2_DMA_PRIORITY          3
#define AT32_I2C_DMA_ERROR_HOOK(i2cp)       osalSysHalt("DMA failure")

/*
 * ICU driver system settings.
 */
#define AT32_ICU_USE_TMR1                   FALSE
#define AT32_ICU_USE_TMR2                   FALSE
#define AT32_ICU_USE_TMR3                   FALSE
#define AT32_ICU_USE_TMR4                   FALSE
#define AT32_ICU_USE_TMR5                   FALSE
#define AT32_ICU_USE_TMR8                   FALSE
#define AT32_ICU_TMR1_IRQ_PRIORITY          7
#define AT32_ICU_TMR2_IRQ_PRIORITY          7
#define AT32_ICU_TMR3_IRQ_PRIORITY          7
#define AT32_ICU_TMR4_IRQ_PRIORITY          7
#define AT32_ICU_TMR5_IRQ_PRIORITY          7
#define AT32_ICU_TMR8_IRQ_PRIORITY          7

/*
 * PWM driver system settings.
 */
#define AT32_PWM_USE_ADVANCED               FALSE
#define AT32_PWM_USE_TMR1                   FALSE
#define AT32_PWM_USE_TMR2                   FALSE
#define AT32_PWM_USE_TMR3                   FALSE
#define AT32_PWM_USE_TMR4                   FALSE
#define AT32_PWM_USE_TMR5                   FALSE
#define AT32_PWM_USE_TMR8                   FALSE
#define AT32_PWM_TMR1_IRQ_PRIORITY          7
#define AT32_PWM_TMR2_IRQ_PRIORITY          7
#define AT32_PWM_TMR3_IRQ_PRIORITY          7
#define AT32_PWM_TMR4_IRQ_PRIORITY          7
#define AT32_PWM_TMR5_IRQ_PRIORITY          7
#define AT32_PWM_TMR8_IRQ_PRIORITY          7

/*
 * RTC driver system settings.
 */
#define AT32_RTC_IRQ_PRIORITY               15

/*
 * SERIAL driver system settings.
 */
#define AT32_SERIAL_USE_USART1              FALSE
#define AT32_SERIAL_USE_USART2              FALSE
#define AT32_SERIAL_USE_USART3              FALSE
#define AT32_SERIAL_USE_UART4               FALSE
#define AT32_SERIAL_USE_UART5               FALSE
#define AT32_SERIAL_USE_USART6              FALSE
#define AT32_SERIAL_USE_UART7               FALSE
#define AT32_SERIAL_USE_UART8               FALSE
#define AT32_SERIAL_USART1_PRIORITY         12
#define AT32_SERIAL_USART2_PRIORITY         12
#define AT32_SERIAL_USART3_PRIORITY         12
#define AT32_SERIAL_UART4_PRIORITY          12
#define AT32_SERIAL_UART5_PRIORITY          12
#define AT32_SERIAL_USART6_PRIORITY         12
#define AT32_SERIAL_UART7_PRIORITY          12
#define AT32_SERIAL_UART8_PRIORITY          12

/*
 * SPI driver system settings.
 */
#define AT32_SPI_USE_SPI1                   FALSE
#define AT32_SPI_USE_SPI2                   FALSE
#define AT32_SPI_USE_SPI3                   FALSE
#define AT32_SPI_SPI1_DMA_PRIORITY          1
#define AT32_SPI_SPI2_DMA_PRIORITY          1
#define AT32_SPI_SPI3_DMA_PRIORITY          1
#define AT32_SPI_SPI1_IRQ_PRIORITY          10
#define AT32_SPI_SPI2_IRQ_PRIORITY          10
#define AT32_SPI_SPI3_IRQ_PRIORITY          10
#define AT32_SPI_DMA_ERROR_HOOK(spip)       osalSysHalt("DMA failure")

/*
 * ST driver system settings.
 */
#define AT32_ST_IRQ_PRIORITY                8
#define AT32_ST_USE_TIMER                   2

/*
 * UART driver system settings.
 */
#define AT32_UART_USE_USART1                FALSE
#define AT32_UART_USE_USART2                FALSE
#define AT32_UART_USE_USART3                FALSE
#define AT32_UART_USART1_IRQ_PRIORITY       12
#define AT32_UART_USART2_IRQ_PRIORITY       12
#define AT32_UART_USART3_IRQ_PRIORITY       12
#define AT32_UART_USART1_DMA_PRIORITY       0
#define AT32_UART_USART2_DMA_PRIORITY       0
#define AT32_UART_USART3_DMA_PRIORITY       0
#define AT32_UART_DMA_ERROR_HOOK(uartp)     osalSysHalt("DMA failure")

/*
 * USB driver system settings.
 */
#define AT32_USB_USE_USB1                   TRUE
#define AT32_USB_LOW_POWER_ON_SUSPEND       FALSE
#define AT32_USB_USB1_H_IRQ_PRIORITY        13
#define AT32_USB_USB1_L_IRQ_PRIORITY        14

/*
 * WDG driver system settings.
 */
#define AT32_WDG_USE_IWDG                   FALSE

#endif /* MCUCONF_H */
