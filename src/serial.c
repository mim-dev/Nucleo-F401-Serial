/*
 * serial.c
 *
 *  Created on: Sep 15, 2016
 *      Author: luther stanton
 */

#include "serial.h"

// transmit status elements
static volatile unsigned char *transmitDataBuffer;
static volatile uint16_t transmitBufferSize = 0;
static volatile uint16_t transmitIndex = 0;

// receive status elements
static volatile unsigned char receiveDataBuffer[10];
static volatile uint16_t receiveIndex = 0;
static volatile uint16_t receiveReadIndex = 0;

static inline void processSerialData(void);

bool serialDataAvailable(){
	return receiveIndex - receiveReadIndex;
}

unsigned char readSerialData(){

	unsigned char result = 0;

	if(serialDataAvailable()){
		result = *(receiveDataBuffer + receiveReadIndex);
		receiveReadIndex = (receiveReadIndex + 1) % 10;
	}

	return result;
}

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
	USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE);
	NVIC_EnableIRQ(USART2_IRQn);
}

void transmitUSART2(unsigned char *data, uint16_t dataSize) {
	transmitDataBuffer = data;
	transmitBufferSize = dataSize;
	transmitIndex = 1;

	USART2->DR = *transmitDataBuffer;
	USART2->CR1 |= (USART_CR1_TXEIE);
}

void USART2_IRQHandler(void) {

	if (transmitBufferSize > 0) {
		// we are currently transmitting - check to see if we are ready to
		//	send the next character
		if ((USART2->SR & USART_SR_TXE) == USART_SR_TXE) {
			if (transmitIndex < transmitBufferSize) {
				USART2->DR = *(transmitDataBuffer + transmitIndex++);
			} else {
				USART2->CR1 &= ~(USART_CR1_TXEIE);
				transmitBufferSize = 0;
			}
		}

		// any new data available?
		if ((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE) {
			processSerialData();
		}
	} else {
		// not transmitting, see if data is available
		if ((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE) {
			processSerialData();
		}
	}
}

static inline void processSerialData(void){
	*(receiveDataBuffer + receiveIndex) = USART2->DR;
	receiveIndex = (receiveIndex + 1) % 10;
}
