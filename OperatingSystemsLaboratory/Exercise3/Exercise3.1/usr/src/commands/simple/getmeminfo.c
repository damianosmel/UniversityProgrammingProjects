#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <ctype.h>

char str[10];
int main(void)
{
typedef struct mem_info
{
	int num_holes;
	int max_hole;
	int min_hole;
	float avg_size;
	float std_dev;
}hole_info;
hole_info *info;
/*allocate memory space for the struct*/
info=(hole_info*)malloc( sizeof(hole_info) );

if( info !=NULL ) 
{ /*you are here if the memory allocation was done.*/
	message m;
	/*pass the struct through the pointer to servers space.*/
	m.m1_p1=(char*)info;	
	do{
		/*wait a sec to give information about holes.*/
		sleep(1);
		_syscall(MM,69,&m);
		printf("\nThe number of holes is %d",info->num_holes);	
		printf("\nmax size of a hole is %d\t",info->max_hole);
		printf("min size of a hole is %d .",info->min_hole);
		printf("\nThe average size of a hole is %.2f\t",info->avg_size);
		printf("standard deviation of size is %.2f",info->std_dev);	
		printf("\nPlease,press Ctrl-D to terminate\n");
		/*the gets() (function) return NULL when you press Ctrl-D.*/
	} while( gets(str) != NULL );
	/*free allocated memory.*/
	free( info );

}
else
{  /*something in the memory allocation went wrong,so inform the user.*/
	printf("\n Memory allocation can't be done.\n ");
}
return 0;
}

