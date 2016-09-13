/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_legacyapps_enable_cmake.h"
#ifdef ENABLE_FLOODING

#include "legacyapps/flooding/flooding_processor.h"
#include "legacyapps/flooding/flooding_message.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node.h"
#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

namespace flooding
{

   // ----------------------------------------------------------------------
   FloodingProcessor::
   FloodingProcessor()
   {
	//at start any processor didn't send message
	send_msg = false;
   }
   // ----------------------------------------------------------------------
   FloodingProcessor::
   ~FloodingProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   FloodingProcessor::
   special_boot( void )
      throw()
   {
   	//assume that flooding begins from processor with id = 0.
	if( id() == 0 )
	{
		send( new FloodingMessage() );
		cout <<"Process with id = "<< id() <<" is sending the flooding message!"<<endl;
		//as processor sent message change the bool send_msg
		send_msg = true;
		set_state(Inactive);
	}
   }
   // ----------------------------------------------------------------------
   
   // Kaleite mia fora prin apo ton  ton prwto gyro(iteration 0)
   void
   FloodingProcessor::
   boot( void )
      throw()
   {
       cout << "Hello from node " << id() << endl;
   }
   // ----------------------------------------------------------------------

   // Kaleite kathe fora poy h diergasia lambanei ena mhnyma
   // To orhsma einai mia class poy periexei to mhnyma
   bool
   FloodingProcessor::
   process_message( const shawn::ConstMessageHandle& mh ) 
      throw()
   {
       // mh.get(): epistrefei to mhnyma
       // dynamic_cast<const FloodingMessage*> : to metatrepoyme sthn class poy
      const FloodingMessage* floodmsg = dynamic_cast<const FloodingMessage*> ( mh.get() );

      if ( floodmsg != NULL && id() != floodmsg->source().id() )
      {
          //kaloume mia method gia na xeirhstoume to mhnyma typou flooding
         handle_flooding_message( *floodmsg );
         return true;
      }

      return shawn::Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
   //Ekteleite se kathe gyro(iteration) ths exomoiwshs
   void
   FloodingProcessor::
   work( void )
      throw()
   {
      // cout << "H diergasia " << id() << " briskete ston gyro " << simulation_round() << endl;
   }
   // ----------------------------------------------------------------------
   void 
   FloodingProcessor::
   handle_flooding_message( const FloodingMessage& flooding )
      throw()
   {
	//each processor prints the source of received message. 
	cout << id() << " Received flooding msg from " << flooding.source_w().id() << endl;
	//now if processor has not sent message sent to its' neighbors
	if ( send_msg == false )
	{
		send( new FloodingMessage() );
		cout<<"Process with id = "<<id()<<" is sending the flooding message!"<<endl;
		send_msg = true;
		//as process  has just sent flooding message, now it will become inactive
		set_state(Inactive);
	} 
	/*if ( send_msg == true )
	{
		set_state(Inactive);
	}*/
   }
}
#endif
