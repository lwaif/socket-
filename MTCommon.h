// MTCommon.h: interface for the MTCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MTCOMMON_H__0A09D59A_3EE3_45F9_9FC3_C5F9CD401C4B__INCLUDED_)
#define AFX_MTCOMMON_H__0A09D59A_3EE3_45F9_9FC3_C5F9CD401C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C" {
#endif

void MyInit();
int GetTime();
#define PORT       9000 
#define PORTPHP	   9090
void nSleep(int s);

#ifndef WIN32
#define LINUX
#endif // WIN32

#ifdef LINUX
#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned int	bool_t;
#define WINAPI
#define CALLBACK
#define ACCESS_PROTECT_TYPE		pthread_mutex_t
#define EVENT_TYPE				sem_t
#define THREAD_PARAM_TYPE		void *
#define THREAD_HANDLE_TYPE		int
#define THREAD_RETURN_TYPE		void *
#define TRUE					1
#define FALSE					0
//#include <android/log.h>
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "yyy", __VA_ARGS__)
#endif // LINUX

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib") 
#include <winsock.h> 
typedef /*signed*/ char		int8_t;
typedef /*signed*/ short	int16_t;
typedef /*signed*/ int		int32_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int    uint32_t;
typedef unsigned int	bool_t;
#define ACCESS_PROTECT_TYPE	    CRITICAL_SECTION
#define EVENT_TYPE				HANDLE
#define THREAD_PARAM_TYPE		LPVOID
#define THREAD_HANDLE_TYPE		HANDLE
#define THREAD_RETURN_TYPE		uint32_t
#define LOGI printf
#endif // WIN32

THREAD_HANDLE_TYPE Thread_Create(uint32_t *id, THREAD_RETURN_TYPE (WINAPI *run)(THREAD_PARAM_TYPE), THREAD_PARAM_TYPE param);
void Thread_Destory(THREAD_HANDLE_TYPE *handle, uint32_t id);
void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )  ;
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);
void my_print(const   char   *form ,...);
int recv_send(int c1,int c2);
int connect_recv_send(int c1,int c2);


extern int islog;
extern char name[MAX_PATH];

#ifdef __cplusplus
}
#endif


#endif // !defined(AFX_MTCOMMON_H__0A09D59A_3EE3_45F9_9FC3_C5F9CD401C4B__INCLUDED_)
