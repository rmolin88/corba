#include <gtest/gtest.h>
#include <glog/logging.h>
#include <string>

#include "echo.hh"
#include "echo_servant.hpp"
#include "echo_client.hpp"

int g_argc;
char **g_argv;

TEST(Corba, OmniorbEchoExample)
{

	CORBA::ORB_ptr orb = CORBA::ORB_init(g_argc, g_argv, "omniORB4");
	CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");

	PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

	PortableServer::Servant_var<Echo_i> echo = new Echo_i();
	PortableServer::ObjectId_var echo_id = poa->activate_object(echo);

	Echo_var echo_ref = echo->_this();

	PortableServer::POAManager_var pman = poa->the_POAManager();
	pman->activate();

	EXPECT_EQ(echo_client(echo_ref),1);

	orb->destroy();
}

int main(int argc, char *argv[])
{
	FLAGS_logtostderr = 1;
	FLAGS_colorlogtostderr = 1;
	::google::InitGoogleLogging(argv[0]);
	::testing::InitGoogleTest(&argc, argv);

	g_argc = argc;
	g_argv = argv;
	return RUN_ALL_TESTS();
}
