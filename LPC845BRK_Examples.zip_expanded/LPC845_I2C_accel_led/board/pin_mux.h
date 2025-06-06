/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
    kPIN_MUX_DirectionInput = 0U,        /* Input direction */
    kPIN_MUX_DirectionOutput = 1U,       /* Output direction */
    kPIN_MUX_DirectionInputOrOutput = 2U /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PIO1_0 (number 11), PIO1_0 / Green LED
  @{ */
#define BOARD_INITPINS_GREEN_PERIPHERAL GPIO                    /*!<@brief Device name: GPIO */
#define BOARD_INITPINS_GREEN_SIGNAL PIO1                        /*!<@brief GPIO signal: PIO1 */
#define BOARD_INITPINS_GREEN_GPIO GPIO                          /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_GREEN_GPIO_PIN 0U                        /*!<@brief PIO1 pin index: 0 */
#define BOARD_INITPINS_GREEN_PORT 1U                            /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_GREEN_PIN 0U                             /*!<@brief 1U pin index: 0 */
#define BOARD_INITPINS_GREEN_CHANNEL 0                          /*!<@brief GPIO PIO1 channel: 0 */
#define BOARD_INITPINS_GREEN_PIN_NAME PIO1_0                    /*!<@brief Pin name */
#define BOARD_INITPINS_GREEN_LABEL "PIO1_0 / Green LED"         /*!<@brief Label */
#define BOARD_INITPINS_GREEN_NAME "GREEN"                       /*!<@brief Identifier name */
#define BOARD_INITPINS_GREEN_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                                /* @} */

/*! @name PIO1_1 (number 14), PIO1_1 / Blue LED
  @{ */
#define BOARD_INITPINS_BLUE_PERIPHERAL GPIO                    /*!<@brief Device name: GPIO */
#define BOARD_INITPINS_BLUE_SIGNAL PIO1                        /*!<@brief GPIO signal: PIO1 */
#define BOARD_INITPINS_BLUE_GPIO GPIO                          /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_BLUE_GPIO_PIN 1U                        /*!<@brief PIO1 pin index: 1 */
#define BOARD_INITPINS_BLUE_PORT 1U                            /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_BLUE_PIN 1U                             /*!<@brief 1U pin index: 1 */
#define BOARD_INITPINS_BLUE_CHANNEL 1                          /*!<@brief GPIO PIO1 channel: 1 */
#define BOARD_INITPINS_BLUE_PIN_NAME PIO1_1                    /*!<@brief Pin name */
#define BOARD_INITPINS_BLUE_LABEL "PIO1_1 / Blue LED"          /*!<@brief Label */
#define BOARD_INITPINS_BLUE_NAME "BLUE"                        /*!<@brief Identifier name */
#define BOARD_INITPINS_BLUE_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                               /* @} */

/*! @name PIO1_2 (number 16), PIO1_2 / Red LED
  @{ */
#define BOARD_INITPINS_RED_PERIPHERAL GPIO                    /*!<@brief Device name: GPIO */
#define BOARD_INITPINS_RED_SIGNAL PIO1                        /*!<@brief GPIO signal: PIO1 */
#define BOARD_INITPINS_RED_GPIO GPIO                          /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_RED_GPIO_PIN 2U                        /*!<@brief PIO1 pin index: 2 */
#define BOARD_INITPINS_RED_PORT 1U                            /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_RED_PIN 2U                             /*!<@brief 1U pin index: 2 */
#define BOARD_INITPINS_RED_CHANNEL 2                          /*!<@brief GPIO PIO1 channel: 2 */
#define BOARD_INITPINS_RED_PIN_NAME PIO1_2                    /*!<@brief Pin name */
#define BOARD_INITPINS_RED_LABEL "PIO1_2 / Red LED"           /*!<@brief Label */
#define BOARD_INITPINS_RED_NAME "RED"                         /*!<@brief Identifier name */
#define BOARD_INITPINS_RED_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                              /* @} */

/*! @name PIO0_4 (number 6), PIO0_4 / PINT_0 / User Push Button
  @{ */
#define BOARD_INITPINS_USER_PERIPHERAL PINT                            /*!<@brief Device name: PINT */
#define BOARD_INITPINS_USER_SIGNAL PINT                                /*!<@brief PINT signal: PINT */
#define BOARD_INITPINS_USER_PORT 0U                                    /*!<@brief PORT device name: 0U */
#define BOARD_INITPINS_USER_PIN 4U                                     /*!<@brief 0U pin index: 4 */
#define BOARD_INITPINS_USER_CHANNEL 0                                  /*!<@brief PINT PINT channel: 0 */
#define BOARD_INITPINS_USER_PIN_NAME PIO0_4                            /*!<@brief Pin name */
#define BOARD_INITPINS_USER_LABEL "PIO0_4 / PINT_0 / User Push Button" /*!<@brief Label */
#define BOARD_INITPINS_USER_NAME "USER"                                /*!<@brief Identifier name */
                                                                       /* @} */

/*! @name PIO0_12 (number 4), PIO0_12 / PINT_1 / ISP
  @{ */
#define BOARD_INITPINS_ISP_PERIPHERAL PINT                /*!<@brief Device name: PINT */
#define BOARD_INITPINS_ISP_SIGNAL PINT                    /*!<@brief PINT signal: PINT */
#define BOARD_INITPINS_ISP_PORT 0U                        /*!<@brief PORT device name: 0U */
#define BOARD_INITPINS_ISP_PIN 12U                        /*!<@brief 0U pin index: 12 */
#define BOARD_INITPINS_ISP_CHANNEL 1                      /*!<@brief PINT PINT channel: 1 */
#define BOARD_INITPINS_ISP_PIN_NAME PIO0_12               /*!<@brief Pin name */
#define BOARD_INITPINS_ISP_LABEL "PIO0_12 / PINT_1 / ISP" /*!<@brief Label */
#define BOARD_INITPINS_ISP_NAME "ISP"                     /*!<@brief Identifier name */
                                                          /* @} */

/*! @name ADC_0 (number 33), ADC_0 / Potentiometer
  @{ */
#define BOARD_INITPINS_ADC_PERIPHERAL ADC0               /*!<@brief Device name: ADC0 */
#define BOARD_INITPINS_ADC_SIGNAL CH                     /*!<@brief ADC0 signal: CH */
#define BOARD_INITPINS_ADC_CHANNEL 0                     /*!<@brief ADC0 channel: 0 */
#define BOARD_INITPINS_ADC_PIN_NAME ADC_0                /*!<@brief Pin name */
#define BOARD_INITPINS_ADC_LABEL "ADC_0 / Potentiometer" /*!<@brief Label */
#define BOARD_INITPINS_ADC_NAME "ADC"                    /*!<@brief Identifier name */
                                                         /* @} */

/*! @name ACMP_I5 (number 42), ACMP_I5 / Capacitive Touch Analog CMP
  @{ */
/*!
 * @brief Device name: ACMP */
#define BOARD_INITPINS_CAPY_PERIPHERAL ACMP
/*!
 * @brief ACMP signal: ACMP_IN5 */
#define BOARD_INITPINS_CAPY_SIGNAL ACMP_IN5
/*!
 * @brief Pin name */
#define BOARD_INITPINS_CAPY_PIN_NAME ACMP_I5
/*!
 * @brief Label */
#define BOARD_INITPINS_CAPY_LABEL "ACMP_I5 / Capacitive Touch Analog CMP"
/*!
 * @brief Identifier name */
#define BOARD_INITPINS_CAPY_NAME "CAPY"
/* @} */

/*! @name CAPT_X0 (number 9), CAPT_X0 / Capacitive Touch X0
  @{ */
#define BOARD_INITPINS_CAPX_PERIPHERAL CAPT                       /*!<@brief Device name: CAPT */
#define BOARD_INITPINS_CAPX_SIGNAL CAPTX                          /*!<@brief CAPT signal: CAPTX */
#define BOARD_INITPINS_CAPX_CHANNEL 0                             /*!<@brief CAPT CAPTX channel: 0 */
#define BOARD_INITPINS_CAPX_PIN_NAME CAPT_X0                      /*!<@brief Pin name */
#define BOARD_INITPINS_CAPX_LABEL "CAPT_X0 / Capacitive Touch X0" /*!<@brief Label */
#define BOARD_INITPINS_CAPX_NAME "CAPX"                           /*!<@brief Identifier name */
                                                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M0P */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
