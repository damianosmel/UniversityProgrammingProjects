#include "_legacyapps_enable_cmake.h"
#ifdef ENABLE_FLOODING

#include "legacyapps/flooding/flooding_message.h"

namespace flooding
{

	// ----------------------------------------------------------------------
   //FloodingMessage::
   //FloodingMessage(int hops,int parent)
	FloodingMessage::
   	FloodingMessage(int hops,int parent)
	{
		hops_ = hops;
		parent_ = parent;
	}
	// ----------------------------------------------------------------------
   FloodingMessage::
	~FloodingMessage()
	{}
   // ----------------------------------------------------------------------

}
#endif
