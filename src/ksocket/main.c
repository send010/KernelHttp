#include "kHTTP.h"


#define DebuggerPrint(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, __VA_ARGS__);

NTSTATUS
NTAPI
DriverEntry(
  _In_ PDRIVER_OBJECT DriverObject,
  _In_ PUNICODE_STRING RegistryPath
)
{
  //UNREFERENCED_PARAMETER(DriverObject);
  //UNREFERENCED_PARAMETER(RegistryPath);

  NTSTATUS Status;



  //
  // Initialize KSOCKET.
  //

  Status = KsInitialize();

  if (!NT_SUCCESS(Status))
  {
    return Status;
  }
  char url[500] = "http://user.bsradar.cc/config/user/GetUserInfo";
  char data[1024] = {0};
  Status = HttpGet(data, url);
  if (!NT_SUCCESS(Status))
  {
    DebuggerPrint("[BuMo] Get Error %d  \r\n", Status);
    return STATUS_UNSUCCESSFUL;
  }

  DebuggerPrint("[BuMo] Data = %s  \r\n", data);

  KsDestroy();

  return STATUS_UNSUCCESSFUL;
}
