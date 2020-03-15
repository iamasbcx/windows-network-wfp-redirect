#pragma once

#define NETWFPRDR						  L"netwfprdr"

#define NETWFPRDR_NT_DEVICE_NAME          L"\\Device\\netwfprdr"
#define NETWFPRDR_DOS_DEVICE_NAME         L"\\??\\netwfprdr"



typedef struct _NET_IO_REQUEST {

	int    test;


} NET_IO_REQUEST, * PNET_IO_REQUEST;



#define IOCTL_NET_FILTER_TEST	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x100, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_NET_FILTER_START	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x110, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_NET_FILTER_STOP	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x111, METHOD_BUFFERED, FILE_ANY_ACCESS)