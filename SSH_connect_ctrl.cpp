// SSH_connect_ctrl.cpp: implementation of the SSH_connect_ctrl class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SSH_connect_ctrl.h"
#include "MTCommon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SSH_connect_ctrl::SSH_connect_ctrl()
{

}

SSH_connect_ctrl::~SSH_connect_ctrl()
{

}

void SSH_connect_ctrl::sample1()
{
	c1.start_server_in(22);
	c1.connect_client_out(“127.0.0.1”,22);
}

void SSH_connect_ctrl::sample2()
{
	c1.start_server_in(22);
	c2.start_server_in(23);
	c1.connect_client_out("127.0.0.1",23);
	c2.connect_client_out(“127.0.0.1”,22);
}

void SSH_connect_ctrl::sample3()
{
	c1.out_connect=&c2;
	c1.start_server_in(22);
	c1.start_server_out(23);
	c2.connect_client_in("127.0.0.1",23);
	c2.connect_client_out("127.0.0.1",33);
}

void SSH_connect_ctrl::msg()
{
	my_print("c1:%d %d\n",c1.in_sock,c1.out_sock);
	my_print("c2:%d %d\n",c2.in_sock,c2.out_sock);
}

void SSH_connect_ctrl::sample4()
{
	c1.out_connect=&c2;
	c1.start_server_in(22);
	c1.start_server_out(23);
}

void SSH_connect_ctrl::close()
{
	c1.isExit=1;
	c2.isExit=1;
	nSleep(2000);
}

void SSH_connect_ctrl::sample5()
{
	c1.out_connect=&c1;

	c1.start_server_in(9020);
	c1.start_server_out(9030);

}

void SSH_connect_ctrl::sample6()
{
	c2.connect_client_in(“127.0.0.1”,9030);
	c2.connect_client_out("127.0.0.1",3389);
}

void SSH_connect_ctrl::sample7()
{

	c1.out_connect=&c2;
	c1.start_server_in(3389);
	c1.start_server_out(9023);
	c2.connect_client_in("127.0.0.1",9023);
	c2.connect_client_out("127.0.0.1",3389);
}

void SSH_connect_ctrl::connect(char *server_ip, int server_port,char *ip, int port)
{
	c2.connect_client_in(server_ip,server_port);
	c2.connect_client_out(ip,port);
}
