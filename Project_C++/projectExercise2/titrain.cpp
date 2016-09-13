#include <iostream>
using namespace std;
#include <iomanip>
#include <fstream>
#include <cstdlib>
             using std::ios;
              using std::cerr;
              using std::endl;
              using std::ofstream;
#include  "ttrain.h"
 string dromol_tr[5]={"PATRA-ATHENS","PATRA-PYRGOS","ATHINA-THES/NIKH","ATHINA-LIANOKLADI","ATHINA-LARISA"};
 int rout_tr[5][3];
      
      //initialize availability for train tickets
      ticketTrain::ticketTrain():Ticket(6,12,-1,5) 
      { 
          for(int i=0;i<=4;i++){
                  rout_tr[i][0]=6;
                  rout_tr[i][1]=12;
                  rout_tr[i][2]=-1; }
             //the third category has negative number of seats as I cannot have an available seat
      }
      ticketTrain::~ticketTrain(){
                  // delete  dromol_tr[5];
              // delete [] rout_tr; 
                   }
     
     //reserve a ticket
     int ticketTrain::getTicket_tr(int kathg,int dest,int num_krat)
      { 
            
                             
               if((rout_tr[dest][kathg]>=num_krat)&&(rout_tr[dest][kathg]!= -1))//check if there are sufficient available seats and selected category can be selected at current means of transport
               {rout_tr[dest][kathg]-=num_krat;
                  
                  //get number of the available seats
                  if(kathg==0){  
                       return 6-rout_tr[dest][kathg];}
                   else if (kathg==1){
                       return 12-rout_tr[dest][kathg];} 
                }
               else if (kathg!=2)//if we are short on tickets availability we suggest new categories
               {    cout<<"Sorry, we cannot provide more seats at selected category"<<endl;
                    cout<<"Info: There are available seats at the following categories:"<<endl;  
                    for(int i=0;i<=2;i++){
                            if((i!=kathg)&&(i!=2)){
                               cout<< "At " << i+1 << " category, there are " << rout_tr[dest][i] << " available seats " << endl; 
                              }  
                              cout<<"\n";   
                    }//close for
                    return -2;
               }
               if (kathg==2) {
                    cout << "We are sorry to inform that our agency cannot reserve seat at selected category if the currently selected means of transport" << endl;
                      return -2;}
       }
       
       //print transportation schedule for trains
        int ticketTrain::printdest_tr()const{
            int proorismos;
					
                    cout<<"Info: Our agency for trains can provide for 1st category with 6 seats as the maximum number and to 2nd category with 12 seats as maximum number"<<endl;
                    cout<<"For train the transportation schedule is: " << endl;             
                   for(int i=0;i<=4;i++)
                       cout<<"\n"<<dromol_tr[i]<<endl;
                       cout<<"\n";
                       cout<<"For which of the trains you would like to reserve a seat, please hit its ascending number" << endl;
                       cin>>proorismos;
                       return proorismos-1;
        }
      
      //cancel ticket reservation        
      void  ticketTrain::deleteTicket_tr(int kathgr,int des,int num_del){
                           int stoixeia_akur=rout_tr[des][kathgr]; //get the information for the reservation to be canceled
                               
                           if(stoixeia_akur!=-1){
                                       switch (kathgr){ 
                                              case 0:                  
                                          if(num_del<=6-stoixeia_akur)
                                             rout_tr[des][kathgr]+=num_del;
                                             
                                          else
                                                cout<<"You cannot cancel " << num_del << " tickets, as you had reserved only " << 6-stoixeia_akur<<endl;
                                             break;
                                             case 1:
                                               if(num_del<=12-stoixeia_akur)
                                                 rout_tr[des][kathgr]+=num_del;
                                               else
                                                  cout<<"You cannot cancel " <<num_del << " tickets, as you had reserved only " << 16-stoixeia_akur<<endl; 
                                             break;             
                                             }
                            }//close if                   
        }
      
      //print boats tickets services
      void  ticketTrain::printState_tr(int proor){
            string dromol_tr[5]={"PATRA-ATHENS","PATRA-PYRGOS","ATHENS-THES/NIKH","ATHENS-LIANOKLADI","ATHENS-LARISA"};
                                 #include <iostream>
                                 using std::cout;
                                 using std::endl;
                                 using std::setw;
                                 #include <string>
                                 using std::string;
                                 
                                  int res_a=rout_tr[proor][0] ;
                                      int res_b=rout_tr[proor][1] ;
                              int res_seats=res_a +res_b;
                               for (int i=0; i<24; i++)
                                          cout << "=";
                                      cout<<"TRAIN";
                                 for (int i=0; i<26; i++)
                                        cout << "=";
                                         cout << endl;      
                                      
                                  
                                      cout<<"ROUTE:"<<setw(25)<<(dromol_tr[proor])<<endl;
                                      cout<<"TOTAL SEATS:"<<setw(9)<<18<<endl;
                                      cout<<"RESERVED SEATS:"<<setw(6)<<18-res_seats<<endl;
                                      cout<<"AVAILABLE SEATS:"<<setw(4)<<res_seats<<endl;
                                      for (int i=0; i<21; i++)
                                          cout <<"-";
                                      cout<<" RESERVED ";
                                      for (int i=0; i<21; i++)
                                          cout <<"-";
                                          cout<<endl;
                                      cout<<setw(26)<<"A CLASS"<<setw(12)<<"B CLASS"<<endl;
                                      cout<<setw(24)<<6-res_a<<setw(13)<<12-res_b<<endl;
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
       
       //write train tickets services to the general service file                                
       void ticketTrain::wfile_tr(){
          
         
                  ofstream outdromolFile("kratiseis.dat",ios::app); //write the new tickets service keeping the already saved data
		   if(!outdromolFile){
                   cerr<<"Error, accessing service file"<<endl;
                   exit(1);}
                   outdromolFile<<"=============================="<<endl;
                   outdromolFile<<"TRAIN"<<endl;
                   outdromolFile<<"ROUTES:"<<endl;
                   for(int i=0;i<=4;i++){
                       outdromolFile<<dromol_tr[i]<<endl;  
                        outdromolFile<<"RESERVED SEATS:"<<endl;
                        outdromolFile<<"A class       B class"<<endl;    
                        outdromolFile<<6-rout_tr[i][0]<<"       "<<12-rout_tr[i][1]<<endl;  
                        outdromolFile<<"AVAILABLE SEATS:"<<endl;
                        outdromolFile<<"A class       B class"<<endl; 
                        outdromolFile<<rout_tr[i][0]<<"         "<<rout_tr[i][1]<<endl; 
                        outdromolFile<<"TOTAL  SEATS :"<<endl;
                        outdromolFile<<18<<endl;;
                        outdromolFile<<"RESERVED SEATS :"<<endl;
                        outdromolFile<<18-rout_tr[i][0]-rout_tr[i][1]<<endl; 
                        } 
    }
            
