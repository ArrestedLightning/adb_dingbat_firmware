
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

void sendProtocolMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer){
    unsigned short i;
    UART1Send(0xFE);	
	UART1Send(length);
	UART1Send((unsigned char)(length>>8));
	UART1Send(msgtype);
	UART1Send(type);
	UART1Send(device);
	UART1Send(endpoint);
	UART1Send(0);
	UART1Send(0);
	UART1Send(0);
	UART1Send(0);
	for (i = 0; i < length; i++)
	{
		UART1Send(msgbuffer[i]);
	}
	UART1Send('\n');
}

void sendHidPollMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
    unsigned short i;
    UART1Send(0xFE);	
	UART1Send(length);
	UART1Send((unsigned char)(length>>8));
	UART1Send(msgtype);
	UART1Send(type);
	UART1Send(device);
	UART1Send(endpoint);
	UART1Send(idVendorL);
	UART1Send(idVendorH);
	UART1Send(idProductL);
	UART1Send(idProductH);
	for (i = 0; i < length; i++)
	{
		UART1Send(msgbuffer[i]);
	}
	UART1Send('\n');
}