#include <stdio.h>
#include "SSH_connect_ctrl.h"
#include "MTCommon.h"

int main(int argc, char *argv[])
{
	if (argc==1)
	{
		printf("connect server_ip server_port client_ip client_port\n");
		return 1;
	}
	MyInit();
	SSH_connect_ctrl ctrl;
	ctrl.connect(argv[1],atoi(argv[2]),argv[3],atoi(argv[4]));
	while (1)
	{
		char GetData[20];
		gets(GetData);
		if(strcmp(GetData,"msg")==0)
		{
			ctrl.msg();
		}
	}
	return 0;
}