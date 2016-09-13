#include<iostream>
#include "Vector.h"
#include<cstdlib>
#include<ctime>
using namespace std;

void Vector::sort(){
     
     quicksort(V,0,n-1);
     
     }
Vector::Vector(int size)
{         n=size;
          int i; 
          V = new int[n];
          srand( (unsigned)time( NULL ) );
          for(i=0;i<=n-1;i++)
          {
                   V[i]=rand();
          }
          
}

void Vector::print() 
{    
     for(int i=0;i<=n-1;i++)
     {
             cout<<i<<"\t"<<V[i]<<endl;
     }
     cout<<endl;
}

Vector::~Vector()
{
      delete []V;
}

//partition the vector
int Vector::partition(int* array,int left,int right,int pi)
{   int g;
    int temp,temp1,temp2,stind=left;
    g=array[pi];
    
    
    for(int i=left;i<=right;i++)
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
}

//sort the vector by quicksort
void Vector::quicksort(int*  array,int left,int right)
{	int pivot;

     if(right>left)
     {
            pivot=right;
            
            int newpint = partition(array,left,right,pivot);
            
            quicksort(array,left,newpint-1);
            
            quicksort(array,newpint+1,right);
     }
}

//test if vector is sorted
bool Vector::cheeksorted(){
        bool issorted=true;
        for(int i=0;i<=n-2;i++)
        {
                if(V[i]>V[i+1])
                {
                         issorted=false;
                         break;
                }
        }
        return issorted;
}
