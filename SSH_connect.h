// SSH_connect.h: interface for the SSH_connect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSH_CONNECT_H__E032A4AD_6685_4337_A7FA_99F8B7F83C93__INCLUDED_)
#define AFX_SSH_CONNECT_H__E032A4AD_6685_4337_A7FA_99F8B7F83C93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct send_data 
{
	int c1,c2;
	char buffer[4096];
	int len;
	void*next;
};
class SSH_connect  
{
public:
	void send_hostory_data();
	void add_send_data(int c1,int c2,char*buffer,int len);
	SSH_connect();
	virtual ~SSH_connect();
	SSH_connect*out_connect;
	int in_sock,out_sock;
	int in_port,out_port;
	int in_c_port,out_c_port;
	char in_address[255],out_address[255];
	int isExit;
	void start_server_in(int port);
	void start_server_out(int port);
	void connect_client_in(char*address,int port);
	void connect_client_out(char*address,int port);
	void start_connect(SSH_connect*n);
	int recv_send(int c1,int c2);
	send_data*p_sd;
	int sd_count;
};

#endif // !defined(AFX_SSH_CONNECT_H__E032A4AD_6685_4337_A7FA_99F8B7F83C93__INCLUDED_)
