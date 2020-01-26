#pragma once


extern "C"
{
#pragma warning(push)
#pragma warning(disable: 4201) /// NAMELESS_STRUCT_UNION
#pragma warning(disable: 4324) /// STRUCTURE_PADDED




#include <ntifs.h>                    /// IfsKit\Inc
#include <ntddk.h>                    /// Inc
//#include <wdf.h>                      /// Inc\WDF\KMDF\1.9
#include <ndis.h>                     /// Inc
#include <fwpmk.h>                    /// Inc
#include <fwpsk.h>                    /// Inc
#include <netioddk.h>                 /// Inc
#include <ntintsafe.h>                /// Inc
#include <ntstrsafe.h>                /// Inc
#include <stdlib.h>                   /// SDK\Inc\CRT


#include "debug.h"

#pragma warning(pop)
}
