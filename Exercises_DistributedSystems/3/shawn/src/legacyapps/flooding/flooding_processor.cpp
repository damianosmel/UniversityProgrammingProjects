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
#include <ctime>
#include <cstdlib>
using namespace std;

namespace flooding
{

   // ----------------------------------------------------------------------
   FloodingProcessor::
   FloodingProcessor()
   {
	//initialize hops counter, parent random integer, sum and send_msg variable  of each processor
	hops_ = 20000;
	parent_ = -1;
	rand_int_ = 0;
	sum_ = 0;
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
   }
   // ----------------------------------------------------------------------

   // Kaleite mia fora prin apo ton  ton prwto gyro(iteration 0)
   void
   FloodingProcessor::
   boot( void )
      throw()
   {
	const shawn::SimulationEnvironment& se = owner().world().simulation_controller().environment();// read input parameteres of .conf file
	graph_diameter_ = se.optional_int_param("graph_diameter",0);
        num_msgs_ = se.optional_int_param("MSGS",0);
        am_ = se.optional_int_param("AM",0);

	if ( id() != 0 && id() % am_ == 0 )//let processors with id % AM = 0 expect of root to choose an integer randomly
	{
		srand( (unsigned)(time(0) + id()) );
		//choose a unique random integer with value between 1 and 40
		rand_int_ = (rand() % 40) + 1;
		cout << "Hello from node " << id() << ", my random chosen  integer = " << rand_int_ << endl;
	}//close if
	else
	{
		cout << "Hello from node " << id() << endl;
	}//close else
	
	//assume that flooding begins from processor with id = 0.
        if( id() == 0 )
        {
                //update the hops counter for root
                hops_ = 0;
                //parent_ = ;
                send( new FloodingMessage( hops_,0 ) );
                cout << "I am processor with id = "<< id() <<" (root) my parent's id = "<< parent_<<"."<<endl;
		cout <<"BFS_TREE: In " << hops_ << " hops, processor with id = " << id() << " is sending the flooding message!" <<endl;
        }//close if
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
     	if ( simulation_round() > graph_diameter_ )
	{
		if( id() != 0 ) // if processor isn't root check to see if it will start convergecasting
		{
			if ( id()%am_ == 0 && send_msg == false ) // if processor has id % AM = 0 then it will send its random chosen number
			{
				//use sum_ variable to store the number of send messages
				while( sum_ < num_msgs_ )//processor start to convergecast msgs number of messages to root
				{
					send( new FloodingMessage( rand_int_,parent_ ) );
					cout <<"CONVERGECAST: My id = "<< id() <<", (id%AM = 0) sending to my papa with id= " << parent_<< " for " << sum_ + 1 << " time(s)!"<<endl;
					sum_++;//increase number of send messages
				}//close while-loop
				send_msg = true;
			}//close 1 inner if
		}//close 2 inner if
	}//close external if
   }
   // ----------------------------------------------------------------------
   void 
   FloodingProcessor::
   handle_flooding_message( const FloodingMessage& flooding )
      throw()
   {
	//each processor prints the source of received message.
	cout <<"Processor with id = "<< id() << " received flooding msg from " << flooding.source_w().id() << endl;

	if ( simulation_round() <=  graph_diameter_ )//check whether this round is for tree construction or convergecast messages
	{
		//Handle tree construction message
		if ( hops_ > flooding.hops() + 1 )//check if stored hops is greater than send message's hops,if so update node's hops counter and parent, otherwise ignore it
		{
			//increase by one hops counter
			hops_ = flooding.hops() + 1;
			parent_ = flooding.parent();
			cout <<"I am processor with id = "<< id() <<" my parent's id = "<< parent_<<"."<<endl;
			//as processor's parent was changed, processor send message
			send( new FloodingMessage( hops_,id() ) );
                        cout <<"BFS_TREE: In "<< hops_ << " hops, processor with id = "<< id() <<" is sending the flooding message!"<<endl;
		}//close inner if
	}//close external if
	else//as now the tree has been constructed, processors convergecast the message of processor with id % 5 = 0 
	{
		//Handle convergecast message
		if ( id() != 0 )//check if processor  is internal node or root
		{
			if ( id() == flooding.parent() )//check if this processor is papa of sender
			{
				//if processor is the sender's papa send a new message to  papa of this processor
				send( new FloodingMessage( flooding.hops(),parent_ ) );
				cout <<"CONVERGECAST: My id = "<< id() <<", i received message from my children, sending to my papa with id=" << parent_<<" !"<<endl;
			}//close 1 inner if
		}//close 2 inner if
		else
		{
			//as convergecast message reachs the root, root adding its' random integer to sum
			//use rand_int_ to count messages for sum
			rand_int_++;
			sum_ = sum_ + flooding.hops();
			cout <<"SUM: My id =  "<< id()<<" (root) in "<< rand_int_ <<" addition, sum = "<< sum_ << endl;
		}//close inner else
	}//close else
   }
}
#endif
