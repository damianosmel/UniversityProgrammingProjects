#include<iostream>
#include "Vector.h"

using namespace std;
int n;
int main()
{    
    do{
        cout<<"Give size of array which you want to sort"<<endl;
        cin>>n;
      }    
      while(n==0);
      
      Vector *v1=new Vector(n);
      
      cout<<"Members of array have initialized by random"<<endl;
      
      v1->print();
      
      if(v1->cheeksorted())
      {
            cout<<"Array is sorted without quicksort"<<endl;
           v1->print();
      }
      else
      {
          v1->sort();
          cout<<"Array just sorted with quicksort"<<endl;
          v1->print();
          
      }
      system("PAUSE");
      
}
