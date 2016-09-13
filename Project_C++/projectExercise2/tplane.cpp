#include <iostream>
using namespace std;
#include <iomanip>
#include <fstream>
#include <cstdlib>
             using std::ios;
              using std::cerr;
              using std::endl;
              using std::ofstream;
#include  "tplane.h"

         int rout_pl[4][3];

	  //initialize availability for plane tickets
      ticketPlane::ticketPlane():Ticket(4,16,-1,4) 
      {  for(int i=0;i<=3;i++){
                  rout_pl[i][0]=4;
                  rout_pl[i][1]=16;
                  rout_pl[i][2]=-1; }
           
             //the third category has negative number of seats as I cannot have an available seat
      }
      
      ticketPlane::~ticketPlane(){
                 // delete [] dromol_pl;
              // delete [] rout_pl;  
        }
     
     //reserve a ticket
     int ticketPlane::getTicket_pl(int kathg,int dest,int num_krat)
      { 
               int g = rout_pl[dest][kathg];
               
               if((g>=num_krat)&&(g!= -1))  //check if there are sufficient available seats and selected category can be selected at current means of transport
               {
                  rout_pl[dest][kathg]-=num_krat;
                  
                  //get number of the available seats
                  if(kathg==0){
                         return 4-rout_pl[dest][kathg];}
                  else if(kathg==1){
                         return 16-rout_pl[dest][kathg];}       
               }
               else if(kathg!=0) //if we are short on tickets availability we suggest new categories
               {   cout<<"Sorry, we cannot provide more seats at selected category"<<endl;
                    cout<<"Info: There are available seats at the following categories:"<<endl;  
                    for(int i=0;i<=2;i++){
                            if((i!=kathg)&&(rout_pl[dest][i]!=0)){
                              cout<< "At " << i+1 << " category, there are " << rout_pl[dest][i]<< " available seats"; }   
                              cout<<"\n"; 
                    }//close for
                    return -2;
               }
               if(kathg==2) {
                    cout<<"We are sorry to inform that our agency cannot reserve seat at selected category if the currently selected means of transport"<< endl;
                   return -2; }     
       }
       
       ////print transportation schedule for planes
       int ticketPlane::printdest_pl()const{
            string dromol_pl[4]={"ATHENS-THES/NIKH","ATHENS-MILANO","ATHENS-FRANKFURT","ATHENS- NEW YORK" };
            int proorismos;
					
                    cout<<"Info: Our agency for trains can provide for 1st category with 4 seats as the maximum number and to 2nd category with 16 seats as maximum number"<<endl;
                    cout<<"For planes the transportation schedule is: "<<endl;
                   for(int i=0;i<=3;i++)
                       cout<<"\n"<<(dromol_pl[i]);
                       cout<<"For which of the planes you would like to reserve a seat, please hit its ascending number"<<endl;
                       cin>>proorismos;
                       return proorismos-1;
       }
               
      //cancel ticket reservation
      void  ticketPlane::deleteTicket_pl(int kathgr,int des,int num_del){
                               int stoixeia_akur=rout_pl[des][kathgr];
                           if(stoixeia_akur!=-1){
                                       switch (kathgr){ 
                                              case 0:    
                                          if( num_del<=4-stoixeia_akur)
                                             rout_pl[des][kathgr]+=num_del;
                                          else
                                              cout<<"You cannot cancel " << num_del << "tickets, as you had reserved only " << 4-stoixeia_akur<<endl;  
                                             break;
                                             case 2:
                                               if(num_del<=16-stoixeia_akur)
                                                  rout_pl[des][kathgr]+=num_del;
                                               else
                                                 cout<<"You cannot cancel " << num_del << " tickets, as you had reserved only " << 16-stoixeia_akur<<endl;  
                                             break;             
                                             }
                            }//close if
                  }
      
      //print planes tickets services
      void  ticketPlane::printState_pl(int proor){
            string dromol_pl[4]={"ATHENS-THES/NIKH","ATHENS-MILANO","ATHENS-FRANKFURT","ATHENS-NEW YORK" };
                                 #include <iostream>
                                 using std::cout;
                                 using std::endl;
                                 using std::setw;
                                      int res_a=rout_pl[proor][0] ;
                                      int res_b=rout_pl[proor][1] ;
                                      int res_seats=res_b +res_a;
                                      for (int i=0; i<24; i++)
                                          cout << "=";
                                      cout<<"AIRPLANE";
                                 for (int i=0; i<26; i++)
                                        cout << "=";
                                         cout << endl; 
                                      cout<<"ROUTE:"<<setw(25)<<(dromol_pl[proor])<<endl;
                                      cout<<"TOTAL SEATS:"<<setw(9)<<20<<endl;
                                      cout<<"RESERVED SEATS:"<<setw(6)<<20-res_seats<<endl;
                                      cout<<"AVAILABLE SEATS:"<<setw(4)<<res_seats<<endl;
                                       for (int i=0; i<21; i++)
                                          cout <<"-";
                                      cout<<" RESERVED ";
                                      for (int i=0; i<21; i++)
                                          cout <<"-";
                                          cout<<endl;
                                      cout<<setw(26)<<"A CLASS"<<setw(12)<<"B CLASS"<<endl;
                                      cout<<setw(24)<<4-res_a<<setw(13)<<16-res_b<<endl;
                                     for (int i=0; i<21; i++)
                                          cout <<"-";
                                      cout<<" AVAILABLE ";
                                      for (int i=0; i<21; i++)
                                          cout<<"-";
                                          cout<<endl;
                                      cout<<setw(26)<<"A CLASS"<<setw(12)<<"B CLASS"<<endl;
                                      cout<<setw(24)<<res_a<<setw(13)<<res_b<<endl;
                                      for(int i=0;i<54;i++)
	                                        cout << "=";
	                                        cout<<endl;
	  }
	  
	  //write plane tickets services to the general service file
      void ticketPlane::wfile_pl(){
        
          string dromol_pl[4]={"ATHENS-THES/NIKH","ATHENS-MILANO","ATHENS-FRANKFURT","ATHENS-NEW YORK" };
                  ofstream outdromolFile("kratiseis.dat",ios::app); //write the new tickets service keeping the already saved data
		   if(!outdromolFile){
                   cerr<<" Error, accessing service file"<<endl;
                   exit(1);}
                   outdromolFile<<"----------------------------------"<<endl;
                   outdromolFile<<"AIRPLANE"<<endl;
                   outdromolFile<<"ROUTES:"<<endl;
                   for(int i=0;i<=3;i++){
                       outdromolFile<<dromol_pl[i]<<endl;  
                        outdromolFile<<"RESERVED SEATS:"<<endl;
                        outdromolFile<<"A class       B class"<<endl;    
                        outdromolFile<<4-rout_pl[i][0]<<"        "<<16-rout_pl[i][1]<<endl;  
                         outdromolFile<<"AVAILABLE SEATS:"<<endl;
                        outdromolFile<<rout_pl[i][0]<<"          "<<rout_pl[i][1]<<endl; 
                        outdromolFile<<"TOTAL  SEATS :"<<endl;
                        outdromolFile<<20<<endl;
                        outdromolFile<<"RESERVED SEATS :"<<endl;
                        outdromolFile<<20-rout_pl[i][0]-rout_pl[i][1]<<endl;  }
               outdromolFile<<"=============================="<<endl;   
      }
                              
            
