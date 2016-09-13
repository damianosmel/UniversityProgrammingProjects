#include <stdio.h>
#include <stdlib.h>
//#define M  1000
#include <math.h>
#include<time.h>
long int b_search(float *table,float x,long int l,long int r);
long int demi_duad(float *table,float x,long int l,long int r);
long int intp_search_bin(float *table,float y,long int left,long int right);
long int intp_search_exp(float *table,float w,long  int left,long int right);
long int linear_search(float *tab,float k,long int left,long int right,int int_s);
//float diab_k();
double gaussrand(void);
int partition(float* array,int left,int right,int pi);
int quicksort(float*  array,int left,int right);
//short int time_b,time_intp_bin,int_exp,xr,xr1,xr2;
int comparisons_b,comparisons_int, comparisons_int1,antipr;
long int size,M;
static long int i,j;
int popl=1;

int main(void){
    
  int apant,found,p,count,M,found1,found2;
  double xron_bin,xron_intp_b,xron_intp_exp;
   double time_b,time_intp_b,time_intp_exp,xr,xr1,xr2;
   float k;
   float *t;
   long int g;
   
   sugk_int1=0;
   count=1;
   
   printf("\t Please, insert the length of the array to be searched\n");
   scanf("%ld",&M);
   
   //allocating the array into memory
   if( (t = (float*)malloc(M * sizeof(float))) == NULL )
   {
		printf("\n\n Error: Memory couldn't be allocated ");
   }

   //time_t time_b,time_intp_b,time_int_exp,xr,xr1,xr2;//xronoi susthmatos
   printf("Please choose distribution for array data 0=uniform, 1=normal, 2=random \n");
   scanf("%d",&apant);
  
   switch(apant)
   {
     case 0 :
     printf(" Creating data from an uniform distribution \n");
     
     //initialization of uniform distribution
     srand( (unsigned)time( NULL ) );
     float pr,st_t;
     //data have the same distance
     st_t=(rand()/1000);
     srand( (unsigned)time( NULL ) );
     t[0]=rand()/1000;
     
     for ( g=1;g<=M-1;g++)
      {
      t[g]=t[g-1]+st_t;
      }
     break;
     
     case 1 :
     printf(" Creating data from an normal distribution \n");
      
     //initialization of normal distribution
     for(g=0;g<=M-1;g++){
           t[g]=(float)gaussrand();
     }//kleinei for
     break;
     
     case 2 :
		printf(" Creating data from an random distribution \n");
	
		//initialization for random distribution
        srand( (unsigned)time( NULL ) );
		//get always the same seed for the generation of random distribution
		t[0]=rand()/1000;
        for(g=1;g<=M-1;g++)
        {
			t[g]=t[g-1]+rand()/1000+1;
        }
        break;
	}//end of switch
	
    //after populating the array, sort array as first step of searching
    quicksort(t,0,M-1); 
    printf("Array data are (in ascending order):\n");
    for(p=0;p<=M-1;p++){
		printf("%f\t",t[p]);
        count++;
        if(!(count%11)){
			count=1;
			printf("\n");
        }
    }//end of print loop
    
    printf("\n Please, insert x to search selected distribution\n");
    scanf("%f",&k);
    printf("\n You have inserted %f\n",k);
    
    // start timer and use binary search..
    time_b=(double)clock();
    found=b_search(t,k,0,M-1);
    
    //after a first use of the search algorithm, benchmark it
    for(i=1;i<=1000000;i++){
		b_search(t,k,0,M-1);
    }
    //get total time for running the algorithm in a vast amount of times
    xr=(double)clock();
    xron_bin=(double)( ((xr-time_b)*1000)/CLOCKS_PER_SEC);
    
    // start timer and use binary interpolation search
    time_intp_b=(double)clock();
    found1=intp_search_bin(t,k,0,M-1);
    
    //after a first use of the search algorithm, benchmark it
    for(i=1;i<=1000000;i++){
		intp_search_bin(t,k,0,M-1);
    }
    //get total time for running the search algorithm, benchmark it
    xr1=(double)clock();
    xron_intp_b=(double)( ((xr1-time_intp_b)*1000)/CLOCKS_PER_SEC);
    
    //start timer and use exponential interpolation search
    time_intp_exp=(double)clock();
    found2=intp_search_bin(t,k,0,M-1);
    
    //after a first use of the search algorithm, benchmark it
    for(j=1;j<=1000000;j++){
       sugk_int1=0;
	   intp_search_exp(t,k,0,M-1);
    }
    //get total time for running the algorithm in a vast amount of times
    xr2=(double)clock();
    xron_intp_exp=(double)( ((xr2-time_intp_exp)*1000)/CLOCKS_PER_SEC);
    
    //if all search algorithms have found the inserted element
	if( (found>0)&&(found1>0)&&(found2>0) )
	{
		printf("Inserted search element x=%f was found after %f nanosecs and %d comparisons (binary search)\n",k,xron_bin,comparisons_b);
		printf("Inserted search element x=%f was found after %f nanosecs and %d comparisons (binary interpolation search)\n",k,xron_intp_b,comparisons_int);
		printf("Inserted search element x=%f was found after %f nanosecs and %d comparisons (exp interpolation search)\n",k,xron_intp_exp,comparisons_int1);
	}
	else 
	{
		printf("Inserted search element x=%f could not be found after %f nanosecs and %d comparisons (binary search) \n",k,time_b,comparisons_b);
		printf("Inserted search element x=%f could not be found after %f nanosecs and %d comparisons (interpolation search) \n",k,time_intp_b,comparisons_int);
		printf("Inserted search element x=%f could not be found after %f nanosecs and %d comparisons ( exp interpolation search) \n",k,xron_intp_exp,comparisons_int1);
	}
	system("PAUSE");
	free(t);
	return 0;
}//end of main

//implement binary search algorithm
long int b_search(float *tab,float x,long  int l,long int r){
   comparisons_b=1;
   long  int left=l,right=r;
   long int next=(right+left)/2;
   
   while(x!=tab[next]&&left<right){
        comparisons_b++;                         
		if(x<=tab[next]){
			comparisons_b++;           
			right=next-1;
		}
		else if(x>tab[next]){ 
			comparisons_b++;
			left=next+1;
		}
		
		next=(right+left)/2; //update the index of next searching point
   }//close search loop
   
   comparisons_b++;
   
   /*check if founded value is close to inserted search value
   to assess the success of search*/
   if ((x-tab[next])<0.0000009){
      comparisons_b++;
      return next;
   }
   else{
        comparisons_b++;
        return -1;
   }
    
        
  }//end of binary search

//implement binary interpolation search algorithm
long int intp_search_bin(float *tab,float y,long int left,long int right){
    comparisons_int=0;
    int j,i;
    long int  next,bhm,test_step;
 
    size =right-left+1;
    next= left + (int)((int)(y - tab[left]) * (right - left)) / (tab[right]-tab[left]);
    next = (next<=0)?1:next; 
 
    comparisons_int+=2;
    //check if value if out of array bounds
    if(y<tab[left]|| y>tab[right]) {    
         return -1;
    } 
    //if value is inside the bounds, start the search loop
    while(y!=tab[next]) {
		comparisons_int++;
		i=0;
		size=right-left+1;
		
		if(size<=5&&size>0) //if search length is small enough do linear search
		{
			return linear_search(tab,y,left,right,1);
		}
        
        //find next search bounds
        bhm= (int)sqrt( (double)size);
        if(y>=tab[next]){
          comparisons_int++;
          test_step = next+i*bhm ;
          
          if (test_step > right)
          {       
               test_step=right;
               left =  next+(i-1)*bhm;
          }
          
          while(y>=tab[test_step] && (test_step <= right))
          {
             if(y==tab[test_step])
             {
                 return test_step;
             }
             else
             {
                 i++;    
                 test_step = next + i*bhm ;   
             }
             sugk_int+=2;
            }//close while
            
            if (test_step > right)
            {   
                 left =  next+(i-1)*bhm;
            }  
            else{                    
                 right=test_step;                     
                 left=next+i*bhm-bhm;
                 comparisons_int++;
           }                    
        }//close if   y>=tab[next]   
     
        else if (y < tab[next]) {
         
          comparisons_int++;
          //find next search bounds
          test_step = next-i*bhm ;
          if (test_step <left)
          {     
               test_step=left;   
               right =  next-(i+1)*bhm;
          }         
          
          while(y<tab[test_step]&&(test_step>=left)){
             if(y==tab[test_step])
             {
                 return test_step;
             }             
             else
             {
                 i++;    
                 test_step = next-i*bhm ;
             }
             comparisons_int+=2;    
           }//end of while 
          
           if (test_step < left)
           {        
               right =  next-(i-1)*bhm;
           }       
           else{             
               right=next-(i-1)*bhm;
               left=test_step; 
               comparisons_int++;
           }
       }//close else if y< tab[next]
       //for every run of the search loop give a new current found value
       next=left + (int)((int)(y - tab[left]) * (right - left)) / (tab[right]-tab[left]); 
       next = (next<=0)?1:next; 
   }//close search while-loop
   comparisons_int++;
   return next;
 }//end of binary interpolation
 
//implement exponential interpolation search  
long int intp_search_exp(float *tab,float w,long int left,long int right)
{
    int j,f=0;
    long int next,bhm,arist_akr,deksi_akr,test_step;
    size =right-left+1;
    
    next= left + (int)((int)(w - tab[left]) * (right - left)) / (tab[right]-tab[left]);
    next = (next<=0)?1:next; 

    //check if search value is out of bounds
    if(w<tab[left]||w>tab[right]) 
    {
         sugk_int1+=2;    
         return -1; 
    }
    
    //if value is inside bounds, start the search loop
    while(w!=tab[next]) 
    {     
      comparisons_int1++;
      i=1;
      size=right-left+1;
      
      if(size<=5&&size>0){ //if search length is small enough run linear search
		return linear_search(tab,w,left,right,0);   
      }//close of linear search case
      
      bhm = (int)sqrt((double)size);
    
      if(w>=tab[next])
      {
          comparisons_int1++;
          test_step = next+i*bhm ;
         
          if (test_step > right)
          {
               left =  next+(i/2)*bhm;
          }         
          
          while(w>=tab[test_step] && (test_step <= right))
          {
             if(w==tab[test_step])
             {                      
                 return test_step;
             }
             else
             {
                 i*=2;    
                 test_step = next + i*bhm ;    
             }
             comparisons_int1+=2;
          }//close while to move test step as right as you currently can
         
          if (test_step > right)
          {   
			 left =  next+(i/2)*bhm;
          }   
          else{                   
              right=test_step;                     
              left=next+(i/2)*bhm; 
              comparisons_int1++;
          }
          
          if(i==2){ //if you have 2 search representatives, you do not need binary search for them
		      return    intp_search_exp(tab,w,left,right);
          }
          else{ //otherwise run binary search on the search representatives
              antipr=i/2;    
              deksi_akr=demi_duad(tab,w,left,right);
              arist_akr=deksi_akr+bhm;
              return   intp_search_exp(tab,w,left,right);
          }//kleinei if-else for representatives number       
     }//close if for search value greater than current found value
    
    else if (w < tab[next]) {
          comparisons_int1++;
          test_step = next-i*bhm ;
          
          if (test_step <left)
          {
               test_step=left;   
               right =  next-i*bhm;    
          }         
          
          while(w<=tab[test_step]&&(test_step>=left)){
                if(w==tab[test_step])
             {                      
                 return test_step;
             }else
             {
                 i*=2;    
                 test_step = next-i*bhm ;    
             }
             comparisons_int1+=2;
           }//close while to move test step as left as you currently can
             
              if (test_step < left)
              {
                  right =  next-(i/2)*bhm;
              }     
              else{               
                 right=next-(i/2)*bhm;
                 left=test_step;
                 comparisons_int1++;          
              }
              
              if(i==2){//if you have 2 search representatives, you do not need binary search for them
				 return   intp_search_exp(tab,w,left,right);
              } 
              else{//otherwise run binary search on the search representatives         
                 deksi_akr=demi_duad(tab,w,left,right);
                 arist_akr=deksi_akr+bhm;
                 return   intp_search_exp(tab,w,left,right);
              } 
       }//close else-if y< tab[next]
     //for every run of the search loop give a new current found value  
     next=left + (int)((int)(w - tab[left]) * (right - left)) / (tab[right]-tab[left]); 
    
  }//close search while loop
  
  comparisons_int1++;
  return next;
 }//end of exponential interpolation 
  
//partition table using a pivot (as part of quicksort)
int   partition(float* array,int left,int right,int pi)
{   int i,stind=left;
    float temp,temp1,temp2;
    float g=array[pi];
    
    for(i=left;i<=right;i++)
    {
            if(array[i]<g)
            {
                   temp1=array[i];
                   array[i]=array[stind];
                   array[stind]=temp1;
                   stind++;
            }
            
            }
            temp2=array[stind];
            array[stind]=array[right];
            array[right]=temp2;
    
    
    return stind;
    
}//end of partition

//implement quicksort sorting algorithm
int quicksort(float*  array,int left,int right)
{int pivot;
    
     if(right>left)
     {
            pivot=right;
            
            int newpint = partition(array,left,right,pivot);
            
            quicksort(array,left,newpint-1);
            
            quicksort(array,newpint+1,right);
     }
}//end of quicksort

//create data from normal (Gaussian) distribution
double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else{
		X = V2 * sqrt(-2 * log(S) / S);
	}
	
	phase = 1 - phase;

	return X;
}//end of Gaussian function

//implement binary-like search algorithm for search representatives
long int demi_duad(float *tab,float x,long int l,long int r){
	
	long  int left=l,right=r;
	int count=1;
	long  int next=(right+left)/2;
	int plat=(int)sqrt( (double)size);
    
    while(count<=antipr){
		
		if(x<=tab[next]){
			if( x>=tab[next-plat] ){//return new search length as the length between the first and last representatives minus current search space
				return next-plat;
            }
			else{ 
				right=next;
			}
        }         
		else if(x>tab[next]){
			if(x<tab[next+plat]){//return new search length as the length between the first and last representatives          
				return next;
			}             
			else {
				left=next;
            }
        
		next=(right+left)/2;
	}//close while
    
}//end of binary-like search

//implement linear search algorithm
long int linear_search(float *tab,float k,long int left,long int right,int int_s){
    long int c;

    for(c=left;c<=right;c++){//serially search for value in the search space 
		if((k-tab[c])<0.0000009){ //if current value and search value are close enough
			//update comparisons counts respectively to the algorithm that called linear search
			if(int_s==1){
				comparisons_int+=c-left;
            }
            else{
				comparisons_int1+=c-left;
            }            
            return c;
        }  
    } //close search for loop                
    return -1;              
}//end of linear search
