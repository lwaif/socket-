// SSH_connect.cpp: implementation of the SSH_connect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSH_connect.h"
#include "MTCommon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SSH_connect::SSH_connect()
{
	out_connect=0;
	isExit=0;
	p_sd=0;
	sd_count=0;
}

SSH_connect::~SSH_connect()
{
	if(p_sd){
		while (1)
		{
			send_data*p=(send_data*)p_sd->next;
			delete p_sd;
			if(p==0)break;
			p_sd=p;
		}
	}
}


THREAD_RETURN_TYPE WINAPI Thread_connect_client_in_Run(THREAD_PARAM_TYPE lpParameter)
{
	SSH_connect *n=(SSH_connect*)lpParameter;
    SOCKADDR_IN addrSrv;
 	int nTimeout = 100;//设置超时1秒
    addrSrv.sin_addr.S_un.S_addr=inet_addr(n->in_address);//本机地址，服务器在本机开启
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(n->in_c_port);// 设置端口号
	while (TRUE)
	{
		n->in_sock=socket(AF_INET,SOCK_STREAM,0);// AF_INET ..tcp连接
	    connect(n->in_sock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//连接服务器
 		setsockopt(n->in_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(nTimeout));	
		while (TRUE)
		{
			int ret=-1;
			char buffer[4096]="\0";
			ret = recv(n->in_sock,buffer,sizeof(buffer),0);
			if(n->out_sock==0 && strcmp(buffer,"hello")==0){//控制创建连接
				if(ret>0)my_print("recv  :%d %d\n",n->in_sock,ret);   
				n->start_connect(n);
			}else{
				ret = send(n->out_sock,buffer,ret,0);
				if(ret>0)my_print("send  :%d %d\n",n->out_sock,ret);   
			}
			if(n->out_sock && strcmp(buffer,"close")==0){//控制连接关闭
				closesocket(n->out_sock);
				n->out_sock=0;
			}
			if(n->isExit)break;
//			if (ret==-1)nSleep(100);
		}
		closesocket(n->in_sock);
		n->in_sock=0;
		if(n->isExit)break;
	}
	return 0;
}
void SSH_connect::connect_client_in(char*address,int port)
{
	isExit=0;
	strcpy(in_address,address);
	in_c_port=port;
	unsigned int       dwThreadId;   
	Thread_Create(&dwThreadId,Thread_connect_client_in_Run,this);
}



THREAD_RETURN_TYPE WINAPI Thread_connect_client_out_Run(THREAD_PARAM_TYPE lpParameter)
{
	SSH_connect *n=(SSH_connect*)lpParameter;
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=inet_addr(n->out_address);//本机地址，服务器在本机开启
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(n->out_c_port);// 设置端口号
    n->out_sock=socket(AF_INET,SOCK_STREAM,0);// AF_INET ..tcp连接
    connect(n->out_sock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//连接服务器
 	int nTimeout = 100;//设置超时1秒
 	setsockopt(n->out_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(nTimeout));	
	while (TRUE)
	{
		if(n->recv_send(n->out_sock,n->in_sock))break;
		if(n->isExit)break;
	}
	closesocket(n->out_sock);
	n->out_sock=0;
	return 0;
}

void SSH_connect::connect_client_out(char*address,int port)
{
	strcpy(out_address,address);
	out_c_port=port;
	out_sock=0;
//	unsigned int       dwThreadId;   
//	Thread_Create(&dwThreadId,Thread_connect_client_out_Run,this);
}

void SSH_connect::start_connect(SSH_connect*n)
{
	if (n->out_sock==0)
	{
		unsigned int       dwThreadId;   
		Thread_Create(&dwThreadId,Thread_connect_client_out_Run,n);
//		nSleep(1000);
	}
}
THREAD_RETURN_TYPE WINAPI Thread_start_server_in_In(THREAD_PARAM_TYPE lpParameter)
{
	char buffer[4096]="\0";
	SSH_connect*n=(SSH_connect*)lpParameter;

	if(n->out_sock==0){
		n->start_connect(n);
	}

 	int nTimeout = 100;//设置超时1秒
 	setsockopt(n->in_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(nTimeout));
	if(n->out_connect){
		send(n->out_sock,"hello",5,0);
		nSleep(500);
	}
	while (TRUE)   
	{               // Accept a connection 
		if(n->recv_send(n->in_sock,n->out_sock))break;
		if(n->isExit)break;
	}	
	if(n->out_connect)
		send(n->out_sock,"close",5,0);
	else if(n->out_sock){
		closesocket(n->out_sock);
		n->out_sock=0;
	}
	closesocket(n->in_sock);
	n->in_sock=0;

	return 0;
}
THREAD_RETURN_TYPE WINAPI Thread_start_server_in_Run(THREAD_PARAM_TYPE lpParameter)
{
	int       sListen;   
	struct sockaddr_in local, client_a;   
	SSH_connect*n=(SSH_connect*)lpParameter;
	sListen = socket(AF_INET, SOCK_STREAM, 0);   
	my_print("Listen :%d \n",n->in_port);     
	
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
	local.sin_family = AF_INET; 
	local.sin_port = htons(n->in_port);   
	int ret = bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));   
	if (ret==SOCKET_ERROR)return 1;
	ret = listen(sListen, 3);   
	if (ret==SOCKET_ERROR)return 1;
	while (TRUE)   
	{               // Accept a connection     
		int fromlen =sizeof(SOCKADDR_IN);
		n->in_sock=accept(sListen,(SOCKADDR*)&client_a,&fromlen);
		my_print("in_sock :%d \n",n->in_sock);     
		unsigned int       dwThreadId;   
		Thread_Create(&dwThreadId,Thread_start_server_in_In,n);
	}
	return 0;
}

void SSH_connect::start_server_in(int port)
{
	isExit=0;
	in_port=port;
	unsigned int       dwThreadId;   
	in_sock=0;
	Thread_Create(&dwThreadId,Thread_start_server_in_Run,this);
}


THREAD_RETURN_TYPE WINAPI Thread_start_server_out_In(THREAD_PARAM_TYPE lpParameter)
{
	char buffer[4096]="\0";
	SSH_connect*n=(SSH_connect*)lpParameter;
 	int nTimeout = 100;//设置超时1秒
 	setsockopt(n->out_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(nTimeout));	
	while (TRUE)   
	{               // Accept a connection 
		if(n->recv_send(n->out_sock,n->in_sock))int a=0;//break;
		if(n->isExit)break;
	}	
	closesocket(n->out_sock);
	n->out_sock=0;
	return 0;
}
THREAD_RETURN_TYPE WINAPI Thread_start_server_out_Run(THREAD_PARAM_TYPE lpParameter)
{
	int       sListen;   
	struct sockaddr_in local, client_a;   
	SSH_connect*n=(SSH_connect*)lpParameter;
	sListen = socket(AF_INET, SOCK_STREAM, 0);   
	my_print("Listen :%d \n",n->out_port);     
	
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
	local.sin_family = AF_INET; 
	local.sin_port = htons(n->out_port);   
	int ret = bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));   
	if (ret==SOCKET_ERROR)return 1;
	ret = listen(sListen, 3);   
	if (ret==SOCKET_ERROR)return 1;
	while (TRUE)   
	{               // Accept a connection     
		int fromlen =sizeof(SOCKADDR_IN);
		n->out_sock=accept(sListen,(SOCKADDR*)&client_a,&fromlen);
		unsigned int       dwThreadId;   
		Thread_Create(&dwThreadId,Thread_start_server_out_In,n);
	}
	return 0;
}

void SSH_connect::start_server_out(int port)
{
	out_port=port;
	unsigned int       dwThreadId;   
	out_sock=0;
	Thread_Create(&dwThreadId,Thread_start_server_out_Run,this);
}

int SSH_connect::recv_send(int c1,int c2) {
	int ret=-1;
	char buffer[4096]="\0";
//	while(TRUE)
	{
		if(c1>0)
			ret = recv(c1,buffer,sizeof(buffer),0);
		if(ret>0)
			my_print("recv  :%d %d %d \n",c1,c2,ret);   
		if (ret>0){
			int len=ret;
			ret=-1;
			if(c2>0){
				send_hostory_data();
				ret = send(c2,buffer,len,0);
				my_print("send  :%d %d %d \n",c1,c2,ret);  
			}
			if(ret==-1){
				add_send_data(c1,c2,buffer,len);
			}
		}
//		if (ret==-1)nSleep(10);
	}
	if (ret==0)
		return 1;
	else
		return 0;
}

void SSH_connect::add_send_data(int c1, int c2, char *buffer, int len)
{
	send_data*p=new send_data;
	p->c1=c1;
	p->c2=c2;
	memcpy(p->buffer,buffer,len);
	p->next=0;
	if(p_sd)
		p_sd->next=p;
	else
		p_sd=p;
	sd_count++;
}

void SSH_connect::send_hostory_data()
{
	while (1)
	{
		if(p_sd==0)break;
		send_data*p=(send_data*)p_sd->next;
		int ret = send(p_sd->c2,p_sd->buffer,p_sd->len,0);
		if(ret>0){
			delete p_sd;
			p_sd=p;
			sd_count--;
		}else
			break;
	}
}
