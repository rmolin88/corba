
#include <glog/logging.h>

#include "echo.hh"

int echo_client(CORBA::Object_ptr obj) 
{ 
	Echo_var e = Echo::_narrow(obj); 
	if (CORBA::is_nil(e)) 
	{
		LOG(ERROR) << "Cannot invoke on a nil object reference.\n";
		return -1;
	} 
	
	CORBA::String_var src = (const char*) "Hello World!"; 
	CORBA::String_var dest; 
	
	dest = e->echoString(src); 
	
	LOG(INFO) << "I said,\"" << src << "\"." 
		<< " The Object said,\"" << dest <<"\""; 

	return 1;
}
