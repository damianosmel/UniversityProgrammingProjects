#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
  if(argc!=2)
	{
		/*inform the user that he chose false argument calling square*/
		printf("Error:giving arguments. \n");
		printf("Please give one number. \n");
		/*return error */
		return -1;
	}
   else
	{	/*change the char type argument into int type */
		int arg=atoi(argv[1]);
		printf("\n %d \n",square(arg) );
		/*return success*/
		return 0;	
  	}	

}