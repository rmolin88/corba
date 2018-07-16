
#ifndef ECHO_SERVANT_HPP
#define ECHO_SERVANT_HPP

class Echo_i : public POA_Echo
{
	public:
		Echo_i() {}
		virtual ~Echo_i() {}
		virtual char* echoString(const char* mesg) { return CORBA::string_dup(mesg); }
};

#endif
