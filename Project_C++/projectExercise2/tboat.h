#ifndef TBOAT_H
#define TBOAT_H
#include "ticket1.h"
class ticketBoat : public Ticket
{     
      public:
                ticketBoat();
                ~ticketBoat();
            void  printState_bt(int );
             int  getTicket_bt(int ,int ,int );
             void  deleteTicket_bt(int ,int ,int ); 
             int  printdest_bt()const;
              void wfile_bt();
              
      private:
          
           string dromol_bt[3];
 };
 #endif         
