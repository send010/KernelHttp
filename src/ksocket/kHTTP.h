#pragma once
#include "berkeley.h"
#include "ksocket.h"

#define DebuggerPrint(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, __VA_ARGS__);
//#define DebuggerPrint(...);

NTSTATUS
NTAPI
KHttpInit(
  VOID
);


NTSTATUS NTAPI HttpGet(_Out_ char* recv_buffer, _In_ char* url);

NTSTATUS NTAPI HttpPost(_Out_ char* recv_buffer, _In_ char* url, _In_ char* data, _In_ char* contextType);
