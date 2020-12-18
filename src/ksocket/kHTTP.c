#pragma once
#include "kHTTP.h"
#include "ksocket.h"
#include "berkeley.h"

int atoi(const char *nptr)
{
  int c;              /* current char */
  int total;         /* current total */
  int sign;           /* if '-', then negative, otherwise positive */

  /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr))
    ++nptr;

  c = (int)(unsigned char)*nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+')
    c = (int)(unsigned char)*nptr++;    /* skip sign */

  total = 0;

  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char)*nptr++;    /* get next char */
  }

  if (sign == '-')
    return -total;
  else
    return total;   /* return result, negated if necessary */
}

char *join(const char *a, const char *b) {
  char *c = (char *)ExAllocatePool(NonPagedPool, strlen(a) + strlen(b) + 1);
  if (c == NULL)
  {
    DbgPrintEx(77, 0, "分配空间失败");
    return NULL;
  }
  char *tempc = c;
  while (*a != '\0') {
    *c++ = *a++;
  }
  while ((*c++ = *b++) != '\0') {
    ;
  }
  //ExFreePool(a);
  return tempc;
}


NTSTATUS
NTAPI
KHttpInit(
  VOID
)
{
  return KsInitialize();
}


void ParseUrl(_Out_ char* Host, _Out_ char* ApiAddress,_Out_ char* Port, _In_ char* url)
{
  char hostTemp[50] = { 0 };
  char host[50] = { 0 };
  int port = 80;

  char* urldata = strstr(url, "//") + 2;

  char* apiAddress = strstr(urldata, "/");

  memcpy(hostTemp, urldata, apiAddress - urldata);

  //获取port
  char* Pport = strstr(hostTemp, ":");
  if (Pport) {
    Pport++;
    port = atoi(Pport);
    memcpy(host, hostTemp, Pport - hostTemp - 1);
  }
  else {
    memcpy(host, hostTemp, sizeof(hostTemp));
    Pport = (char *)ExAllocatePool(NonPagedPool, 10);
    memcpy(Pport, "80", 3);
  }

  memcpy(Port, Pport, strlen(Pport));
  memcpy(Host, host, strlen(host));
  memcpy(ApiAddress, apiAddress, strlen(apiAddress));
}

//char url[500] = "http://user.bsradar.cc/config/user/GetUserInfo";
NTSTATUS NTAPI HttpGet(_Out_ char* recv_buffer,_In_ char* url) {


  DbgBreakPoint();

  char host[50] = { 0 };
  char port[10] = { 0 };
  char apiAddress[1000] = { 0 };

  ParseUrl(&host, &apiAddress, &port,url);

  DebuggerPrint("[BuMo] host = %s \r\n", host);
  DebuggerPrint("[BuMo] port = %s \r\n", port);
  DebuggerPrint("[BuMo] apiAddress = %s \r\n", apiAddress);

  char * HttpBuffer = join("GET ", url);
  HttpBuffer = join(HttpBuffer, " HTTP/1.1\n");
  //Host: map.bsradar.cc
  HttpBuffer = join(HttpBuffer, "Host: ");
  HttpBuffer = join(HttpBuffer, host);
  HttpBuffer = join(HttpBuffer, "\n");


  HttpBuffer = join(HttpBuffer, "Connection: keep-alive\n");
  HttpBuffer = join(HttpBuffer, "Pragma: no-cache\n");
  HttpBuffer = join(HttpBuffer, "Cache-Control: no-cache\n");
  HttpBuffer = join(HttpBuffer, "Accept-Language: zh-CN,zh;q=0.9\n");
  HttpBuffer = join(HttpBuffer, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36\n");
  HttpBuffer = join(HttpBuffer, "\r\n");


  DebuggerPrint("[BuMo] HttpBuffer = %s \r\n", HttpBuffer);

  int result;
  char temp_recv_buffer[1024 * 3] = { 0 };
  struct addrinfo hints = { 0 };
  hints.ai_flags |= AI_CANONNAME;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo *res;
  result = getaddrinfo(host, port, &hints, &res);
  DebuggerPrint("[BuMo] getaddrinfo result = %d \r\n", result);
  int sockfd;
  sockfd = socket_connection(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  result = connect(sockfd, res->ai_addr, (int)res->ai_addrlen);
  DebuggerPrint("[BuMo] connect result = %d \r\n", result);
  result = send(sockfd, HttpBuffer, strlen(HttpBuffer), 0);
  DebuggerPrint("[BuMo] connect send = %d \r\n", result);
  result = recv(sockfd, temp_recv_buffer, sizeof(temp_recv_buffer), 0);
  DebuggerPrint("[BuMo] connect recv = %d \r\n", result);
  temp_recv_buffer[sizeof(temp_recv_buffer) - 1] = '\0';
  char* temp = strstr(temp_recv_buffer, "{\"message\":");
  memcpy(recv_buffer, temp, strlen(temp) + 1);
  closesocket(sockfd);
}
