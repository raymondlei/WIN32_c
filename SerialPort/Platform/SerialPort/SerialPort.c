/*
 * SerialPort.c
 *
 *  Created on: Feb 6, 2017
 *      Author: leira
 */

/*
 * URL:
 *
 * http://www.flounder.com/serial.htm
 * http://xanthium.in/Serial-Port-Programming-using-Win32-API
 * */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>

#include <SerialPort.h>

static void _defaultSettings(SerialPortT* handle);
static void _defaultTimeout(SerialPortT* handle);
static void _attachEvent(SerialPortT* handle, uint16_t bit_fields);

bool SerialPort_Init(SerialPortT* handle)
{
    SerialPort_Open(handle, COM_11);
    SerialPort_Configure(handle);

    if(SerialPort_isOpened(handle))
        return true;
    else
        return false;
}

void SerialPort_Open(SerialPortT* handle, char* port_name)
{
    handle->hComm = CreateFile(port_name,  //see #define COM_xx
                               GENERIC_READ | GENERIC_WRITE, //Read/Write
                               0,               // No Sharing
                               NULL,            // No Security
                               OPEN_EXISTING,   // Open existing port only
                               //0,               // Non Overlapped I/O
                               FILE_FLAG_OVERLAPPED,  // open asynchronous
                               NULL);
    if(handle->hComm == INVALID_HANDLE_VALUE)
    {
        handle->error = GetLastError();
        handle->isOpened = false;
    }
    else
    {
        handle->isOpened = true;
    }
}

void SerialPort_PurgeAll(SerialPortT* handle)
{
    bool status = PurgeComm(handle->hComm, PURGE_ALL_FLAGS);
}

void SerialPort_Close(SerialPortT* handle)
{
    CloseHandle(handle->hComm);
}

bool SerialPort_isOpened(SerialPortT* handle)
{
    return handle->isOpened;
}

void SerialPort_Configure(SerialPortT* handle)
{
    WINBOOL status;

    _defaultSettings(handle);
    status = SetCommState(handle->hComm, &handle->portConfig);

    _defaultTimeout(handle);
    status = SetCommTimeouts(handle->hComm, &handle->timeout);

    _attachEvent(handle, EV_RXCHAR);
}

void SerialPort_WriteByte(SerialPortT* handle, unsigned char ch)
{
    char lpBuffer[] = "A";
    DWORD bytesToWrite;
    DWORD bytesWritten = 0;
    bytesToWrite = sizeof(lpBuffer);
    lpBuffer[0] = ch;

    WINBOOL status = WriteFile(handle->hComm,    //Handle of the Serial Port
                               lpBuffer,         //Data buffer
                               bytesToWrite,     //Number of bytes to write
                               &bytesWritten,    //Bytes written
                               NULL);
}

unsigned char SerialPort_ReadByte(SerialPortT* handle)
{
    char rx_byte;
    DWORD bytesRead;

    ReadFile(handle->hComm,    //Handle of the Serial Port
            &rx_byte,          //receiving character
            sizeof(rx_byte),   //size of receiving character
            &bytesRead,      //number of bytes read
            NULL);

    return (unsigned char)rx_byte;
}

static void _attachEvent(SerialPortT* handle, uint16_t bit_fields)
{
    WINBOOL status;
    DWORD eventMask;

    /* Flag Options
        EV_BREAK
        EV_CTS
        EV_DSR
        EV_ERR
        EV_EVENT1
        EV_EVENT2
        EV_PERR
        EV_RING
        EV_RLSD
        EV_RX80FULL
        EV_RXCHAR
        EV_RXFLAG
        EV_TXEMPTY
    */

    //1. Create an Event for a particular action like character reception,
    //   change in modem lines etc using SetCommMask() function
    status = SetCommMask(handle->hComm, bit_fields);

    //2. Ask windows to wait for the event set by SetCommMask() function
    //   using WaitCommEvent() and notify us when the condition happens.
    eventMask = WaitCommEvent(handle->hComm, &eventMask, NULL);
}

static void _defaultSettings(SerialPortT* handle)
{
    DCB params = {0};
    handle->portConfig = params;
    handle->portConfig.DCBlength = sizeof(params);

    WINBOOL status = GetCommState(handle->hComm, &handle->portConfig);

    handle->portConfig.BaudRate = CBR_19200;  //Baud Rate: 9600
    handle->portConfig.ByteSize = 8;         //Data size: 8
    handle->portConfig.StopBits = ONESTOPBIT;  //Stopbit: 1
    handle->portConfig.Parity = NOPARITY;    // Parity: none
}

static void _defaultTimeout(SerialPortT* handle)
{
    /*
     * URL: http://xanthium.in/Serial-Port-Programming-using-Win32-API
     */

    /*
     * ReadIntervalTimeout:      specifies the maximum time interval between arrival
     *                           of two bytes. If the arrival time exceeds these
     *                           limits the ReadFile() function returns.
     *
     * ReadTotalTimeoutConstant:  used to calculate the total time-out period for
     *                            read operations. For each read operation, this
     *                            value is added to the product of the
     *                            ReadTotalTimeoutMultiplier member and the
     *                            requested number of bytes.
     *
     * ReadTotalTimeoutMultiplier: used to calculate the total time-out period for
     *                             read operations. For each read operation, this
     *                             value is multiplied by the requested number of
     *                             bytes to be read.
     *
     * WriteTotalTimeoutConstant:  similar to ReadTotalTimeoutConstant but for write
     *                             operation.
     *
     * WriteTotalTimeoutMultiplier: similar to ReadTotalTimeoutMultiplier but for
     *                              write operation.
     * After this you have to set the values using SetCommTimeouts() function.
     */

    handle->timeout.ReadIntervalTimeout = 50;  //in ms
    handle->timeout.ReadTotalTimeoutConstant = 50;  //in ms
    handle->timeout.ReadTotalTimeoutMultiplier = 10; //in ms
    handle->timeout.WriteTotalTimeoutConstant = 50;  //in ms
    handle->timeout.WriteTotalTimeoutMultiplier = 10; //in ms
}
