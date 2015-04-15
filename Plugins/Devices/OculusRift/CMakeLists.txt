cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)

project (MinVR_oculus)

#------------------------------------------
# Define the source and header files
#------------------------------------------

set (SOURCEFILES
OculusRiftDisplay.h
)

set (HEADERFILES
OculusRiftDisplay.cpp
)

source_group("Header Files" FILES ${HEADERFILES})

#------------------------------------------
# Download dependencies
#------------------------------------------

if(BUILD_DEPENDENCIES)
	set(osdk_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/osdk)
	make_directory(${osdk_checkout_Dir})
    message("   Library will be built from source in ${osdk_checkout_Dir}.")
	include(ExternalProject)
	set(osdk_PREFIX "${osdk_checkout_Dir}")
	set(osdk_INSTALL_DIR "${MINVR_INSTALL_DIR}/oculus")
	set(osdk_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${osdk_INSTALL_DIR}")
	set(osdk_DIR "${osdk_INSTALL_DIR}")

	if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set(osdk_CMAKE_ARGS ${osdk_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD=c++11 -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY=libc++)
	endif() 
	 
	 ExternalProject_add(osdk
		PREFIX ${osdk_PREFIX}
		GIT_REPOSITORY https://github.com/ivlab/OculusSdkMin.git
		GIT_TAG mesa
		INSTALL_DIR ${osdk_INSTALL_DIR}
		CMAKE_ARGS ${osdk_CMAKE_ARGS}
		UPDATE_COMMAND ""
   )
   
	set_property(TARGET osdk PROPERTY FOLDER "Dependencies")
	set(osdk_INCLUDE_DIR ${osdk_INSTALL_DIR}/include CACHE INTERNAL "Directory of osdk header files")
	if(MSVC)
		set(osdk_LIBRARY ${osdk_INSTALL_DIR}/lib/OculusVR.dll CACHE INTERNAL "osdk lib file")
	else()
		set(osdk_LIBRARY ${osdk_INSTALL_DIR}/lib/libOculusVR.so CACHE INTERNAL "osdk lib file")
	endif()
	add_definitions(-DUSE_osdk)
	set(LIBS_ALL ${LIBS_ALL} ${osdk_LIBRARY})
	include_directories(${osdk_INCLUDE_DIR})
	message(${osdk_INCLUDE_DIR})
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
	add_dependencies(${PROJECT_NAME} boost osdk MVRCore)
else()
	add_dependencies(${PROJECT_NAME} osdk MVRCore)
endif()
target_link_libraries(${PROJECT_NAME} ${osdk_LIBRARY})

#------------------------------------------
# Install Target
#------------------------------------------
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION "${MINVR_INSTALL_DIR}/plugins/include")

