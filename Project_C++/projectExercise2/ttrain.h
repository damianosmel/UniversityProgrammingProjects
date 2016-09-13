#ifndef TTRAIN_H
#define TTRAIN_H
#include "ticket1.h"
class ticketTrain : public Ticket
{     
      public:
                ticketTrain();
                ~ticketTrain();
             void  printState_tr(int );
             int  getTicket_tr(int ,int ,int ) ;
             void  deleteTicket_tr(int ,int ,int ); 
             int  printdest_tr() const;
             void wfile_tr(); 
      private:
             
             string dromol_pl[5];
      };
 #endif         
