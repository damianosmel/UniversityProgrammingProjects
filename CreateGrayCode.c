#include <stdio.h>
#include <math.h>
#define K 3      
int G[(int)pow(2,K)][K];
void mgc(int b);
int main(void) {
    int i,j,a=1;
    
    for(i=0;i<=pow(2,K)-1;i++){
		for(j=0;j<=K-1;j++)
			G[i][j]=0;
    }
    G[1][K-1]=1;
    mgc(a);
    return 0;
}

 void mgc(int b){
    int i,j,s,g;
      
	if (b==K){
		for (i=0;i<=(int)pow(2,K)-1;i++){
			printf("The number %d in Gray code is ",i);
            for(j=0;j<=K-1;j++)
				printf("%d",G[i][j]);
            
            printf("\n");
            scanf("%d",&s);
        }  
             
    }   
    else{ 
		for(i=b;i>0;i--){
			g=1;
            for(j=((int)pow(2,b)-1);j>=0;j--){
				G[(j+g)][K-i]= G[j][K-i];
                g+=2;
            }
        }    
        for(i=pow(2,b);i<=(int)pow(2,b+1);i++)
			G[i][K-(b+1)]=1;
        
        mgc(b+1);
    }  
}
