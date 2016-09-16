/*
 * main.h
 *
 *  Created on: Sep 1, 2016
 *      Author: luther stanton
 */
#include "stm32f4xx.h"
#include "serial.h"

#ifndef MAIN_H_
#define MAIN_H_

extern uint16_t ClkStatus;

void SysTick_Handler(void);

#endif /* MAIN_H_ */
