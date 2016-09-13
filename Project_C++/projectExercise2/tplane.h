#ifndef TPLANE_H
#define TPLANE_H
#include "ticket1.h"
class ticketPlane : public Ticket
{     
      public:
                ticketPlane();
                ~ticketPlane();
             void  printState_pl(int );
             int  getTicket_pl(int ,int ,int );
             void  deleteTicket_pl(int ,int ,int ); 
             int  printdest_pl()const;
              void wfile_pl();
      private:
            
            string dromol_pl[4];
 };
 #endif         
