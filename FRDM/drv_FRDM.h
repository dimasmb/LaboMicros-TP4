/*
 * drv_FRDM.h
 *
 *  Created on: 2 sep. 2022
 *      Author: Salta
 */

#ifndef DRV_FRDM_H_
#define DRV_FRDM_H_

/**
 * @brief TODO: Inicializa LED RGB de la placa FRDM
 * @return Nada
*/
void DRV_FRDM_Init_LEDs(void);

/**
 * @brief TODO: Prende LED verde
 * @return Nada
*/
void TurnOn_LED_FRDM_GREEN(void);

/**
 * @brief TODO: Prende LED azul
 * @return Nada
*/
void TurnOn_LED_FRDM_BLUE(void);

/**
 * @brief TODO: Prende LED rojo
 * @return Nada
*/
void TurnOn_LED_FRDM_RED(void);

/**
 * @brief TODO: Apaga LED verde
 * @return Nada
*/
void TurnOff_LED_FRDM_GREEN(void);

/**
 * @brief TODO: Apaga LED azul
 * @return Nada
*/
void TurnOff_LED_FRDM_BLUE(void);

/**
 * @brief TODO: Apaga LED rojo
 * @return Nada
*/
void TurnOff_LED_FRDM_RED(void);

/**
 * @brief TODO: Cambia el estado del LED rojo
 * @return Nada
*/
void Toggle_LED_FRDM_RED(void);

/**
 * @brief TODO: Cambia el estado del LED verde
 * @return Nada
*/
void Toggle_LED_FRDM_GREEN(void);

/**
 * @brief TODO: Cambia el estado del LED azul
 * @return Nada
*/
void Toggle_LED_FRDM_BLUE(void);

/**
 * @brief TODO: Prende LED rojo por secs segundos
 * @return Nada
*/
void turnOn_Secs_RED(int secs);

/**
 * @brief TODO: Prende LED verde por secs segundos
 * @return Nada
*/
void turnOn_Secs_GREEN(int secs);

/**
 * @brief TODO: Prende LED azul por secs segundos
 * @return Nada
*/
void turnOn_Secs_BLUE(int secs);

#endif /* DRV_FRDM_H_ */
