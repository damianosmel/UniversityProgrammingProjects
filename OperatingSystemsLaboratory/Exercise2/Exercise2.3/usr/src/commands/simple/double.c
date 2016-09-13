#include <stdio.h>
#include <lib.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
  if(argc!=2)
	{
   		/*inform the user that he chose false argument calling double*/
		printf("Error:giving arguments.\n");
		printf("Please give one number.\n");
		/*return error*/
		return -1;

	}
    else
	{
		/*change the char type argument into int type*/
		int arg=atoi(argv[1]);
		message m;
		int doub;
		m.m1_i1=arg;
		doub= _syscall(MM,70,&m);
		printf("\n The result is : %d\n",doub);
		/*return success*/	
		return 0;
	
	}	

}
