#include <lib.h>
#include <unistd.h>

PUBLIC int square(int x)
{ 
 message m;
 /* Pass the argument into the message */
 m.m1_i1 = x;
 return(_syscall(MM,SQUARE,&m));

}