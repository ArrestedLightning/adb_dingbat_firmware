typedef unsigned char *PUINT8;
typedef unsigned char __xdata *PUINT8X;
typedef const unsigned char __code *PUINT8C;
typedef unsigned char __xdata UINT8X;
typedef unsigned char  __data UINT8D;

#include "globals.h"

void main()
{
    unsigned char s;
    initClock();
    initPins();
    initUART0(UART_BAUDRATE, true);
//    initUART1(UART_BAUDRATE);
    DEBUG_OUT("\nADB Dingbat!\n");
    DEBUG_OUT("Startup\n");
    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
    DEBUG_OUT("Ready\n");
    //sendProtocolMSG(MSG_TYPE_STARTUP,0, 0x00, 0x00, 0x00, 0);
    while(1)
    {
        processUart();
        s = checkRootHubConnections();
        pollHIDdevice();
    }
}
