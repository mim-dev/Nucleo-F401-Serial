/*
 * serial.h
 *
 *  Created on: Sep 15, 2016
 *      Author: luthe
 */

#include "stm32f4xx.h"

#ifndef SERIAL_H_
#define SERIAL_H_

/*
 * Configures USART 2 for TX / RX @ 115200 8N1
 */
void configureUSART2(void);
void transmitUSART2(char *data, uint16_t dataLength);

void USART2_IRQHandler(void);

#endif /* SERIAL_H_ */
