/*
 * SerialPort_Open.h
 *
 *  Created on: Feb 6, 2017
 *      Author: leira
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

#define  COM_01     "COM1"
#define  COM_02     "COM2"
#define  COM_03     "COM3"
#define  COM_04     "COM4"
#define  COM_05     "COM5"
#define  COM_06     "COM6"
#define  COM_07     "COM7"
#define  COM_08     "COM8"
#define  COM_09     "COM9"
#define  COM_10     "\\\\.\\COM10"
#define  COM_11     "\\\\.\\COM11"
#define  COM_12     "\\\\.\\COM12"
#define  COM_13     "\\\\.\\COM13"
#define  COM_14     "\\\\.\\COM14"
#define  COM_15     "\\\\.\\COM15"

#define PURGE_ALL_FLAGS    PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR

typedef struct
{
    HANDLE hComm;
    DCB portConfig;
    COMMTIMEOUTS timeout;
    bool isOpened;
    uint16_t error;
    DWORD bytesToRead;

} SerialPortT;

#ifdef __cplusplus
extern "C"
{
#endif

    extern bool SerialPort_Init(SerialPortT* handle);
    extern void SerialPort_Open(SerialPortT* handle, char* port_name);
    extern void SerialPort_PurgeAll(SerialPortT* handle);
    extern void SerialPort_Close(SerialPortT* handle);

    extern bool SerialPort_isOpened(SerialPortT* handle);

    extern void SerialPort_Configure(SerialPortT* handle);
    extern void SerialPort_WriteByte(SerialPortT* handle, unsigned char ch);
#ifdef __cplusplus
}
#endif
#endif /* SERIALPORT_H_ */
