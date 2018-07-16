include_directories(${CMAKE_SOURCE_DIR}/inc)

include_directories(SYSTEM
		"${GLOG_INCLUDE_DIRS}"
		"${GTEST_INCLUDE_DIRS}"
		"${RAPIDJSON_INCLUDE_DIRS}"
	)

# Add executable and list sources to the executable
add_executable(test_corba_client 
		test_corba_client.cpp
		${CMAKE_SOURCE_DIR}/src/echoSK.cc
		${CMAKE_SOURCE_DIR}/src/echo_client.cc
	)

# Link test executable against gtest & gtest_main
target_link_libraries(test_corba_client
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
		/usr/lib/libomniORB4.so
		/usr/lib/libomnithread.so
	)

add_test(test_corba_client ${EXECUTABLE_OUTPUT_PATH})

# --------------------------

add_executable(test_new_student_data_client
	test_new_student_data_client.cpp
	${CMAKE_SOURCE_DIR}/src/student_dataSK.cc
	)

# Link test executable against gtest & gtest_main
target_link_libraries(test_new_student_data_client
	${GTEST_BOTH_LIBRARIES}
	${GLOG_LIBRARIES}
	/usr/lib/libomniORB4.so
	/usr/lib/libomnithread.so
	)

add_test(test_new_student_data_client ${EXECUTABLE_OUTPUT_PATH})

# --------------------------

add_executable(test_new_student_data_server
		test_new_student_data_server.cpp
		${CMAKE_SOURCE_DIR}/src/student_dataSK.cc
	)

# Link test executable against gtest & gtest_main
target_link_libraries(test_new_student_data_server
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
		/usr/lib/libomniORB4.so
		/usr/lib/libomnithread.so
	)

add_test(test_new_student_data_server ${EXECUTABLE_OUTPUT_PATH})
