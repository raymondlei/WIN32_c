/*
 ============================================================================
 Name        : SerialPort_C.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SerialPort.h>

void Test_OpenClose(void);

int main(void)
{
	puts("Serial Port in C"); /* prints Serial Port in C */

	Test_OpenClose();

	return EXIT_SUCCESS;
}

void Test_OpenClose(void)
{
    SerialPortT target;

    SerialPort_Create(&target, COM_04);
    SerialPort_Configure(&target);

    if(SerialPort_isOpened(&target))
    {
        printf("Error opening serial port\n");
    }
    else
    {
        printf("Opening serial port successful\n");
        SerialPort_WriteByte(&target, 'c');
    }

    if(SerialPort_isOpened(&target) == true)
    {
        printf("Closing serial port\n");
        SerialPort_PurgeAll(&target);
        SerialPort_Close(&target);
    }
}
