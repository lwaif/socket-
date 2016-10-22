// MTCommon.cpp: implementation of the MTCommon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTCommon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
HANDLE
MYCreateThread(
			   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			   DWORD dwStackSize,
			   LPTHREAD_START_ROUTINE lpStartAddress,
			   LPVOID lpParameter,
			   DWORD dwCreationFlags,
			   LPDWORD lpThreadId
			   )
{
	return Thread_Create(lpThreadId,lpStartAddress,lpParameter);
}
*/
void MyInit()
{
#if defined(WIN32)
	WSADATA     wsaData;   
	WSAStartup(0x0202, &wsaData);   
#endif
}
int GetTime()
{
#if defined(WIN32)
	return GetTickCount();
#elif defined(LINUX)
	struct timeval t_start;
	gettimeofday(&t_start, NULL);
	long temp = ( t_start.tv_usec /1000 + t_start.tv_sec*1000);
	return temp;
#endif
}

void nSleep(int s)
{
#if defined(WIN32)
	Sleep(s);
#elif defined(LINUX)
	usleep(s*1000);
#endif
}

void Thread_Destory(THREAD_HANDLE_TYPE *handle, uint32_t id)
{
	if(*handle)
	{
#if defined(WIN32)
		WaitForSingleObject(*handle, INFINITE);
		CloseHandle(*handle);
#elif defined(LINUX)
		pthread_join(id, NULL);
#endif // LINUX
		*handle = NULL;
	}
}
THREAD_HANDLE_TYPE Thread_Create(uint32_t *id, THREAD_RETURN_TYPE (WINAPI *run)(THREAD_PARAM_TYPE), THREAD_PARAM_TYPE param)
{
#if defined(WIN32)
	return CreateThread(0, 0, run, param, 0, id);
#elif defined(LINUX)
	return !pthread_create(id, 0, run, param);
#endif // LINUX
}
#if defined(WIN32)
#define BEGIN_CRITICIAL(a)	EnterCriticalSection(&a);
#define END_CRITICIAL(a)	LeaveCriticalSection(&a);
#elif defined(LINUX)
#define BEGIN_CRITICIAL(a)	pthread_mutex_lock(&a);
#define END_CRITICIAL(a)	pthread_mutex_unlock(&a);
#endif

#ifdef WIN32
void mutex_init(ACCESS_PROTECT_TYPE *lock)
{
	InitializeCriticalSection(lock);
}
#endif

#ifdef LINUX
void mutex_init(ACCESS_PROTECT_TYPE *lock, pthread_mutexattr_t *attr)
{
	pthread_mutexattr_init(attr);
	pthread_mutexattr_settype(attr, 1);
	pthread_mutex_init(lock, attr);
}
#endif

#ifdef WIN32
void mutex_lock(ACCESS_PROTECT_TYPE *lock)
{
	EnterCriticalSection(lock);
}
#endif

#ifdef LINUX
void mutex_lock(ACCESS_PROTECT_TYPE *lock)
{
	pthread_mutex_lock(lock);
}
#endif

#ifdef WIN32
void mutex_unlock(ACCESS_PROTECT_TYPE *lock)
{
	LeaveCriticalSection(lock);
}
#endif

#ifdef LINUX
void mutex_unlock(ACCESS_PROTECT_TYPE *lock)
{
	pthread_mutex_unlock(lock);
}
#endif

#ifdef WIN32
void mutex_uninit(ACCESS_PROTECT_TYPE *lock)
{
	DeleteCriticalSection(lock);
}
#endif

#ifdef LINUX
void mutex_uninit(ACCESS_PROTECT_TYPE *lock, pthread_mutexattr_t *attr)
{
	pthread_mutex_destroy(lock);
	pthread_mutexattr_destroy(attr);
}
#endif

#ifndef LINUX

    //字节流转换为十六进制字符串  
    void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)  
    {  
        short i;  
        unsigned char highByte, lowByte;  
      
        for (i = 0; i < sourceLen; i++)  
        {  
            highByte = source[i] >> 4;  
            lowByte = source[i] & 0x0f ;  
      
            highByte += 0x30;  
      
            if (highByte > 0x39)  
                    dest[i * 2] = highByte + 0x07;  
            else  
                    dest[i * 2] = highByte;  
      
            lowByte += 0x30;  
            if (lowByte > 0x39)  
                dest[i * 2 + 1] = lowByte + 0x07;  
            else  
                dest[i * 2 + 1] = lowByte;  
        }  
        return ;  
    }  
      
    //字节流转换为十六进制字符串的另一种实现方式  
    void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )  
    {  
        int  i;  
        char szTmp[3];  
      
        for( i = 0; i < nSrcLen; i++ )  
        {  
            sprintf( szTmp, "%02X", (unsigned char) sSrc[i] );  
            memcpy( &sDest[i * 2], szTmp, 2 );  
        }  
        return ;  
    }  
      
    //十六进制字符串转换为字节流  
    void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)  
    {  
        short i;  
        unsigned char highByte, lowByte;  
          
        for (i = 0; i < sourceLen; i += 2)  
        {  
            highByte = toupper(source[i]);  
            lowByte  = toupper(source[i + 1]);  
      
            if (highByte > 0x39)  
                highByte -= 0x37;  
            else  
                highByte -= 0x30;  
      
            if (lowByte > 0x39)  
                lowByte -= 0x37;  
            else  
                lowByte -= 0x30;  
      
            dest[i / 2] = (highByte << 4) | lowByte;  
        }  
        return ;  
    }  
char* gettime()
{
	static char result[200];
	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	sprintf(result,"%4d-%02d-%02d %02d:%02d:%02d ",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
	return result;
}
#ifdef _DEBUG
int islog=1;
#else
int islog=2;
#endif
char name[MAX_PATH]={0};   
void my_print(const   char   *form ,...)
{
    va_list   arg;   
    int   done;   
    char pbString[4096];
	if(name[0]==0){
		char name_txt[MAX_PATH];   
		char *p;
		GetModuleFileName(NULL,(LPSTR)name,MAX_PATH);  
		p=strrchr(name,'\\');
		*p=0;
		sprintf(name_txt,"%s\\HWSERVER.TXT", name);   
	}
	if (islog==1)
	{
		va_start(arg,form);  
		vsprintf(pbString,form,arg);				//若此行和下行改为：done = fprintf(console_printf_f,form,arg);则可变
//		done = fprintf(console_printf_f,pbString);	//参数的值不能正常显示，原因是参数传递错误，fprintf不能正确处理va_list类型的参数
		done = printf(pbString);
	    va_end(arg);
	}else if(islog==2){
		char*t;
		FILE *fp = NULL;  
		char name_txt[200];   
		sprintf(name_txt,"%s\\HWSERVER.TXT", name);   
		fp = fopen(name_txt, "a");  
	    if(fp == NULL)  
		{  
			return;  
		}  
		va_start(arg,form);  
		vsprintf(pbString,form,arg);				//若此行和下行改为：done = fprintf(console_printf_f,form,arg);则可变

		t=gettime();
		fprintf(fp,"%s",t);
		done = fprintf(fp,pbString);
	    va_end(arg);
		fclose(fp);  
		fp = NULL;  		
	}
}
#endif
void my_print(const   char   *form ,...);

int recv_send12(int c1,int c2) {
	int ret=4096;
	char buffer[4096]="\0";
//	while(TRUE)
	{
		ret = recv(c1,buffer,sizeof(buffer),0);
		if(ret>0)
			my_print("recv  :%d %d\n",c1,ret);   
		if (ret>0){
			ret = send(c2,buffer,ret,0);
			my_print("send  :%d %d\n",c2,ret);   
		}
		if (ret==0)return 1;
	}
	ret=0;
	return 0;
}

int recv_send(int c1,int c2) {
	int ret=-1;
	char buffer[4096]="\0";
//	while(TRUE)
	{
		if(c1>0)
			ret = recv(c1,buffer,sizeof(buffer),0);
		if(ret>0)
			my_print("recv  :%d %d %d \n",c1,c2,ret);   
		if (ret>0 && c2>0){
			ret = send(c2,buffer,ret,0);
			my_print("send  :%d %d %d \n",c1,c2,ret);   
		}
//		if (ret==-1)nSleep(10);
	}
	if (ret==0)
		return 1;
	else
		return 0;
}
/*
struct Mysenddata 
{
	int c1,c2;
	int buffer[4096];
	int len;
	void*next;
}*sd;
linklist s_d;
int recv_send11(int c1,int c2) {
	int ret=4096;
	char buffer[4096]="\0";
//	while(TRUE)
	{
		for (int i=1;i<=s_d.count();i++)
		{
			send_data *s = s_d.find(i);
			if (s->c1==c1 && s->c2==c2)
			{
				ret = send(c2,s->buffer,s->len,0);
				if (ret>0)
				{
					
				}
			}
		}
		ret = recv(c1,buffer,sizeof(buffer),0);
		int len=ret;
		if(ret>0)
			my_print("recv  :%d %d\n",c1,ret);   
		if (ret>0){
			ret = send(c2,buffer,ret,0);
			my_print("send  :%d %d\n",c2,ret);   
			if (ret<0)
			{
				send_data *s=new send_data;
				s->c1=c1;
				s->c2=c2;
				s->len=len;
				memcpy(s->buffer,buffer,4096);
				s_d.append(s);
			}
		}
		if (ret==0)return 1;
	}
	ret=0;
	return 0;
}
*/
int connect_recv_send1(int c1,int c2) {
	int ret=4096;
	char buffer[4096]="\0";
//	while(TRUE)
	{
		ret = recv(c1,buffer,sizeof(buffer),0);
		if(ret>0)
			my_print("recv  :%d %d %d\n",c1,c2,ret);   
		if (ret>0){
			while(TRUE)
			{
				int ret1 = send(c2,buffer,ret,0);
				my_print("send  :%d %d %d\n",c1,c2,ret1);   
				if (ret==0)return 1;
				if(ret1>0){
					my_print("send  :%d %d %d\n",c1,c2,ret1);   
					ret=ret1;
					break;
				}else{
					nSleep(100);
				}
			}
		}
		if (ret==0)return 1;
	}
	ret=0;
	return 0;
}