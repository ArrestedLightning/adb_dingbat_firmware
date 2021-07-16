typedef unsigned char *PUINT8;
typedef unsigned char __xdata *PUINT8X;
typedef const unsigned char __code *PUINT8C;
typedef unsigned char __xdata UINT8X;
typedef unsigned char  __data             UINT8D;

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "config.h"

SBIT(LED, 0x90, 6);

void CH559UART0Interrupt(void) __interrupt INT_NO_UART0 __using 1;
void CH559TIMER3Interrupt(void) __interrupt INT_NO_TMR3 __using 2;

void main()
{
    unsigned char s;

    turnOffDummyLoad();
    // disable load monitor pin
    P3_PU &= ~(1 << 5);
    initClock();
    initSPI0();
    // initUART0(UART_BAUDRATE, 1);
#ifdef DEBUG_ENABLE
    initUART1(UART_BAUDRATE);
#endif
    DEBUG_OUT("Startup\n");

    SAFE_MOD = 0x55;
    SAFE_MOD = 0xaa;
    SLEEP_CTRL |= (bSLP_OFF_ADC | bSLP_OFF_P1S1 | bSLP_OFF_SPI0 | bSLP_OFF_TMR3 | bSLP_OFF_LED);
#ifndef DEBUG_ENABLE
    SLEEP_CTRL |= bSLP_OFF_UART1;
#endif
    SAFE_MOD = 0xff;

    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
    DEBUG_OUT("Ready\n");

    bool start_flag = true;

    TMOD |= bT0_M0;
    T2MOD &= ~bT0_CLK;
    TR0 = 1;

    delay(100);

    while(1)
    {
        TH0 = 0;
        TL0 = 0;
        slowClockDuration = 0;

        if((P5_IN & (1 << 1)))
        {
            turnOffDummyLoad();
            uninitClock();
            runBootloader();
        }

        ENABLE_SPI0_CLK();

        uint8_t ack = 0xff;
        uint16_t retry = 0;
        do
        {
            SCS = 0;
            delayUs(1);

            ack = recvSPI0();
            if (ack != 0xff)
            {
                // recv 3byte
                processSPI();

                if (start_flag)
                {
                    sendProtocolMSG(MSG_TYPE_STARTUP, 0x00, MAJOR_VER, MINOR_VER, REVISION, 0);
                    start_flag = false;
                    delayUs(1);
                    ack = 0xff;
                    retry = 0;
                    SCS = 1;
                    continue;
                } else {
                    break;
                }
            }

            SCS = 1;
            delayUs(1);
        } while (++retry < 5000 && ack == 0xff);

        if (ack == 0xff) {
            continue;
        }

        syncSof();
        // processUart();
        s = checkRootHubConnections();
        pollHIDdevice();

        delayUs(1);
        SCS = 1;
        DISABLE_SPI0_CLK();

        uint16_t tcnt = TL0 | (TH0 << 8);
        uint16_t t = (tcnt >> 2) + slowClockDuration;

        if (t < 920) {
            delayUs(920 - t);
        }
    }
}