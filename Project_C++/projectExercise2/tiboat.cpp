#include <iostream>
using namespace std;
#include <iomanip>
#include <fstream>
#include <cstdlib>
             using std::ios;
              using std::cerr;
              using std::endl;
              using std::ofstream;
#include  "tboat.h"

      int rout_bt[3][3];
      
      //initialize availability for boat tickets
      ticketBoat::ticketBoat():Ticket(-1,10,24,3) 
      {
            for(int i=0;i<=2;i++){
                  rout_bt[i][0]=-1;
                  rout_bt[i][1]=10;
                  rout_bt[i][2]=24; }
             //the first category has negative number of seats as I cannot have an available seat
      }
      
      ticketBoat::~ticketBoat(){
              // delete [] dromol_bt;
              // delete [] rout_bt;      
              }
     
     //reserve a ticket
     int ticketBoat::getTicket_bt(int kathg,int dest,int num_krat)
      { 
               int g= rout_bt[dest][kathg];
               if((g>=num_krat)&&(g!= -1))//check if there are sufficient available seats and selected category can be selected at current means of transport
               {   rout_bt[dest][kathg]-=num_krat;
                  
                  //get number of the available seats
                   if(kathg==1){
                          return 10-rout_bt[dest][kathg];} 
                   else if(kathg==2){
                          return 24-rout_bt[dest][kathg]; }       
               }
               else if(kathg!=0) //if we are short on tickets availability we suggest new categories
               {    cout<<"Sorry, we cannot provide more seats at selected category"<<endl;
                    cout<<"Info: There are available seats at the following categories:";  
                    for(int i=0;i<=2;i++){
                            if((i!=kathg)&&(rout_bt[dest][i]!=0)){
                              cout << "At " << i+1 << " category, there are " << rout_bt[dest][i] << " available seats";} 
                                  cout<<"\n"; 
                    }
                    return -2;
               }
               if(kathg==0) {
                    cout <<"We are sorry to inform that our agency cannot reserve seat at selected category if the currently selected means of transport" << endl;
                    return -2;}     
       }
       
        //print transportation schedule for boats
        int ticketBoat::printdest_bt()const{
            string dromol_bt[3]={"Patra-Angona","Patra-Venice","Patra-Brindisi"};
            int proorismos;
                    cout<<"Info: Our agency for boats can provide for 2nd category with 10 seats as the maximum number and to 3rd category with 24 seats as maximum number"<<endl;
                    cout<<"For boat the transportation schedule is:";             
                   for(int i=0;i<=2;i++)
                       cout<<"\n"<<dromol_bt[i]<<endl;
                       cout<<"For which of the boats you would like to reserve a seat, please hit its ascending number"<<endl;
                       cin>>proorismos;
                       return proorismos-1;
           }        
      
      //cancel ticket reservation                 
      void  ticketBoat::deleteTicket_bt(int kathgr,int des,int num_del){
                               int stoixeia_akur=rout_bt[des][kathgr];
                           if(stoixeia_akur!=-1){
                                       switch (kathgr){ 
                                              case 1:                  
                                                     if(num_del<=10-stoixeia_akur)
                                                        rout_bt[des][kathgr]+=num_del;
                                                     else
                                                         cout<<"You cannot cancel " << num_del << "tickets, as you had reserved only " << 10-stoixeia_akur<<endl;  
                                                    break;
                                              case 2:
                                                      if(num_del<=24-stoixeia_akur)
                                                        rout_bt[des][kathgr]+=num_del;
                                                      else
                                                        cout<<"You cannot cancel " << num_del << "tickets,  as you had reserved only " << 24-stoixeia_akur<<endl;                                       
                                             break;             
                                             }
                            }//close if                    
      }
      
      //print boats tickets services
      void  ticketBoat::printState_bt(int proor){
                                 #include <iostream>
                                 using std::cout;
                                 using std::endl;
                                 using std::setw;
                                      int res_b= rout_bt[proor][1];
                                      int res_c=rout_bt[proor][2] ;
                                      int res_seats=res_b +res_c;
                                      cout<<right;
                                      for (int i=0; i<24; i++)
                                          cout << "=";
                                      cout<<"BOAT";
                                 for (int i=0; i<26; i++)
                                        cout << "=";
                                         cout << endl;
                                      
                                      cout<<"ROUTE:"<<setw(25)<<(dromol_bt[proor])<<endl;
                                      cout<<"TOTAL SEATS:"<<setw(9)<<34<<endl;
                                      cout<<"RESERVED SEATS:"<<setw(6)<<34-res_seats<<endl;
                                      cout<<"AVAILABLE SEATS:"<<setw(4)<<res_seats<<endl;
                                      for (int i=0; i<21; i++)
                                          cout <<"-";
                                      cout<<" RESERVED ";
                                      for (int i=0; i<21; i++)
                                          cout <<"-";
                                          cout<<endl;
                                      cout<<setw(26)<<"B CLASS"<<setw(12)<<"C CLASS"<<endl;
                                      cout<<setw(24)<<10-res_b<<setw(13)<<24-res_c<<endl;
                                      for (int i=0; i<21; i++)
                                          cout <<"-";
                                      cout<<" AVAILABLE ";
                                      for (int i=0; i<21; i++)
                                          cout<<"-";
                                          cout<<endl;
                                      cout<<setw(26)<<"B CLASS"<<setw(12)<<"C CLASS"<<endl;
                                      cout<<setw(24)<<res_b<<setw(13)<<res_c<<endl;
                                      for(int i=0;i<54;i++)
	                                        cout << "=";
	                                        cout<<endl; 
                                      }

      //write boats tickets services to the general service file                                
      void ticketBoat::wfile_bt(){
          
         string dromol_bt[3]={"Patra-Angona","Patra-Benetia","Patra-Mprintezi"};
                  ofstream outdromolFile;
                  outdromolFile.open("kratiseis.dat",ios::app); //write the new tickets service keeping the already saved data
		   if(!outdromolFile){
                   cerr<<"Error, accessing service file"<<endl;
                   exit(1);}
     else{         outdromolFile<<"----------------------------------"<<endl;
                   outdromolFile<<"BOAT"<<endl;
                   outdromolFile<<"ROUTES:"<<endl;
                   for(int i=0;i<=2;i++){
                        outdromolFile<<dromol_bt[i]<<endl;  
                        outdromolFile<<"RESERVED SEATS:"<<endl;
                        outdromolFile<<"B class       C class"<<endl;   
                        outdromolFile<<10-rout_bt[i][1]<<"        "<<24-rout_bt[i][2]<<endl;  
                         outdromolFile<<"AVAILABLE SEATS:"<<endl;
                        outdromolFile<<rout_bt[i][1]<<"       "<<rout_bt[i][2]<<endl; 
                        outdromolFile<<"TOTAL  SEATS :"<<endl;
                        outdromolFile<<34<<endl;
                        outdromolFile<<"RESERVED SEATS :"<<endl;
                        outdromolFile<<34-rout_bt[i][1]-rout_bt[i][2]<<endl; 
                        } 
                        }
     }
     
            
