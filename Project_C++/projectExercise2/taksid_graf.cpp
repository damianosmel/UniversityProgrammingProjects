#include <iostream>
#include <fstream>
using namespace std;
#include "ticket1.h"
#include "tplane.h"
#include "tboat.h"
#include "ttrain.h"
int main(){
       ticketTrain train;
       ticketBoat  boat;
       ticketPlane plane;
       int apant;
       int dest;
       int func;
       int krat;
       int ari8m_eis;
       int akur;
       int sum_krat;

       
       cout<<"Welcome to our travel agency!! "<<endl;
       do{
              cout<<"Please, select the means of transportation, train, boat or airplane,"<<endl;
              cout<<"for train, boat and airplane please hit 1,2,3 respectively"<<endl;
              cout<<"If you would like to end this service session please hit 0"<<endl;
              cin>>apant;      
                    if(apant!=0){
                                 if(apant==1){
                                         dest=train.printdest_tr();
                                         cout<<"Please, service 0 for seat reservation, 1 for cancellation of reservation and 2 to show the current status of your reservations"<<endl;
                                         cin>>func;
                                         switch(func)
                                          {
                                              case 0 :
                                                   do{
                                                       cout<<"To which category you would like your seat, (1)st, (2)nd or (3)rd? Please use the indicated numbers"<<endl;
                                                       cout<<"If you would like to finish the reservation please hit 0"<<endl;
                                                       cin>>krat;   
                                                       if(krat!=0)
                                                         {
                                                         
                                                            cout<<"How many tickets you would like to reserve for the " << krat << "category?"<<endl;
                                                            cin>>ari8m_eis;
                                                            train.getTicket_tr(krat-1,dest,ari8m_eis);}//close if for reservation
                                                  }while(krat);//as long as the user does not hit 0 the services loop
                                                  break;//end of switch 0
                                              
                                              case 1:
                                                  do{
                                                     cout<<"You would like to cancel a ticket at category (1)st, (2)nd or h (3)rd? Please use the indicated numbers"<<endl;
                                                     cout<<"If you would like to finish the cancellation please hit 0"<<endl;
                                                     cin>>akur; 
                                                     sum_krat=train.getTicket_tr(akur-1,dest,0);
                                                     
                                                     if(akur!=0){
                                                         //calling getTicket with 0 number of tickets will return the number of reserved tickets
                                                         sum_krat=train.getTicket_tr(akur-1,dest,0);             
                                                         cout<<"How many tickets you would like to cancel for the " << akur << " category?" << endl;
                                                         cin>>ari8m_eis;
                                                         if(sum_krat==0){   
                                                             //if user wants to cancel without any reservation
                                                             cout<<"Error, you have to make a reservation before canceling one.."<<endl;}
                                                                 
                                                         else{
                                                               train.deleteTicket_tr(akur-1,dest,ari8m_eis);}
                        							  }//close if for cancellation
                                                  }while(akur);
                                               break; //end of switch 1
                                               
                                              case 2:
												   //print the tickets service status for trains
                                                   train.printState_tr(dest);
                                                   break;//close if for printing service status 
                                                   }//end of switch       
                                                  
                                              }//close if for train tickets services
                                              
                             else if(apant==2) {           
                                              dest=boat.printdest_bt();
                                         cout<<"Please, service 0 for seat reservation, 1 for cancellation of reservation and 2 to show the current status of your reservations"<<endl;
                                         cin>>func;
                                         switch(func)
                                          {
                                              case 0 :
                                                   do{
                                                       cout<<"To which category you would like your seat, (1)st, (2)nd or (3)rd? Please use the indicated numbers"<<endl;
                                                       cout<<"If you would like to finish the reservation please hit 0"<<endl;
                                                       cin>>krat;   
                                                       if(krat!=0)
                                                         {
                                                            cout<<"How many tickets you would like to reserve for the " << krat << " category?" << endl;
                                                            cin>>ari8m_eis;
                                                            boat.getTicket_bt(krat-1,dest,ari8m_eis);}//close if for reservation
                                                  }while(krat);//as long as the user does not hit 0 the services loop
                                                  break;//close case 0 for reservation
                                              case 1:
                                                  do{
                                                     cout << "To which category you would like your seat, (1)st, (2)nd or (3)rd? Please use the indicated numbers" << endl;
                                                     cout << "If you would like to finish the cancellation please hit 0"<<endl;
                                                     cin>>akur; 
                                                     if(akur!=0){
                                                         //calling getTicket with 0 number of tickets will return the number of reserved tickets
                                                         sum_krat=boat.getTicket_bt(akur-1,dest,0);             
                                                         cout<<"How many tickets you would like to cancel for the " << akur << " category?"<<endl;
                                                         cin>>ari8m_eis;
                                                         if(sum_krat==0){   
                                                              //if user wants to cancel without any reservation
                                                             cout<<"Error, you have to make a reservation before canceling one"<<endl;}
                                                                 
                                                         else{
                                                               boat.deleteTicket_bt(akur-1,dest,ari8m_eis);}
                        							  }//close if for cancellation
                                                  }while(akur);
                                               break; //close case 1 for cancellation
                                              case 2:
												   //print the tickets service status for boats
                                                   boat.printState_bt(dest);
                                                   break;//close case 2
                                                   
                                             }//end of switch 
                                              }//close if for boat tickets services
                                                                                          
                               else if(apant==3){
                                                   dest=plane.printdest_pl();
                                         cout<<"Please, service 0 for seat reservation, 1 for cancellation of reservation and 2 to show the current status of your reservations"<<endl;
                                         cin>>func;
                                         switch(func)
                                          {
                                              case 0 :
                                                   do{
                                                       cout<<"To which category you would like your seat, (1)st, (2)nd or (3)rd? Please use the indicated numbers"<<endl;
                                                       cout<<"If you would like to finish the reservation please hit 0"<<endl;
                                                       cin>>krat;   
                                                       if(krat!=0)
                                                         {
                                                            cout<<"How many tickets you would like to reserve for the " << krat << "  category?"<<endl;
                                                            cin>>ari8m_eis;
                                                            plane.getTicket_pl(krat-1,dest,ari8m_eis);}//close if for reservation
                                                  }while(krat);//as long as the user does not hit 0 the services loop
                                                  break;//close case 0
                                              case 1:
                                                  do{
                                                     cout<<"To which category you would like your seat, (1)st, (2)nd or (3)rd? Please use the indicated numbers?"<<endl;
                                                     cout<<"If you would like to finish the cancellation please hit 0"<<endl;
                                                     cin>>akur; 
                                                     if(akur!=0){
														 //calling getTicket with 0 number of tickets will return the number of reserved tickets
                                                         sum_krat=plane.getTicket_pl(akur-1,dest,0);
                                                         cout<<"How many tickets you would like to cancel for the " << akur << " category?"<<endl;
                                                         cin>>ari8m_eis;
                                                         if(sum_krat==0){   
                                                              //if user wants to cancel without any reservation
                                                             cout<<"Error, you have to make a reservation before canceling one"<<endl;}
                                                                 
                                                         else{
                                                               plane.deleteTicket_pl(akur-1,dest,ari8m_eis);}
                        							  }//close if for cancellation
                                                  }while(akur);
                                                  break; //close case 1 for cancellation
                                              case 2:
												   //print the tickets service status for airplanes
                                                   plane.printState_pl(dest);
                                                     break;}//close case 2
                                                   
                                               }//close of switch
                                             }//close if for airplane tickets services
                                            
                                            cout << "All tickets services are saved to kratseis.dat" << endl;
                                            train.wfile_tr(); 
                                            boat.wfile_bt();
                                            plane.wfile_pl();  
                            } while(apant!=0);//close do while loop for all services.
                            
                            return 0;
                           }//close of main()            
                                                
