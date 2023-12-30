#include "globals.h"

FunctionReference runBootloader = (FunctionReference)0xF400;

#ifndef FREQ_SYS
#define	FREQ_SYS	48000000
#endif

void initClock()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

	//start external oscillator
	CLOCK_CFG |= bOSC_EN_XT;

	// wait stable oscillation
	// Do not use delayUsLong
	for (int i = 0; i < 10; i++)
	{
		delayUsShort(1000);
	}

    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

	// switch to external oscillator
	CLOCK_CFG &= ~(bOSC_EN_INT);

	// Do not use delayUsLong
	for (int i = 0; i < 10; i++)
	{
		delayUsShort(1000);
	}

    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;
	CLOCK_CFG |= SYS_CLK_DIV;
	PLL_CFG = ((PLL_MULT << 0) | (USB_CLK_DIV << 5)) & 255;

    SAFE_MOD = 0xFF;

	// delay(7);
	// Do not use delayUsLong
	for (int i = 0; i < 7; i++)
	{
		delayUsShort(1000);
	}
}

void uninitClock()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

	// restore initial value
	CLOCK_CFG = (unsigned char)(1 << 7) | (0b11000);
	PLL_CFG = 0b11011000;

    SAFE_MOD = 0xFF;

	// delay(7);
	// Do not use delayUsLong
	for (int i = 0; i < 7; i++)
	{
		delayUsShort(1000);
	}
}

/**
 * Initialize UART0 port with given baud rate
 * pins: tx = P3.1 rx = P3.0
 * alt != 0 pins: tx = P0.2 rx = P0.3
 */

void initUART0(unsigned long baud, unsigned char alt)
{
	unsigned long x;
	if(alt)
	{
		PORT_CFG |= bP0_OC;
		P0_DIR |= bTXD_;
		P0_PU |= bTXD_ | bRXD_;
		PIN_FUNC |= bUART0_PIN_X;
	}

 	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;
   //RCLK = 0;
    //TCLK = 0;
    PCON |= SMOD;//UART Fast Mode
    x = (((unsigned long)FREQ_SYS / 8) / baud + 1) / 2;

    TMOD = MASK_T1_MOD & (bT1_M1 | ~bT1_M0);
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;
    TH1 = (256 - x) & 255;
    TR1 = 1;
    TI = 1;

	// Enable serial interrupt
	//ES = 1;
	EA = 1;
}

unsigned char UART0Receive()
{
    while(RI == 0);
    RI = 0;
    return SBUF;
}

void UART0Send(unsigned char b)
{
    SBUF = b;
    while (!TI);
    TI = 0;
}
#ifdef USE_UART1
void initUART1(unsigned long baud)
{
	// Allow access to RBR, THR, IER, ADR
	SER1_LCR =  0;

	// set pin mode, RXD->P2.6 TXD->P2.7
	SER1_IER = bIER_PIN_MOD1;

	// Allow access to Baudrate control register
	SER1_LCR = bLCR_DLAB;

	// set baudrate divisor
	SER1_DIV = 1;
	unsigned short div = (FREQ_SYS / 8) / SER1_DIV / baud;

	SER1_DLL = div & 0xFF;
	SER1_DLM = (div >> 8) & 0xFF;

	// enable FIFO
	SER1_FCR = bFCR_FIFO_EN;

	// set data format: 8bit, none, 1bit
	// Allow access to RBR, THR, IER, ADR
	SER1_LCR = 0x03;
}

unsigned char UART1Receive()
{
	// Wait data available
	while (!(SER1_LSR & bLSR_DATA_RDY))
	{
		continue;
	}

	return SER1_RBR;
}

void UART1Send(unsigned char b)
{
	// Wait FIFO empty
	while (!(SER1_LSR & bLSR_T_FIFO_EMP))
	{
		continue;
	}

	// Send to FIFO
	SER1_THR = b;
}
#endif

#ifdef USE_SPI0
void initSPI0()
{
	ENABLE_SPI0_CLK();

	PORT_CFG &= ~bP1_OC;	// Push-pull output
    P1_DIR |= (bSCK | bMOSI | bSCS);
	P1_IE |= bMISO;

	SPI0_SETUP &= ~(bS0_MODE_SLV | bS0_BIT_ORDER); // Master, MSB
	SPI0_CTRL |= bS0_MOSI_OE | bS0_SCK_OE | bS0_MST_CLK; // Enable MOSI, SCK output, mode 3
	SPI0_CTRL &= ~(bS0_DATA_DIR | bS0_2_WIRE); // Full-duplex

	SPI0_CK_SE = 6; // 48/6 = 8MHz
	SPI0_CTRL &= ~bS0_CLR_ALL; // release clear

	SCS = 1;
}

void sendSPI0(uint8_t data)
{
	SPI0_DATA = data;
	while (S0_FREE == 0)
	{
		continue;
	}
}

uint8_t recvSPI0()
{
	SPI0_DATA = 0xC0;
	while (S0_FREE == 0)
	{
		continue;
	}
	return SPI0_DATA;
}
#endif

void initPins(void) {
  //Start with LEDs turned off (active low)
  LED_1 = 1;
  LED_2 = 1;
  LED_3 = 1;
  LED_4 = 1;

  P4_DIR = (1 << LED_2_PIN) | (1 << LED_3_PIN);

}
/**
* #define PIN_MODE_INPUT 0
* #define PIN_MODE_INPUT_PULLUP 1
* #define PIN_MODE_OUTPUT 2
* #define PIN_MODE_OUTPUT_OPEN_DRAIN 3
* #define PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK 4
* #define PIN_MODE_INPUT_OUTPUT_PULLUP 5
* #define PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK 6
 */
void pinMode(unsigned char port, unsigned char pin, unsigned char mode)
{
	volatile unsigned char *dir[] = {&P0_DIR, &P1_DIR, &P2_DIR, &P3_DIR, &P4_DIR};
	volatile unsigned char *pu[] = {&P0_PU, &P1_PU, &P2_PU, &P3_PU, &P4_PU};
	switch (mode)
	{
	case PIN_MODE_INPUT: //Input only, no pull up
	  if (port < 4) {
	    PORT_CFG &= ~(bP0_OC << port);
	  }
		*dir[port] &= ~(1 << pin);
		*pu[port] &= ~(1 << pin);
		break;
	case PIN_MODE_INPUT_PULLUP: //Input only, pull up
	  if (port < 4) {
	    PORT_CFG &= ~(bP0_OC << port);
	  }
		*dir[port] &= ~(1 << pin);
		*pu[port] |= 1 << pin;
		break;
	case PIN_MODE_OUTPUT: //Push-pull output, high and low level strong drive
	  if (port < 4) {
	    PORT_CFG &= ~(bP0_OC << port);
	  }
		*dir[port] |= 1 << pin;
		break;
	case PIN_MODE_OUTPUT_OPEN_DRAIN: //Open drain output, no pull-up, support input
	  if (port < 4) {
	    PORT_CFG |= (bP0_OC << port);
	  }
		*dir[port] &= ~(1 << pin);
		*pu[port] &= ~(1 << pin);
		break;
	case PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK: //Open-drain output, no pull-up, only drives 2 clocks high when the transition output goes from low to high
	  if (port < 4) {
	    PORT_CFG |= (bP0_OC << port);
	  }
		*dir[port] |= 1 << pin;
		*pu[port] &= ~(1 << pin);
		break;
	case PIN_MODE_INPUT_OUTPUT_PULLUP: //Weakly bidirectional (standard 51 mode), open drain output, with pull-up
	  if (port < 4) {
	    PORT_CFG |= (bP0_OC << port);
	  }
		*dir[port] &= ~(1 << pin);
		*pu[port] |= 1 << pin;
		break;
	case PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK: //Quasi-bidirectional (standard 51 mode), open-drain output, with pull-up, when the transition output is low to high, only drives 2 clocks high
	  if (port < 4) {
	    PORT_CFG |= (bP0_OC << port);
	  }
		*dir[port] |= 1 << pin;
		*pu[port] |= 1 << pin;
		break;
	default:
		break;
	}
}

static void turnOffTimer3Clock() {
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xaa;
	SLEEP_CTRL |= bSLP_OFF_TMR3;
    SAFE_MOD = 0xff;
}

static void turnOnTimer3Clock() {
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xaa;
	SLEEP_CTRL &= ~bSLP_OFF_TMR3;
    SAFE_MOD = 0xff;
}

static void setTimer3Prescaler(uint16_t div)
{
	T3_SETUP |= bT3_EN_CK_SE;
	T3_CK_SE_L = div & 0xff;
	T3_CK_SE_H = div >> 8;
	T3_SETUP &= ~bT3_EN_CK_SE;
}

static uint16_t __xdata s_timeout_100ms;
static uint8_t __xdata s_turn_off_100ms;
static void tmr3Start()
{
	// if (s_timeout_100ms > 0) return;

	turnOnTimer3Clock();

	// clock to 10kHz
	setTimer3Prescaler(480);

	// clear counter
	T3_CTRL &= ~bT3_CNT_EN;
	T3_CTRL &= ~bT3_MOD_CAP;
	T3_CTRL |= bT3_CLR_ALL;
	T3_CTRL &= ~bT3_CLR_ALL;

	// 100ms timeout
	T3_END_L = (10000 - 1) & 0xff;
	T3_END_H = (10000 - 1) >> 8;

	// enable interrupt
    T3_STAT |= bT3_IF_DMA_END | bT3_IF_FIFO_OV | bT3_IF_FIFO_REQ | bT3_IF_ACT | bT3_IF_END;
	T3_SETUP |= bT3_IE_END;
	IP_EX |= bIP_TMR3;
	IE_TMR3 = 1;
	EA = 1;

	// start counter
	T3_CTRL |= bT3_CNT_EN;
}

void CH559TIMER3Interrupt(void) __interrupt INT_NO_TMR3 __using 2
{
	T3_CTRL &= ~bT3_CNT_EN; // stop timer
	T3_STAT |= bT3_IF_END; // clear interrupt flag

	--s_timeout_100ms;

	if (s_timeout_100ms == s_turn_off_100ms)
	{
	}
	else if (s_timeout_100ms == 0)
	{
		// disable interrupt
		IE_TMR3 = 0;
		// reset timer
		T3_CTRL |= bT3_CLR_ALL;

		turnOffTimer3Clock();

		return;
	}

	T3_CTRL |= bT3_CNT_EN; // restart timer
}

void setTMR3TimeOut(uint8_t turn_off_100ms)
{
	if (s_timeout_100ms)
	{
		return;
	}

	// set timeout
	if (turn_off_100ms > 100)
	{
		turn_off_100ms = 100;
	}
	s_timeout_100ms = turn_off_100ms * 2;
	s_turn_off_100ms = s_timeout_100ms - turn_off_100ms;

	if (s_timeout_100ms > 0) {
		tmr3Start();

//		turnOnDummyLoad();
//
//		// send message dummy load start
//		sendProtocolMSG(MSG_TYPE_DUMMY_LOAD, 0x00, 1, s_timeout_100ms, s_turn_off_100ms, 0);
	} else {
		// send message dummy load stop
//		sendProtocolMSG(MSG_TYPE_DUMMY_LOAD, 0x00, 0, 0, 0, 0);
	}
}

/*
unsigned char getPortAddress(unsigned char port)
{
	const unsigned char portAddr[] = {0x80, 0x90, 0xA0, 0xB0, 0xC0};
	return portAddr[port];
}

unsigned char digitalRead(unsigned char port, unsigned char pin)
{

}
*/
/**
 * stdio printf directed to UART0 using putchar and getchar
 */

int putchar(int c)
{
#ifdef USE_UART1
	UART1Send(c);
#else
	UART0Send(c);
#endif
    return c;
}

int getchar()
{
#ifdef USE_UART1
	return UART1Receive();
#else
	return UART0Receive();
#endif
}

static inline void slowClock()
{

	// change timer3 freq
	setTimer3Prescaler(480 * SYS_CLK_DIV / 32);

	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG &= ~MASK_SYS_CK_DIV; // div = 32

	SAFE_MOD = 0xFF;
}

static inline void restoreClock()
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG |= SYS_CLK_DIV; // restore clock to 48MHz
	SAFE_MOD = 0xFF;

	// restore timer3 freq
	setTimer3Prescaler(480);
}

void syncSof()
{
	UIF_HST_SOF = 0;
	slowClock();
	while (UIF_HST_SOF == 0)
	{
		// delayUs(50);
		continue;
	}
	restoreClock();
}

__xdata uint16_t slowClockDuration;

void delayUsLong(__data unsigned short n_divided)
{
	TR0 = 0;
	slowClock();

	delayUsShort(n_divided);

	restoreClock();

	TR0 = 1;
}

/*******************************************************************************
* Function Name  : delayUs(UNIT16 n)
* Description    : us
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void	delayUsShort(__data unsigned short n)
{
	while (n)
	{  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 26000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 28000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 30000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 32000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 34000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 36000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 38000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 40000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 42000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 44000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 46000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 48000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 50000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 52000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 54000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 56000000
		++ SAFE_MOD;
#endif
#endif
		--n;
	}
}

/*******************************************************************************
* Function Name  : delay(UNIT16 n)
* Description    : ms
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void delay(__data unsigned short n)
{
	while (n)
	{
		delayUs(1000);
		--n;
	}
}
