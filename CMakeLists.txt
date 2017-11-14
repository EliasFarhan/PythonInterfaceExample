cmake_minimum_required(VERSION 3.3)
project(PyInterface)

include(cmake/IntelCompileFeatures.cmake)

include(cmake/EnumeratedOption.cmake)
include(cmake/CheckCXXFeature.cmake)
include(cmake/CheckCXXStandardLibrary.cmake)

message(STATUS "CXX compiler ID: '${CMAKE_CXX_COMPILER_ID}'")
message(STATUS "CXX compiler Version: '${CMAKE_CXX_COMPILER_VERSION}'")

enum_option(BUILD_CXX_STANDARD
  DOC "C++ Standard to compile against"
  VALUES 11 14 17
  CASE_INSENSITIVE
  )
message(STATUS "List of CXX features known to CMake")
	get_property(_cxx_known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
	message(STATUS "List of supported CXX11 features")
	foreach(_f ${CMAKE_CXX11_COMPILE_FEATURES})
	  message("  ${_f}")
	endforeach()
	message(STATUS "List of supported CXX14 features")
	foreach(_f ${CMAKE_CXX14_COMPILE_FEATURES})
	  message("  ${_f}")
	endforeach()
	message(STATUS "List of supported CXX17 features")
	foreach(_f ${CMAKE_CXX17_COMPILE_FEATURES})
		message("  ${_f}")
	endforeach()
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)

include_directories(include ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE PyInterface_SRC src/*/*.cpp include/*.h)


#PYTHON
IF(NOT WIN32)
	add_definitions(-Wall -Wextra -pedantic -std=c++11)

	IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
		add_definitions(-g -O0)
	ELSE()
		add_definitions(-O3)
	ENDIF()

	LIST(APPEND PyInterface_LIBRARIES resolv)
ELSE()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()


find_package(PythonLibs 3 EXACT REQUIRED)

include_directories(${PYTHON_INCLUDE_DIR})

LIST(APPEND PyInterface_LIBRARIES ${PYTHON_LIBRARY})

#PYBIND11
set(PYBIND11_DIR ${EXTERNAL_DIR}/pybind11/)
add_subdirectory(${PYBIND11_DIR})
include_directories(${PYBIND11_DIR})
LIST(APPEND PyInterface_LIBRARIES
  pybind11)

#PyInterface_COMMON
if(NOT WIN32)
LIST(APPEND PyInterface_LIBRARIES
  stdc++fs)
ENDIF()
add_library(PyInterface_COMMON STATIC ${PyInterface_SRC})
include_directories(include/)
target_link_libraries(PyInterface_COMMON PUBLIC ${PyInterface_LIBRARIES})

#PyInterface
add_executable(PyInterface src/main.cpp)

MESSAGE("PyInterface_COMMON: " ${PyInterface_COMMON})

target_link_libraries(PyInterface PUBLIC PyInterface_COMMON)
