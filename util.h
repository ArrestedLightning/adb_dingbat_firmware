#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdio.h>
#include "config.h"
#include "pins.h"
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
void initUART0(unsigned long baud, unsigned char alt);
unsigned char UART0Receive();
void UART0Send(unsigned char b);
#ifdef USE_UART1
void initUART1(unsigned long baud);
unsigned char UART1Receive();
void UART1Send(unsigned char b);
#endif

void setTMR3TimeOut(uint8_t turn_off_100ms);

#ifdef USE_SPI0
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
#endif

void initPins(void);

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

SBIT(LED_1, 0xA0, LED_1_PIN); //LED1 on P2.4
SBIT(LED_2, 0xC0, LED_2_PIN); //LED2 on P4.5
SBIT(LED_3, 0xC0, LED_3_PIN); //LED3 on P4.2
SBIT(LED_4, 0x80, LED_4_PIN); //LED4 on P0.0

SBIT(M1, 0xC0, EXTRA_MOUSE1_PIN);//MOUSE1 on P4.1
SBIT(M2, 0xC0, EXTRA_MOUSE2_PIN);//MOUSE2 on P4.0
SBIT(M3, 0xA0, EXTRA_MOUSE3_PIN);//MOUSE3 on P2.0
SBIT(M4, 0xC0, EXTRA_MOUSE4_PIN);//MOUSE4 on P4.3
SBIT(M5, 0x80, EXTRA_MOUSE5_PIN);//MOUSE5 on P0.5
SBIT(M6, 0xB0, EXTRA_MOUSE6_PIN);//MOUSE6 on P3.0

#endif
