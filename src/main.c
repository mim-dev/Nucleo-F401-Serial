#include "main.h"

#define CODED_DATA_LENGTH 	11

// local variables
static volatile uint32_t sysTickCnt = 0;

// local forward function definitions
static inline void configureGPIO(void);
static inline void configureGps(void);

static char data[CODED_DATA_LENGTH] = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o',
		'r', 'l', 'd' };

/**
 **===========================================================================
 **
 **  Main entry point
 **
 **  - Configures GPIO ports
 **  - Sets SysTick for 100mS interval
 **  - Turns on LED attached to PA5
 **
 **===========================================================================
 */
int main(void) {

	if (ClkStatus != RESET) {

		configureUSART2();

		configureGPIO();
		configureGps();
		SysTick_Config((uint32_t) 0x00802C7F);

		GPIOA->ODR |= GPIO_ODR_ODR_5;

		while (1) {

		}

	} else {
		while (1) {

		}
	}
}

/**
 **===========================================================================
 **
 **  Toggles PA5 state every second and sends 'Hello World' on USART2
 **
 **===========================================================================
 */
void SysTick_Handler(void) {
	if (++sysTickCnt > 9) {
		GPIOA->ODR ^= GPIO_ODR_ODR_5;
		sysTickCnt = 0;
		transmitUSART2(data, CODED_DATA_LENGTH);
	}
}

/**

/**
 **===========================================================================
 **
 **  Configures GPIOs
 **
 **  - PA5 as output
 *
 **  - PA11 / 12 USART6 TX / RX (AF08)
 **
 **===========================================================================
 */
static inline void configureGPIO(void) {

	GPIOA->MODER &= ~(GPIO_MODER_MODER5);
	GPIOA->MODER |= (GPIO_MODER_MODER5_0);

	GPIOA->AFR[1] &= ~((0x0F << 12) | (0x0F << 16));
	GPIOA->AFR[1] |= (0x08 << 12) | (0x08 << 16);

	GPIOA->MODER &= ~(GPIO_MODER_MODER11 | GPIO_MODER_MODER12);
	GPIOA->MODER |= (GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1);
}

static inline void configureGps(void) {

}
