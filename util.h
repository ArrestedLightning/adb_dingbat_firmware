#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdio.h>
#include "config.h"
#ifdef DEBUG_ENABLE
#define DEBUG_OUT(...) printf(__VA_ARGS__);
#else
#define DEBUG_OUT(...) delayUs(50);
#endif

extern __xdata uint16_t slowClockDuration;

void uninitClock();
void initClock();
uint16_t getSlowClockDuration();
void resetSlowClockDuration();
void syncSof();
void delayUsShort(__data unsigned short n);
void delayUsLong(__data unsigned short n);
static inline void delayUs(__data unsigned short n)
{
    if (n < 50)
    {
        delayUsShort(n);
    }
    else
    {
        slowClockDuration += n;
        delayUsLong((n * SYS_CLK_DIV + 16) / 32);
    }
}

void delay(__data unsigned short n);
void uninitUART0();
void initUART0(unsigned long baud, int alt);
unsigned char UART0Receive();
void UART0Send(unsigned char b);
void initUART1(unsigned long baud);
unsigned char UART1Receive();
void UART1Send(unsigned char b);

void setTMR3TimeOut(uint8_t turn_off_100ms);
void turnOffDummyLoad();

void initSPI0();
void sendSPI0(uint8_t data);
uint8_t recvSPI0();
static inline void ENABLE_SPI0_CLK()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xaa;
    SLEEP_CTRL &= ~bSLP_OFF_SPI0;
    SAFE_MOD = 0xff;
}

static inline void DISABLE_SPI0_CLK()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xaa;
    SLEEP_CTRL |= bSLP_OFF_SPI0;
    SAFE_MOD = 0xff;
}

#define PIN_MODE_INPUT 0
#define PIN_MODE_INPUT_PULLUP 1
#define PIN_MODE_OUTPUT 2
#define PIN_MODE_OUTPUT_OPEN_DRAIN 3
#define PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK 4
#define PIN_MODE_INPUT_OUTPUT_PULLUP 5
#define PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK 6
void pinMode(unsigned char port, unsigned char pin, unsigned char mode);

typedef void(* __data FunctionReference)();
extern FunctionReference runBootloader;

#endif