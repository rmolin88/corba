#include <gtest/gtest.h>
#include <glog/logging.h>
#include <string>
#include <iostream>

#include "student_data.hh"

int g_argc;
char **g_argv;

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb);

// IOR:010000002b00000049444c3a6f6d672e6f72672f436f734e616d696e672f4e616d696e67436f6e746578744578743a312e300000010000000000000070000000010102000e0000003139322e3136382e312e31393300f90a0b0000004e616d6553657276696365000300000000000000080000000100000000545441010000001c000000010000000100010001000000010001050901010001000000090101000354544108000000f6bb4c5b01005181
// IOR:010000001400000049444c3a53747564656e74496e666f3a312e3000010000000000000064000000010102000e0000003139322e3136382e312e3139330047c40e000000fe1dbe4c5b00005a74000000000000000200000000000000080000000100000000545441010000001c00000001000000010001000100000001000105090101000100000009010100

TEST(Corba, OmniorbStudentDataClient)
{

	try 
	{
		std::string buf;
		CORBA::ORB_ptr orb = CORBA::ORB_init(g_argc, g_argv, "omniORB4");
		CORBA::Object_var obj = CORBA::Object::_nil();

		ASSERT_NO_THROW({ obj = getObjectReference(orb); });
		ASSERT_FALSE(CORBA::is_nil(obj));
		ASSERT_FALSE(CORBA::is_nil(orb));

		// StudentInfo_var stud_data = StudentInfo::_narrow(obj);
		// StudentInfo_ptr p_stud_data = stud_data;
		StudentInfo_ptr p_stud_data = (StudentInfo_ptr) StudentInfo::_narrow(obj);
		ASSERT_FALSE(CORBA::is_nil(p_stud_data));

		do{ // Get the cryptographic key
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin >> buf;
			}
			std::cout << "Press any key to set the data from the file and display it.\n";
			std::cout << "Press \"q\" to exit.\n";
			std::cin >> buf;

			if ((buf.length() > 0) && (buf.compare("q") == 0))
				break;

			ASSERT_EQ(p_stud_data->SetStudentData(), (CORBA::Long) 1);
			p_stud_data->GetStudentData();
		} while (1);

		orb->destroy();
	}
	catch (CORBA::TRANSIENT&) {
		FAIL() << "Caught system exception TRANSIENT -- unable to contact the "
			<< "server.";
	}
	catch (CORBA::SystemException& ex) {
		FAIL() << "Caught a CORBA::" << ex._name();
	}
	catch (CORBA::Exception& ex) {
		FAIL() << "Caught CORBA::Exception: " << ex._name();
	}
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

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb)
{
	CosNaming::NamingContext_var rootContext;

	try {
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references("NameService");

		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);

		if (CORBA::is_nil(rootContext)) {
			LOG(ERROR) << "Failed to narrow the root naming context.";
			return CORBA::Object::_nil();
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		LOG(ERROR) << "Caught NO_RESOURCES exception. You must configure omniORB "
			<< "with the location of the naming service.";
		return CORBA::Object::_nil();
	}
	catch (CORBA::ORB::InvalidName& ex) {
		// This should not happen!
		LOG(ERROR) << "Service required is invalid [does not exist].";
		return CORBA::Object::_nil();
	}

	// Create a name object, containing the name test/context:
	CosNaming::Name name;
	name.length(2);

	name[0].id   = (const char*) "NewStudentData";       // string copied
	name[0].kind = (const char*) "Context"; // string copied
	name[1].id   = (const char*) "Json";
	name[1].kind = (const char*) "Object";
	// Note on kind: The kind field is used to indicate the type
	// of the object. This is to avoid conventions such as that used
	// by files (name.type -- e.g. test.ps = postscript etc.)

	try {
		// Resolve the name to an object reference.
		return rootContext->resolve(name);
	}
	catch (CosNaming::NamingContext::NotFound& ex) {
		// This exception is thrown if any of the components of the
		// path [contexts or the object] aren't found:
		LOG(ERROR) << "Context not found.";
		return CORBA::Object::_nil();
	}
	catch (CORBA::TRANSIENT& ex) {
		LOG(ERROR) << "Caught system exception TRANSIENT -- unable to contact the "
			<< "naming service."
			<< "Make sure the naming server is running and that omniORB is "
			<< "configured correctly.";
		return CORBA::Object::_nil();
	}
	catch (CORBA::SystemException& ex) {
		LOG(ERROR) << "Caught a CORBA::" << ex._name()
			<< " while using the naming service.";
		return CORBA::Object::_nil();
	}
	return CORBA::Object::_nil();
}
