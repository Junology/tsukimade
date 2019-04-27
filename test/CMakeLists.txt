find_package(PkgConfig)

set(PROJECT_TEST_NAME ${CMAKE_PROJECT_NAME}_test)

if(PKG_CONFIG_FOUND)
  pkg_check_modules(LUA ${PROJECT_LUA})

  if(LUA_FOUND)
	find_package(Threads REQUIRED)
	link_directories(${LUA_LIBRARY_DIRS})
	include_directories(${LUA_INCLUDE_DIRS})

	file(GLOB PROJECT_TEST_CSRC RELATIVE ${CMAKE_SOURCE_DIR} *.c)
	file(GLOB PROJECT_TEST_CXXSRC RELATIVE ${CMAKE_SOURCE_DIR} *.cpp)

	add_executable(
	  ${PROJECT_TEST_NAME}
	  ${PROJECT_TEST_CSRC}
	  ${PROJECT_TEST_CXXSRC})

	target_link_libraries(
	  ${PROJECT_TEST_NAME}
	  Threads::Threads
	  ${LUA_LIBRARIES})
  endif(LUA_FOUND)
endif(PKG_CONFIG_FOUND)
