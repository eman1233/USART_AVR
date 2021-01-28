/*
 * USART_MemMap.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: inter
 */

#ifndef USART_MEMMAP_H_
#define USART_MEMMAP_H_

#include "Std_Types.h"

#define UDR_REG     *((volatile u8*)0x2C)

#define UCSRA_REG   *((volatile u8*)0x2B)
#define RXC   7
#define TXC   6
#define UDRE  5
#define FE    4
#define DOR   3
#define PE    2
#define U2X   1
#define MPCM  0

#define UCSRB_REG   *((volatile u8*)0x2A)
#define RXCIE  7
#define TXCIE  6
#define UDRIE  5
#define RXEN   4
#define TXEN   3
#define UCSZ2  2
#define RXB8   1
#define TXB8   0

#define UCSRC_REG   *((volatile u8*)0x40)
#define URSEL  7
#define UMSEL  6
#define UPM1   5
#define UPM0   4
#define USBS   3
#define UCSZ1  2
#define UCSZ0  1
#define UCPOL  0

#define UBRRL_REG   *((volatile u8*)0x29)

#define UBRRH_REG   *((volatile u8*)0x40)
//#define URSEL_RRH  7

#define READ_BIT(PORT,BIT)    ((PORT>>BIT)&1)
#define SET_BIT(PORT,BIT)    (PORT=PORT|(1<<BIT))
#define CLEAR_BIT(PORT,BIT)  (PORT&=~(1<<BIT))
#define TOGGLE_BIT(PORT,BIT) (PORT^=(1<<BIT))


#endif /* USART_MEMMAP_H_ */
