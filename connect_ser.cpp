#include <stdio.h>
#include "SSH_connect_ctrl.h"
#include "MTCommon.h"

int main(int argc, char *argv[])
{
	MyInit();
	SSH_connect_ctrl ctrl;
	ctrl.sample5();
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