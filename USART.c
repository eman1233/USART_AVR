/*
 * USART.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: inter
 */

#include "USART.h"
#include "USART_MemMap.h"
static u8 UCSRC_MEM;
static u8 BusyFlagTransimate=0;
static u8 BusyFlagRecieve=0;
static void (*USART_TransimateInterrupt)(void)=NULL;
static void (*USART_RecieveInterrupt)(void)=NULL;
static u8 indexTransimate=0;
static u8 indexRecieve=0;
static u8 * StringTransimate=NULL;
static u8 * StringRecieve=NULL;
static u8 SizeStringTransimate=0;
static u8 SizeStringRecieve=0;
static void USART_OperationModeSelection(USART_OperationMode OperationMode)
{
	SET_BIT(UCSRC_MEM,URSEL);

	if(OperationMode==Asynchronous)
	{
		CLEAR_BIT(UCSRC_MEM,UMSEL);


	}
	else if(OperationMode==Synchronous)
	{
		SET_BIT(UCSRC_REG,UMSEL);
		//TODO:align
	}
	UCSRC_REG =UCSRC_MEM ;
}

static void USART_ParityModeSelection (USART_ParityMode ParityMode)
{
	SET_BIT(UCSRC_MEM,URSEL);
	if(ParityMode==Disabled)
	{
		CLEAR_BIT(UCSRC_MEM,UPM1);
		CLEAR_BIT(UCSRC_MEM,UPM0);
	}
	else if (ParityMode==Even)
	{
		SET_BIT(UCSRC_MEM,UPM1);
		CLEAR_BIT(UCSRC_MEM,UPM0);
	}
	else if(ParityMode==Odd)
	{
		SET_BIT(UCSRC_MEM,UPM1);
		SET_BIT(UCSRC_MEM,UPM0);
	}
	UCSRC_REG =UCSRC_MEM ;
}

static void USART_StopBitSelection(USART_StopBit StopBit)
{
	SET_BIT(UCSRC_MEM,URSEL);
	if(StopBit==OneBit)
	{
		CLEAR_BIT(UCSRC_MEM,USBS);
	}

	if(StopBit==TwoBit)
	{
		SET_BIT(UCSRC_MEM,USBS);
	}
	UCSRC_REG =UCSRC_MEM ;
}

static void USART_CharacterSizeSelection(USART_CharacterSize CharacterSize)
{
	SET_BIT(UCSRC_MEM,URSEL);
	if(CharacterSize==FiveBit)
	{
		CLEAR_BIT(UCSRB_REG,UCSZ2);
		//TODO:CHANGE
		CLEAR_BIT(UCSRC_MEM,UCSZ1);
		CLEAR_BIT(UCSRC_MEM,UCSZ0);

	}
	else if(CharacterSize==SixBit)
	{
		CLEAR_BIT(UCSRB_REG,UCSZ2);
		CLEAR_BIT(UCSRC_MEM,UCSZ1);
		SET_BIT(UCSRC_MEM,UCSZ0);
	}
	else if(CharacterSize==SevenBit)
	{
		CLEAR_BIT(UCSRB_REG,UCSZ2);
		SET_BIT(UCSRC_MEM,UCSZ1);
		CLEAR_BIT(UCSRC_MEM,UCSZ0);
	}
	else if(CharacterSize==EightBit)
	{
		CLEAR_BIT(UCSRB_REG,UCSZ2);
		SET_BIT(UCSRC_MEM,UCSZ1);
		SET_BIT(UCSRC_MEM,UCSZ0);
	}
	else if(CharacterSize==NineBit)
	{
		SET_BIT(UCSRB_REG,UCSZ2);
		SET_BIT(UCSRC_MEM,UCSZ1);
		SET_BIT(UCSRC_MEM,UCSZ0);
	}
	UCSRC_REG =UCSRC_MEM ;
}

static void USART_ClockPolaritySelection(USART_ClockPolarity ClockPolarity)
{
	//TODO:CHANGE
	SET_BIT(UCSRC_MEM,UMSEL);
	if(ClockPolarity==TransimateRaising_RecieveFalling)
	{
		CLEAR_BIT(UCSRC_MEM,UCPOL);
	}
	else if(ClockPolarity==TransimateFalling_RecieveRaising)
	{
		SET_BIT(UCSRC_MEM,UCPOL);
	}
	UCSRC_REG =UCSRC_MEM ;
}

static void USART_BoudRateSelection(u16 BoudRate)
{
	CLEAR_BIT(UCSRC_MEM,UMSEL);
	UBRRL_REG=BoudRate;
	UBRRH_REG=BoudRate>>8;
	UCSRC_REG =UCSRC_MEM ;
}

static void TransimationEnable(void)
{
	SET_BIT(UCSRB_REG,TXEN);

}

static void RecptionEnable(void)
{
	SET_BIT(UCSRB_REG,RXEN);
}

void USART_Init(USART_Config *ConfigPtr)
{
	USART_BoudRateSelection(ConfigPtr->BoudRate);
	USART_CharacterSizeSelection(ConfigPtr->CharacterSize);
	USART_ParityModeSelection(ConfigPtr->ParityMode);
	USART_StopBitSelection(ConfigPtr->StopBit);
    TransimationEnable();
	RecptionEnable();
}

void USART_TransimateData(u8 Data)
{
	while(!READ_BIT(UCSRA_REG,UDRE));
	UDR_REG=Data;
}

u8 USART_RecieveData(void)
{
	u8 data;
	while(!READ_BIT(UCSRA_REG,RXC));
	data=UDR_REG;
	return data;
}

void USART_TransimateString(u8 *str,u8 size)
{
	for(u8 i=0;i<size;i++)
	{
		USART_TransimateData(str[i]);
	}
}

u8 *USART_RecieveString(void)
{
	static u8 *str;
	u8 i=0;
	while(str[i]!=NULL)
	{
		str[i]=USART_RecieveData();
		i++;
	}

	return str;
}

void USART_TransimateAsyncronous(u8 *ptr,u8 size,void(*USART_TansimateCallBack)(void))
{
	if(BusyFlagTransimate==0)
	{
		BusyFlagTransimate=1;
		USART_TransimateInterrupt=USART_TansimateCallBack;
		SizeStringTransimate=size;
		StringTransimate=ptr;
		if(!READ_BIT(UCSRA_REG,UDRE))
		{
			UDR_REG=ptr[indexTransimate];
		}
		SET_BIT(UCSRB_REG,UDRIE);

	}

}

void USART_RecieveAsyncronous(u8 *ptr,u8 size,void(*USART_RecieveCallBack)(void))
{
	if(BusyFlagRecieve==0)
	{
		BusyFlagRecieve=1;
		USART_RecieveInterrupt=USART_RecieveCallBack;
		SizeStringRecieve=size;
		StringRecieve=ptr;

		if(!READ_BIT(UCSRA_REG,RXC))
		{

			ptr[indexRecieve]=UDR_REG;
			indexRecieve+=1;
		}
		SET_BIT(UCSRB_REG,RXCIE);
	}
}

void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
	indexTransimate++;
	if(indexTransimate!=SizeStringTransimate)
	{
		UDR_REG=StringTransimate[indexTransimate];
	}
	else
	{
		CLEAR_BIT(UCSRB_REG,UDRIE);
		BusyFlagTransimate=0;
		USART_TransimateInterrupt();

	}
}

void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
	StringRecieve[indexRecieve]=UDR_REG;
	indexRecieve++;
	if(indexRecieve!=SizeStringRecieve)
	{


	}
	else
	{
		CLEAR_BIT(UCSRB_REG,RXCIE);
		BusyFlagRecieve=0;
		USART_RecieveInterrupt();
	}
}
