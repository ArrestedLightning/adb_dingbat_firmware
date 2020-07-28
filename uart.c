
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "uart.h"

uint8_t __xdata uartRxBuff[64];
uint8_t __xdata rxPos = 0;


void processUart(){
    while(RI){
            RI=0;
            uartRxBuff[rxPos] = SBUF;
            if (uartRxBuff[rxPos]=='\n' || rxPos >= 64){
                for (uint8_t i = 0; i < rxPos; i ++ )
                    {
                        printf( "0x%02X ",uartRxBuff[i]);
                    }
                    printf("\n");
                if(uartRxBuff[0]=='k'){
                //if(uartRxBuff[1]==0x61)LED=0;
                //if(uartRxBuff[1]==0x73)LED=1;
                if(uartRxBuff[1]=='b')runBootloader();
                }
            rxPos=0;
            }else{
            rxPos++;
            }
        }
}

#define sendByte(b) UART1Send(b)

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

#define SendSLIPEnd() sendByte(END)

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