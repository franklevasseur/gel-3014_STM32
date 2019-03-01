#include "uart_module.h"

GPIO_TypeDef* GPIOX_UART = GPIOC;
uint32_t GPIO_UART_CLCK = RCC_AHB1Periph_GPIOC;
uint32_t UART_CLCK = RCC_APB1Periph_UART4;

uint8_t UART_IRQ_Chn = UART4_IRQn;

USART_TypeDef* UARTX = UART4;
uint8_t GPIO_UART_AF = GPIO_AF_UART4;

uint16_t GPIO_PinSourceTX = GPIO_PinSource10;
uint16_t GPIO_PinSourceRX = GPIO_PinSource11;

uint32_t GPIO_Pin_TX = GPIO_Pin_10;
uint32_t GPIO_Pin_RX = GPIO_Pin_11;

void init_uart(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(GPIO_UART_CLCK, ENABLE);

	GPIO_PinAFConfig(GPIOX_UART, GPIO_PinSourceTX, GPIO_UART_AF);
	GPIO_PinAFConfig(GPIOX_UART, GPIO_PinSourceRX, GPIO_UART_AF);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_TX | GPIO_Pin_RX;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOX_UART, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(UART_CLCK, ENABLE);

	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(UARTX, &USART_InitStruct);
	USART_Cmd(UARTX, ENABLE);

	USART_ITConfig(UARTX, USART_IT_RXNE, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel = UART_IRQ_Chn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

void UART4_IRQHandler(void)
{
	if(UARTX->SR & USART_FLAG_RXNE)
	{
		UARTX->SR &= ~USART_FLAG_RXNE;

		char receivedByte = UARTX -> DR;
		writeInstructionBuffer(receivedByte);
	}
}

void uart_write(char byte) {

	while (!(UARTX->SR & USART_FLAG_TXE)); // attente active
	UARTX->DR = byte;
	return;
}

void transmit_sample(int16_t sample) {
	char firstByte = (char)(sample & 0xFF);
	char secondByte = (char)((sample >> 8) & 0xFF);

	uart_write(firstByte);
	uart_write(secondByte);
}
