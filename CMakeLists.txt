cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

project (MinVR)

set(MINVR_VERSION_MAJOR "0")
set(MINVR_VERSION_MINOR "9")
set(MINVR_VERSION_PATCH "0")
set(MINVR_VERSION_EXTRA "")
set(MINVR_VERSION "${MINVR_VERSION_MAJOR}.${MINVR_VERSION_MINOR}")
set(MINVR_VERSION_FULL "${MINVR_VERSION}.${MINVR_VERSION_PATCH}${MINVR_VERSION_EXTRA}")

option(USE_BOOST "Forces the application to use Boost" OFF)

option(USE_APPKIT_GLFW "Enable to use the GLFW app kit" ON)
option(USE_APPKIT_G3D9 "Enable to use the G3D9 app kit" OFF)
option(USE_APPKIT_GLUT "Enable to use the GLut app kit" OFF)

option(BUILD_USE_SOLUTION_FOLDERS "Enable grouping of projects in Visual Studio" ON)
option(BUILD_EXAMPLES "Enable to build app kit example projects" ON)
option(BUILD_DEPENDENCIES "If enabled, dependencies will be downloaded and built if an installed version is not found" ON)
option(BUILD_DOCUMENTATION "If enable, cmake attempts to find Doxygen and build the API documentation" ON)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ${BUILD_USE_SOLUTION_FOLDERS})

set(MINVR_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/MinVR" CACHE PATH "Install location for MinVR")
add_definitions(-DINSTALLPATH="${MINVR_INSTALL_DIR}")

if (BUILD_DOCUMENTATION)
	set(DOXYGEN_SKIP_DOT TRUE)
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		add_subdirectory(docs)
	endif()
endif()

find_package(OpenGL REQUIRED)
set(LIBS_ALL ${LIBS_ALL} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
include_directories(${OPENGL_INCLUDE_DIR})

if(USE_BOOST)
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_MULTITHREADED ON)  
	set(Boost_USE_STATIC_RUNTIME OFF)
	set(Boost_ADDITIONAL_VERSIONS "1.54" "1.54.0")
	find_package(Boost 1.55.0 COMPONENTS chrono date_time system thread log log_setup filesystem)
	if(Boost_FOUND)
	    message("Using installed version of Boost")
	    set(BUILD_BOOST OFF)
	elseif(BUILD_DEPENDENCIES)
		set(BUILD_BOOST ON)
	     set(Boost_Download_Dir ${CMAKE_SOURCE_DIR}/dependencies/boost)
		 make_directory(${Boost_Download_Dir})
	     message("   Library will be built from source in ${Boost_Download_Dir}.")
	     
	     include(ExternalProject)
	     set(boost_PREFIX "${Boost_Download_Dir}")
	     set(boost_INSTALL_DIR "${MINVR_INSTALL_DIR}/boost")
	     if (MSVC)
	        set(boost_CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.bat)
	        set(boost_BUILD_COMMAND <SOURCE_DIR>/b2 install --prefix=${boost_INSTALL_DIR} --with-thread --with-chrono --with-date_time --with-log --with-filesystem toolset=msvc-10.0 link=static runtime-link=shared threading=multi address-model=64 variant=debug,release)
	     else()
	        set(boost_CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.sh cxxflags="-std=c++11 -stdlib=libc++" --with-toolset=clang)
	        set(boost_BUILD_COMMAND <SOURCE_DIR>/b2 install --prefix=${boost_INSTALL_DIR} --with-thread --with-chrono --with-date_time --with-log --with-filesystem toolset=clang cxxflags=-std=c++11 cxxflags=-stdlib=libc++ linkflags=-stdlib=libc++ link=static runtime-link=shared threading=multi address-model=64 variant=release)
	     endif()
	     ExternalProject_add(boost
	     		  PREFIX ${boost_PREFIX}
				  URL http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.bz2
	     		  URL_MD5 d6eef4b4cacb2183f2bf265a5a03a354
	     		  CONFIGURE_COMMAND ${boost_CONFIGURE_COMMAND}
	              BUILD_COMMAND ${boost_BUILD_COMMAND}
	     		  INSTALL_DIR ${boost_INSTALL_DIR}
				  INSTALL_COMMAND ""
	     		  BUILD_IN_SOURCE 1
				  UPDATE_COMMAND ""
	     )
		 
	    set_property(TARGET boost PROPERTY FOLDER "Dependencies")
	    
	    set( Boost_DIR ${boost_INSTALL_DIR} CACHE PATH "Boost Root Directory")
	    set( Boost_LIBRARY_DIRS ${boost_INSTALL_DIR}/lib CACHE PATH "Boost Library directory" FORCE)
	    
	    if(MSVC)
	        set( Boost_INCLUDE_DIR ${boost_INSTALL_DIR}/include/boost-1_55 CACHE PATH "Boost Include directory" FORCE)
	        set( Boost_LIBRARIES optimized ${Boost_LIBRARY_DIRS}/libboost_chrono-vc100-mt-1_55.lib optimized ${Boost_LIBRARY_DIRS}/libboost_date_time-vc100-mt-1_55.lib optimized ${Boost_LIBRARY_DIRS}/libboost_system-vc100-mt-1_55.lib optimized ${Boost_LIBRARY_DIRS}/libboost_thread-vc100-mt-1_55.lib optimized ${Boost_LIBRARY_DIRS}/libboost_filesystem-vc100-mt-1_55.lib optimized ${Boost_LIBRARY_DIRS}/libboost_log_setup-vc100-mt-1_55.lib optimized ${Boost_LIBRARY_DIRS}/libboost_log-vc100-mt-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_chrono-vc100-mt-gd-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_date_time-vc100-mt-gd-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_system-vc100-mt-gd-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_thread-vc100-mt-gd-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_filesystem-vc100-mt-gd-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_log_setup-vc100-mt-gd-1_55.lib debug ${Boost_LIBRARY_DIRS}/libboost_log-vc100-mt-gd-1_55.lib CACHE INTERNAL "Boost libraries needed to compile MINVR apps")
	    else()
	        set( Boost_INCLUDE_DIR ${boost_INSTALL_DIR}/include CACHE PATH "Boost Include directory" FORCE)
	        set( Boost_LIBRARIES ${Boost_LIBRARY_DIRS}/libboost_chrono.a ${Boost_LIBRARY_DIRS}/libboost_date_time.a ${Boost_LIBRARY_DIRS}/libboost_system.a ${Boost_LIBRARY_DIRS}/libboost_thread.a ${Boost_LIBRARY_DIRS}/libboost_filesystem.a ${Boost_LIBRARY_DIRS}/libboost_log_setup.a ${Boost_LIBRARY_DIRS}/libboost_log.a CACHE INTERNAL "Boost libraries needed to compile MINVR apps")
	    endif()
	else ()
		message("   Set the BOOST_ROOT env var and re-configure, or set BUILD_BOOST to ON to download and build boost automatically.")
	endif()
	include_directories(${Boost_INCLUDE_DIR})
endif()

add_definitions(-DGLM_SWIZZLE)
include_directories(dependencies/glm)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Mac OS X specific code
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(MSVC)
	add_definitions(-DNOMINMAX)
endif()

add_subdirectory (MVRCore)

#Compile the user specified App Kits
if (USE_APPKIT_GLFW)
	add_subdirectory (AppKits/AppKit_GLFW)
endif()
if (USE_APPKIT_GLUT)
	add_subdirectory (AppKits/AppKit_Glut)
endif()
if (USE_APPKIT_G3D9)
	add_subdirectory (AppKits/AppKit_G3D9)
endif()

if (BUILD_EXAMPLES)
	if(USE_APPKIT_GLFW)
		add_subdirectory(AppKits/AppKit_GLFW/example)
	endif()
	if(USE_APPKIT_G3D9)
		add_subdirectory(AppKits/AppKit_G3D9/example)
	endif()
endif()

#Configure MinVRConfig.cmake
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
include(${CMAKE_SOURCE_DIR}/cmake/minvr_minvrconfig.cmake)

install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION "${MINVR_INSTALL_DIR}/lib")
install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/ DESTINATION "${MINVR_INSTALL_DIR}/bin")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/Modules/ DESTINATION "${MINVR_INSTALL_DIR}/cmake/modules")
