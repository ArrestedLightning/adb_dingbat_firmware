@echo off 
set project_name=CH559USB
set xram_size=0x0800
set xram_loc=0x0600
set code_size=0xEFFF
set dfreq_sys=48000000

if not exist "config.h" echo //add your personal defines here > config.h

SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys% -DKEYBOARD_QUANTIZER_REV4 main.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys% -DKEYBOARD_QUANTIZER_REV4 util.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys% -DKEYBOARD_QUANTIZER_REV4 USBHost.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys% -DKEYBOARD_QUANTIZER_REV4 uart.c

SDCC\bin\sdcc main.rel util.rel USBHost.rel uart.rel -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys% -DKEYBOARD_QUANTIZER_REV4  -o %project_name%.ihx

SDCC\bin\packihx %project_name%.ihx > %project_name%.hex

SDCC\bin\hex2bin -c %project_name%.hex

REM del %project_name%.lk
REM del %project_name%.map
REM del %project_name%.mem
REM del %project_name%.ihx

del *.asm
REM del *.lst
del *.rel
del *.rst
del *.sym


Rem This tool flashes the bin file directly to the ch559 chip, you need to install the libusb-win32 driver with the zadig( https://zadig.akeo.ie/ ) tool so the tool can access the usb device
chflasher.exe %project_name%.bin
