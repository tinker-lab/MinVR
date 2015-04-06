cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)

project (MinVR_TUIO)

#------------------------------------------
# Define the source and header files
#------------------------------------------

set (SOURCEFILES
InputDeviceTUIOClient.cpp
TuioDeviceFactory.cpp
)

set (HEADERFILES
InputDeviceTUIOClient.H
TuioDeviceFactory.h
)

source_group("Header Files" FILES ${HEADERFILES})

#------------------------------------------
# Download dependencies
#------------------------------------------

find_package(LibTUIO)
if (LIBTUIO_FOUND)
	add_definitions(-DUSE_TUIO)
	set(LIBS_OPT ${LIBS_OPT} ${LIBTUIO_OPT_LIBRARIES})
	set(LIBS_DEBUG ${LIBS_DEBUG} ${LIBTUIO_DEBUG_LIBRARIES})
	set(LIBS_ALL ${LIBS_ALL} ${LIBTUIO_ALL_LIBRARIES})
	set(TUIO_LIBRARY ${LIBTUIO_ALL_LIBRARIES})
	include_directories(${LIBTUIO_INCLUDE_DIRS})
elseif(BUILD_DEPENDENCIES)
	set(tuio_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/tuio)
	make_directory(${tuio_checkout_Dir})
    message("   Library will be built from source in ${tuio_checkout_Dir}.")
	include(ExternalProject)
	set(tuio_PREFIX "${tuio_checkout_Dir}")
	set(tuio_INSTALL_DIR "${MINVR_INSTALL_DIR}/tuio")
	set(tuio_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${tuio_INSTALL_DIR}")
	set(tuio_DIR "${tuio_INSTALL_DIR}")
	set(tuio_PATCH ${CMAKE_COMMAND} -E copy ${tuio_checkout_Dir}/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt && ${CMAKE_COMMAND} -E copy ${tuio_checkout_Dir}/TuioServer.cpp <SOURCE_DIR>/TUIO/TuioServer.cpp)

	if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set(tuio_CMAKE_ARGS ${tuio_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD=c++11 -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY=libc++)
	endif()

	ExternalProject_add(TUIO
			PREFIX ${tuio_PREFIX}
			URL http://prdownloads.sourceforge.net/reactivision/TUIO_CPP-1.4.zip
			INSTALL_DIR ${tuio_INSTALL_DIR}
			CMAKE_ARGS ${tuio_CMAKE_ARGS}
			PATCH_COMMAND ${tuio_PATCH}
			UPDATE_COMMAND ""
	 )		 
	set_property(TARGET TUIO PROPERTY FOLDER "Dependencies")
	set(TUIO_INCLUDE_DIR ${tuio_INSTALL_DIR}/include CACHE INTERNAL "Directory of TUIO header files")
	if(MSVC)
		set(TUIO_LIBRARY ${tuio_INSTALL_DIR}/lib/TUIO.dll CACHE INTERNAL "TUIO lib file")
	else()
		set(TUIO_LIBRARY ${tuio_INSTALL_DIR}/lib/libTUIO.so CACHE INTERNAL "TUIO lib file")
	endif()
	add_definitions(-DUSE_TUIO)
	set(LIBS_ALL ${LIBS_ALL} ${TUIO_LIBRARY})
	include_directories(${TUIO_INCLUDE_DIR})
	message(${TUIO_INCLUDE_DIR})
endif()

#------------------------------------------
# Include Directories
#------------------------------------------
include_directories (
  .
  ${CMAKE_SOURCE_DIR}/dependencies/glm
  ${CMAKE_SOURCE_DIR}/MVRCore/include
)

#------------------------------------------
# Specific preprocessor defines
#------------------------------------------

# Windows Section #
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	# Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()

#------------------------------------------
# Set output directories to lib, and bin
#------------------------------------------
make_directory(${CMAKE_BINARY_DIR}/lib)
make_directory(${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach (CONF ${CMAKE_CONFIGURATION_TYPES})
	string (TOUPPER ${CONF} CONF)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/bin)
	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/lib)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/lib)
endforeach(CONF CMAKE_CONFIGURATION_TYPES)

#------------------------------------------
# Handle library naming
#------------------------------------------
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
set(CMAKE_MINSIZEREL_POSTFIX "s")
#set the build postfix extension according to the current configuration
if (CMAKE_BUILD_TYPE MATCHES "Release")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
	set(CMAKE_BUILD_POSTFIX "")
endif()

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

#------------------------------------------
# Build Target
#------------------------------------------
add_library ( ${PROJECT_NAME} SHARED ${HEADERFILES} ${SOURCEFILES} )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugins")
if(USE_BOOST)
	add_dependencies(${PROJECT_NAME} boost TUIO MVRCore)
else()
	add_dependencies(${PROJECT_NAME} TUIO MVRCore)
endif()
target_link_libraries(${PROJECT_NAME} ${TUIO_LIBRARY})


#------------------------------------------
# Install Target
#------------------------------------------
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION "${MINVR_INSTALL_DIR}/plugins/include")

