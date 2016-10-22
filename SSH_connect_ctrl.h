// SSH_connect_ctrl.h: interface for the SSH_connect_ctrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSH_CONNECT_CTRL_H__2EDF7866_E1CF_445A_B662_79CAC07934BD__INCLUDED_)
#define AFX_SSH_CONNECT_CTRL_H__2EDF7866_E1CF_445A_B662_79CAC07934BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SSH_connect.h"

class SSH_connect_ctrl  
{
public:
	void connect(char *server_ip, int server_port,char *ip, int port);
	void sample7();
	void sample6();
	void sample5();
	void close();
	void sample4();
	void msg();
	void sample3();
	void sample2();
	void sample1();
	SSH_connect_ctrl();
	virtual ~SSH_connect_ctrl();
	SSH_connect c1;
	SSH_connect c2;
};

#endif // !defined(AFX_SSH_CONNECT_CTRL_H__2EDF7866_E1CF_445A_B662_79CAC07934BD__INCLUDED_)
