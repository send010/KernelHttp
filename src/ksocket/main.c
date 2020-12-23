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
  //char url[500] = "hhttp://baidu.com/Info";
  //char data[1024] = {0};
  //Status = HttpGet(data, url);
  //if (!NT_SUCCESS(Status))
  //{
  //  DebuggerPrint("[BuMo] Get Error %d  \r\n", Status);
  //  return STATUS_UNSUCCESSFUL;
  //}

  //DebuggerPrint("[BuMo] Data = %s  \r\n", data);


  char posturl[500] = "http://baidu.com/login";

  char postdata[1024] = { 0 };
  char requestData[1024] ="{\"email\":\"test\",\"password\":\"test\",\"projectId\":\"1\"}";
  Status = HttpPost(postdata, posturl, requestData,"application/json;charset=UTF-8");
  if (!NT_SUCCESS(Status))
  {
    DebuggerPrint("[BuMo] Post Error %d  \r\n", Status);
    return STATUS_UNSUCCESSFUL;
  }

  DebuggerPrint("[BuMo] Post Data = %s  \r\n", postdata);




  KsDestroy();

  return STATUS_UNSUCCESSFUL;
}
