cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)


project (MVRCore)


set (SOURCEFILES
source/AbstractMVREngine.cpp
source/AbstractWindow.cpp
source/CameraOffAxis.cpp
source/ConfigMap.cpp
source/ConfigVal.cpp
source/DataFileUtils.cpp
source/Event.cpp
source/RenderThread.cpp
source/StringUtils.cpp
source/Rect2D.cpp
source/io/FileSystem.cpp
source/log/Logger.cpp
source/io/BasicFileSystem.cpp
source/log/BasicLogger.cpp
source/log/ThreadSafeLogger.cpp
source/log/CompositeLogger.cpp
source/framework/plugin/PluginManager.cpp
source/framework/plugin/SharedLibrary.cpp
)

set (HEADERFILES
include/MVRCore/AbstractCamera.H
include/MVRCore/AbstractMVRApp.H
include/MVRCore/AbstractMVREngine.H
include/MVRCore/AbstractWindow.H
include/MVRCore/CameraOffAxis.H
include/MVRCore/CameraTraditional.H
include/MVRCore/ConfigMap.H
include/MVRCore/ConfigVal.H
include/MVRCore/DataFileUtils.H
include/MVRCore/Event.H
include/MVRCore/RenderThread.H
include/MVRCore/StringUtils.H
include/MVRCore/WindowSettings.H
include/MVRCore/Rect2D.H
include/io/FileSystem.h
include/log/Logger.h
include/MVRCore/Time.h
include/MVRCore/Thread.h
include/io/BasicFileSystem.h
include/log/BasicLogger.h
include/log/ThreadSafeLogger.h
include/log/CompositeLogger.h
include/framework/plugin/PluginFramework.h
include/framework/plugin/Plugin.h
include/framework/plugin/PluginInterface.h
include/framework/plugin/PluginManager.h
include/framework/plugin/SharedLibrary.h
include/framework/InputDevice.h
)

if (USE_BOOST)
	set (SOURCEFILES ${SOURCEFILES}
	source/Boost/BoostLogger.cpp
	source/io/BasicFileSystem.cpp
	source/Boost/BoostFileSystem.cpp
	)

	set (HEADERFILES ${HEADERFILES}
	include/Boost/BoostFileSystem.h
	include/Boost/BoostLogger.h
	include/Boost/BoostThread.h
	include/Boost/BoostTime.h
	)
endif()

source_group("Header Files" FILES ${HEADERFILES})


# Include Directories

include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/dependencies/glm
)

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

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMinVR_DEBUG")

# Build Target

add_library ( ${PROJECT_NAME} ${HEADERFILES} ${SOURCEFILES} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBS_ALL} )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION "${MINVR_INSTALL_DIR}/include")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/glm/glm/ DESTINATION "${MINVR_INSTALL_DIR}/include/glm")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/shaders/ DESTINATION "${MINVR_INSTALL_DIR}/share/shaders")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/vrsetup/ DESTINATION "${MINVR_INSTALL_DIR}/share/vrsetup")

add_dependencies(${PROJECT_NAME} boost)

