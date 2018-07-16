include_directories(${CMAKE_SOURCE_DIR}/inc)

include_directories(SYSTEM
	"${GLOG_INCLUDE_DIRS}"
	"${GTEST_INCLUDE_DIRS}"
	)

# Add executable and list sources to the executable
add_executable(eg3_clt 
		eg3_clt.cc
		echoSK.cc
	)

# Link test executable against gtest & gtest_main
target_link_libraries(eg3_clt
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
		/usr/lib/libomniORB4.so
		/usr/lib/libomnithread.so
	)

add_executable(eg3_impl 
		eg3_impl.cc
		echoSK.cc
	)

# Link test executable against gtest & gtest_main
target_link_libraries(eg3_impl
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
		/usr/lib/libomniORB4.so
		/usr/lib/libomnithread.so
	)
