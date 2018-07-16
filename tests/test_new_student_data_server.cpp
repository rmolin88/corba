#include <gtest/gtest.h>
#include <glog/logging.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>

#include "student_data.hh"

static int g_argc;
static char **g_argv;

static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr, CORBA::Object_ptr);

class StudentInfo_i : public POA_StudentInfo
{
	rapidjson::Document data;
	int load_json();
public:
	StudentInfo_i() 
	{ 
		if (load_json() != 1)
			throw std::runtime_error("Failed to load json data");
	}
	virtual ~StudentInfo_i() {}
	virtual CORBA::Long SetStudentData();
	virtual void GetStudentData();
};

CORBA::Long StudentInfo_i::SetStudentData()
{
	// TODO-[RM]-(Mon Jul 16 2018 09:15):  
	// Load all the data.json and output to screen
	// Need to save it somewhere
	return 8888;
}

void StudentInfo_i::GetStudentData()
{
	LOG(INFO) << "We are here";
}

TEST(Corba, OmniorbStudentDataServer)
{
	try
	{
		CORBA::ORB_ptr orb = CORBA::ORB_init(g_argc, g_argv, "omniORB4");
		CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

		PortableServer::Servant_var<StudentInfo_i> stud_info;
		ASSERT_NO_THROW({ stud_info = new StudentInfo_i(); });

		PortableServer::ObjectId_var stud_info_id = poa->activate_object(stud_info);

		obj = stud_info->_this();

		CORBA::String_var sior(orb->object_to_string(obj));

		LOG(INFO) << sior;
		ASSERT_TRUE(bindObjectToName(orb, obj));

		PortableServer::POAManager_var pman = poa->the_POAManager();
		pman->activate();

		orb->run();
	}
	catch (CORBA::SystemException& ex)
	{
		FAIL() << "Caught CORBA::" << ex._name();
	}
	catch (CORBA::Exception& ex)
	{
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

int StudentInfo_i::load_json()
{
	std::ostringstream file_content;

	{
		std::ifstream ifs("bin/data.json");

		if (ifs.is_open() == false)
		{
			LOG(ERROR) << "Failed to open file: ./data.json\n";
			return -2;
		}

		file_content << ifs.rdbuf();
	} // Close ifs

	rapidjson::ParseResult rc = data.Parse(file_content.str().c_str());

	if (rc.IsError() == true)
	{
		LOG(ERROR)	<< "Error ("
			<< static_cast<unsigned>(rc.Offset())
			<< "): "
			<< rapidjson::GetParseError_En(rc.Code())
			<< '\n';
		return -3;
	}

	return 1;
}

static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref)
{
	CosNaming::NamingContext_var rootContext;

	try {
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj = orb->resolve_initial_references("NameService");

		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);
		if (CORBA::is_nil(rootContext)) {
			LOG(ERROR) << "Failed to narrow the root naming context.";
			return 0;
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		LOG(ERROR) << "Caught NO_RESOURCES exception. You must configure omniORB "
			<< "with the location"
			<< "of the naming service.";
		return 0;
	}
	catch (CORBA::ORB::InvalidName&) {
		// This should not happen!
		LOG(ERROR) << "Service required is invalid [does not exist].";
		return 0;
	}

	try {
		// Bind a context called "test" to the root context:

		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id   = (const char*) "NewStudentData";       // string copied
		contextName[0].kind = (const char*) "Context"; // string copied
		// Note on kind: The kind field is used to indicate the type
		// of the object. This is to avoid conventions such as that used
		// by files (name.type -- e.g. test.ps = postscript etc.)

		CosNaming::NamingContext_var testContext;
		try {
			// Bind the context to root.
			testContext = rootContext->bind_new_context(contextName);
		}
		catch(CosNaming::NamingContext::AlreadyBound& ex) {
			// If the context already exists, this exception will be raised.
			// In this case, just resolve the name and assign testContext
			// to the object returned:
			CORBA::Object_var obj = rootContext->resolve(contextName);
			testContext = CosNaming::NamingContext::_narrow(obj);
			if (CORBA::is_nil(testContext)) {
				LOG(ERROR) << "Failed to narrow naming context.";
				return 0;
			}
		}
	}
	catch (CORBA::TRANSIENT& ex) {
		LOG(ERROR) << "Caught system exception TRANSIENT -- unable to contact the "
			<< "naming service."
			<< "Make sure the naming server is running and that omniORB is "
			<< "configured correctly.";

		return 0;
	}
	catch (CORBA::SystemException& ex) {
		LOG(ERROR) << "Caught a CORBA::" << ex._name()
			<< " while using the naming service.";
		return 0;
	}
	return 1;
}
