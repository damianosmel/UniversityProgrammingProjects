#ifndef VECTOR_H
#define VECTOR_H
class Vector {
      public :
             Vector (int size);
             void print();
             int partition(int* array,int left,int right,int pi);
             void quicksort(int* array,int left,int right);
             bool cheeksorted();
             int *V;
             int n;
             void sort();
             ~Vector();
      };
#endif 
