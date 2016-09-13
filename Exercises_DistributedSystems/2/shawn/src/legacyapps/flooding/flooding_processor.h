/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_LEGACYAPPS_FLOODING_PROCESSOR_H
#define __SHAWN_LEGACYAPPS_FLOODING_PROCESSOR_H
#include "_legacyapps_enable_cmake.h"
#ifdef ENABLE_FLOODING

#include "sys/processor.h"
#include "legacyapps/flooding/flooding_message.h"

namespace flooding
{

   /**
    */
   class FloodingProcessor
       : public shawn::Processor
   {
   public:
      ///@name Constructor/Destructor
      ///@{
      FloodingProcessor();
      virtual ~FloodingProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      /**
       */
      virtual void special_boot( void ) throw();
      /**
       */
      virtual void boot( void ) throw();\
      /**
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      /**
       */
      virtual void work( void ) throw();
      ///@}

      void handle_flooding_message( const FloodingMessage& ) throw();

   private:
	bool send_msg; 
      // --------------------------------------------------------------------
   };


}

#endif
#endif
