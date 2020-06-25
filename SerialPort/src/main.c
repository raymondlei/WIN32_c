/*
 ============================================================================
 Name        : Timer.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "timer.h"
#include "soft_timer.h"
#include "SerialPort.h"

void timer_handler(void);
void sysTick_handler(void);
void app_terminate(void);

static int var = 0;
static SoftTimer_st _sysTick;
static SerialPortT _serialPort;

int main(void)
{
    if(SerialPort_Init(&_serialPort) == false)
    {
        printf("Error opening serial port\n");
        return EXIT_FAILURE;
    }

    printf("Opening serial port successful\n");
    if (start_timer(10, &timer_handler))
    {
        printf("\n timer error\n");
        return (1);
    }

    SoftTimer_default(&_sysTick, 10);
    _sysTick.on_tick_handler = sysTick_handler;

    printf("\npress ctl-c to quit.\n");
    while (1)
    {
        if (var > 50)
        {
            break;
        }
    }

    stop_timer();
    app_terminate();

    return EXIT_SUCCESS;
}

void sysTick_handler(void)
{
    printf("Sys Tick %d \r\n", var++);
    SerialPort_WriteByte(&_serialPort, 'c');
}

void timer_handler(void)
{
    SoftTimer_Tick(&_sysTick);
}

void app_terminate(void)
{
    SerialPortT* com_handle = &_serialPort;

    if(SerialPort_isOpened(com_handle))
    {
        printf("Closing serial port\n");
        SerialPort_PurgeAll(com_handle);
        SerialPort_Close(com_handle);
    }
}
