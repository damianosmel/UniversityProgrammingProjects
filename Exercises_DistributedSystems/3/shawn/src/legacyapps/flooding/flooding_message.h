#ifndef __SHAWN_LEGACYAPPS_FLOODING_MESSAGE_H
#define __SHAWN_LEGACYAPPS_FLOODING_MESSAGE_H
#include "_legacyapps_enable_cmake.h"
#ifdef ENABLE_FLOODING

#include "sys/message.h"

namespace flooding
{

   class FloodingMessage
       : public shawn::Message
   {
   public:
      FloodingMessage(int, int);
      virtual ~FloodingMessage();
	inline int hops( void ) const throw()
	{
		return hops_;
	};
	inline int parent( void ) const throw()
	{
		return parent_;
	};
   private:
	int hops_;
	int parent_;
   };
   // ----------------------------------------------------------------------
}

#endif
#endif
