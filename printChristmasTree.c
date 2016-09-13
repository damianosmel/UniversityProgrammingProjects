#include <stdio.h>
main (){
	int i,j,n,hlf;

	printf("Please, insert a odd number higher than 3\n");
	scanf("%d",&n);

	for (j=1;j<=n;j++){ 
		for (i=1;i<=2*n;i++){ 
			if( i==(n-(j-1))||i==(n+(j-1)) )
				printf("o");
			else if ( i>(n-(j-1))&&i<(n+(j-1)) ) {
				printf("*");
			}	
			else printf(" ");
		}
		printf("\n");
	}
	hlf=(n-1)/2;

	for(i=1;i<=n-1;i++){
		for(j=1;j<n+4;j++){ 
			if(i>hlf){
				if(j==n-1)
					printf("<");
				else if(j==n+1)
					printf(">");
				else if(j==n)
					printf("|");
				else
					printf(" ");
			}
			else{
				if(j==n)
					printf("|");
				else
					printf(" ");
			}
		}     
		printf("\n");
	}
	return 0;
} 
