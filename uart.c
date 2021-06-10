
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "uart.h"
#include "USBHost.h"
#include "config.h"

uint8_t __xdata uartRxBuff[64];
static uint8_t __xdata rxPos = 0;

void CH559UART0Interrupt(void) __interrupt INT_NO_UART0 __using 1
{
	while (RI)
	{
		RI = 0;
		uartRxBuff[rxPos] = SBUF;
		rxPos++;
		if (rxPos >= 64)
		{
			rxPos = 0;
		}
	}

	if (TI)
	{
		TI = 0;
	}
}

#ifndef USB_PROTOCOL_DEBUG
#define sendByte(b) UART0Send(b)
#else
#define sendByte(b) 
#endif

#define END 0xC0
#define ESC 0xDB
#define ESC_END 0xDC
#define ESC_ESC 0xDD

void SendSLIP(uint8_t b)
{
	switch (b)
	{
	case END:
		sendByte(ESC);
		sendByte(ESC_END);
		break;
	case ESC:
		sendByte(ESC);
		sendByte(ESC_ESC);
		break;

	default:
		sendByte(b);
		break;
	}
}

void SendSLIPEnd() { sendByte(END); }

void sendProtocolMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer){
    unsigned short i;
	SendSLIP(length);
	SendSLIP((unsigned char)(length>>8));
	SendSLIP(msgtype);
	SendSLIP(type);
	SendSLIP(device);
	SendSLIP(endpoint);
	SendSLIP(0);
	SendSLIP(0);
	SendSLIP(0);
	SendSLIP(0);
	for (i = 0; i < length; i++)
	{
		SendSLIP(msgbuffer[i]);
	}
	SendSLIPEnd();
}

void sendHidPollMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
    unsigned short i;
	SendSLIP(length);
	SendSLIP((unsigned char)(length>>8));
	SendSLIP(msgtype);
	SendSLIP(type);
	SendSLIP(device);
	SendSLIP(endpoint);
	SendSLIP(idVendorL);
	SendSLIP(idVendorH);
	SendSLIP(idProductL);
	SendSLIP(idProductH);
	for (i = 0; i < length; i++)
	{
		SendSLIP(msgbuffer[i]);
	}
	SendSLIPEnd();
}

void processUart()
{
	if (rxPos > 0 && uartRxBuff[rxPos - 1] == '\n')
	{
		// for (uint8_t i = 0; i < rxPos; i++)
		// {
		// 	printf("0x%02X ", uartRxBuff[i]);
		// }
		// printf("\n");
		if (uartRxBuff[0] == 'k')
		{
			//if(uartRxBuff[1]==0x61)LED=0;
			//if(uartRxBuff[1]==0x73)LED=1;
			if (uartRxBuff[1] == 'b')
			{
				uninitClock();
				runBootloader();
			}
			else if (uartRxBuff[1] == 'r')
			{
				SAFE_MOD = 0x55;
				SAFE_MOD = 0xaa;
				GLOBAL_CFG |= bSW_RESET;
			}
		}
		else if (uartRxBuff[0] & 0x80)
		{
			sendHidOutReport((uartRxBuff[0] & 0x60) >> 5,
							 (uartRxBuff[0]) & 0x1F);
		}
		rxPos = 0;
	}
}