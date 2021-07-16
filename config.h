//add your personal defines here 
#pragma once

#define MAJOR_VER 0x01
#define MINOR_VER 0x01
#define REVISION 0x00

// 12MHz * 20 / 5 = 48MHz
// PLL selected to select 115200 baud at SYS_CLK_DIV=32
#define PLL_MULT 20
#define USB_CLK_DIV 5
#define SYS_CLK_DIV 5

#define UART_BAUDRATE 115200

#define USB_H0_DISABLED
// #define DEBUG_ENABLE

// Output debug message through UART0(atmega32u4)
// #define USB_PROTOCOL_DEBUG