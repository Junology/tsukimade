find_package(PkgConfig)

set(PROJECT_TEST_NAME ${CMAKE_PROJECT_NAME}_test)
set(PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

if(PKG_CONFIG_FOUND)
  pkg_check_modules(LUA ${PROJECT_LUA})

  if(LUA_FOUND)
	find_package(Threads REQUIRED)
	link_directories(${LUA_LIBRARY_DIRS})
	include_directories(${LUA_INCLUDE_DIRS})

	file(GLOB PROJECT_TEST_CSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
	file(GLOB PROJECT_TEST_CXXSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
	file(GLOB PROJECT_TEST_LUA RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.lua)

	# Compile setting
	set(CMAKE_CXX_FLAGS "-O0 -g")
	set(CMAKE_C_FLAGS "-O0 -g")

	# Create the common static library.
	add_library(testcommon STATIC
	  common.cpp)

	add_executable(
	  ${PROJECT_TEST_NAME}_vect2d
	  vect2d.c
	  test_vect2d.cpp)

	target_link_libraries(
	  ${PROJECT_TEST_NAME}_vect2d
	  testcommon
	  Threads::Threads
	  ${LUA_LIBRARIES})

	configure_file(test_vect2d.lua ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

	add_test(
	  NAME test_vect2d
	  COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}_vect2d> ${CMAKE_CURRENT_BINARY_DIR}/test_vect2d.lua)

	add_executable(
	  ${PROJECT_TEST_NAME}_list
	  list.c
	  test_list.cpp)

	target_link_libraries(
	  ${PROJECT_TEST_NAME}_list
	  testcommon
	  Threads::Threads
	  ${LUA_LIBRARIES})

	configure_file(test_list.lua ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

	add_test(
	  NAME test_list
	  COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}_list> ${CMAKE_CURRENT_BINARY_DIR}/test_list.lua)

  endif(LUA_FOUND)
endif(PKG_CONFIG_FOUND)
