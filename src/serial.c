/*
 * serial.c
 *
 *  Created on: Sep 15, 2016
 *      Author: luther stanton
 */

#include "serial.h"

static volatile char *dataBuffer;
static volatile uint16_t bufferSize;
static volatile uint16_t transmitIndex;

/*
 * Configures USART2 for data transmit / receive @ 115200 baud 8N1
 *  - PA02 / 03 USART2 TX / RX (AF07) (ties to Arduino USART)
 *  - USART2 - TX / RX enable @ 115200 8N1
 *  - USART2 - enable TXE interrupt
 */
void configureUSART2(void) {

	GPIOA->AFR[0] &= ~((0x0F << 8) | (0x0F << 12));
	GPIOA->AFR[0] |= (0x07 << 8) | (0x07 << 12);

	GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
	GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);

	USART2->BRR |= (0xB << 4) | (0x6);
	USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_TXEIE);
}

void transmitUSART2(char *data, uint16_t dataSize) {
	dataBuffer = data;
	bufferSize = dataSize;
	transmitIndex = 1;

	USART2->DR = *data;
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void){

	if((USART2->SR & USART_SR_TXE) == USART_SR_TXE){
		if(transmitIndex < bufferSize){
			USART2->DR = *(dataBuffer + transmitIndex++);
		} else{
			NVIC_DisableIRQ(USART2_IRQn);
		}
	}
}
