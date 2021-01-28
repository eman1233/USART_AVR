/*
 * USART.h
 *
 *  Created on: 27/1/2012
 *      Author:Eman
 */

#ifndef USART_H_
#define USART_H_

#include "Platform_Types.h"
typedef enum
{
	Asynchronous=0,
	Synchronous,

}USART_OperationMode;

typedef enum
{
	Disabled=0,
	Even,
	Odd,

}USART_ParityMode;

typedef enum
{
	OneBit=0,
	TwoBit

}USART_StopBit;

typedef enum
{
	FiveBit=0,
	SixBit,
	SevenBit,
	EightBit,
	NineBit

}USART_CharacterSize;

typedef enum
{
	TransimateRaising_RecieveFalling=0,
	TransimateFalling_RecieveRaising,

}USART_ClockPolarity;

typedef struct
{
	USART_OperationMode OperationMode;
	USART_ParityMode ParityMode;
	USART_CharacterSize CharacterSize;
	u16 BoudRate;
	USART_StopBit StopBit;
}USART_Config;

static void USART_OperationModeSelection(USART_OperationMode OperationMode);

static void USART_ParityModeSelection (USART_ParityMode ParityMode);

static void USART_StopBitSelection(USART_StopBit StopBit);

static void USART_CharacterSizeSelection(USART_CharacterSize CharacterSize);

static void USART_ClockPolaritySelection(USART_ClockPolarity ClockPolarity);

static void USART_BoudRateSelection(u16 BoudRate);

static void TransimationEnable(void);

static void RecptionEnable(void);

void USART_Init(USART_Config *ConfigPtr);

void USART_TransimateData(u8 Data);

u8 USART_RecieveData(void);

void USART_TransimateString(u8 *str,u8 size);

u8 *USART_RecieveString(void);

void USART_TransimateAsyncronous(u8 *ptr,u8 size,void(*USART_TansimateCallBack)(void));

void USART_RecieveAsyncronous(u8 *ptr,u8 size,void(*USART_RecieveCallBack)(void));

#endif /* USART_H_ */
