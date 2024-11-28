#include "stm32f10x.h" // Device header
#include "stdio.h"

uint8_t txpack[4];
char rxpack[100];
uint8_t data;
uint8_t Serial_RxFlag3;

void serial3_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // 模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		  // 引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  // 初始化 GPIOB

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	  // 引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);		  // 初始化 GPIOB

	USART_InitTypeDef USART_INITStructure;
	USART_INITStructure.USART_BaudRate = 9600;										// 波特率
	USART_INITStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 流控
	USART_INITStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;					// 收发模式
	USART_INITStructure.USART_Parity = USART_Parity_No;								// 校验位
	USART_INITStructure.USART_StopBits = USART_StopBits_1;							// 停止位
	USART_INITStructure.USART_WordLength = USART_WordLength_8b;						// 字长
	USART_Init(USART3, &USART_INITStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 使能接收中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        // 中断分组
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;		   // 指定通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		   // 开启
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;		   // 响应优先级
	NVIC_Init(&NVIC_InitStruct);

	USART_Cmd(USART3, ENABLE);
}

void serial3_sendbyte(uint8_t byte)
{
	USART_SendData(USART3, byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void serial_sendstring(char *string)
{
	int i;
	for (i = 0; string[i] != 0; i++)
	{
		serial3_sendbyte(string[i]);
	}
}

uint32_t power(uint32_t x, uint32_t y)
{
	uint32_t result = 1;
	while (y--)
	{
		result *= x;
	}
	return result;
}

void serial_sendnum(uint32_t num, uint8_t length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		serial3_sendbyte(num / power(10, length - i - 1) % 10 + '0');
	}
}

void serial_sendpack(void)
{
	serial3_sendbyte(0xFF);
	int i;
	for (i = 0; i < 4; i++)
	{
		serial3_sendbyte(txpack[i]);
	}
	serial3_sendbyte(0xFE);
}

uint8_t serial_recdata(void)
{
	return data;
}


void USART3_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);

		if (RxState == 0)
		{
			if (RxData == '@' && Serial_RxFlag3 == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				rxpack[pRxPacket] = RxData;
				pRxPacket++;
			}
		}
		else if (RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				rxpack[pRxPacket] = '\0';
				Serial_RxFlag3 = 1;
			}
		}

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}


}