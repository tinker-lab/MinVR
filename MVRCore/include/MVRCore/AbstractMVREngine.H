/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/MVRCore/include/MVRCore/AbstractMVREngine.H

Original Author(s) of this File: 
	Bret Jackson, 2013, University of Minnesota (inspired by VRG3D/VRApp.H)
	
Author(s) of Significant Updates/Modifications to the File:
	...

-----------------------------------------------------------------------------------
Copyright (c) 2008-2015 Regents of the University of Minnesota and Brown University
Copyright (c) 2004-2008 Brown University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the University of Minnesota, Brown University, nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
================================================================================ */


#ifndef ABSTRACTMVRENGINE_H
#define ABSTRACTMVRENGINE_H

#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractWindow.H"
#include "MVRCore/ConfigMap.H"
#include "MVRCore/ConfigVal.H"
#include "MVRCore/WindowSettings.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/CameraOffAxis.H"
#include "framework/InputDevice.h"
#include "MVRCore/RenderThread.H"
#include "MVRCore/DataFileUtils.H"
#include "framework/plugin/PluginManager.h"
#include "framework/plugin/PluginInterface.h"
#include "MVRCore/Event.H"
#include <glm/glm.hpp>
#ifdef nil
#undef nil
#endif

#include <memory>
#include "MVRCore/Thread.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "log/Logger.h"

namespace MinVR {

/*! @brief Base class for vr engines
 *
 *  The MVREngine handles creating the windows, threads, devices, and mainloop.
 */
class AbstractMVREngine : public MinVR::framework::plugin::PluginInterface {
public:

	AbstractMVREngine();
	virtual ~AbstractMVREngine();

	/*! @brief Initialize the engine.
	 * 
	 *  Reads config files and parameters based on 'standard' command line arguments then
	 *  initializes graphics windows etc. based on this info.
	 */
	virtual void init(int argc, char **argv);

	/*! @brief Initialize the engine.
	 * 
	 *  Use this version of init instead and pass it config settings in the configMap argument
	 *  if you want to handle your own command line arguments
	 *
	 *	@param[in] A map of configuration parameters.
	 *  @param[in] Log file.
	 */
	virtual void init(ConfigMapRef configMap);

	/*! @brief Start the main loop.
	 *
	 *  This version of run takes over the main loop of the program, call it after init when you are
	 *  ready to start processing user input and rendering graphics
	 *
	 *  @param[in] A VR application reference.
	 */
	virtual void runApp(AbstractMVRAppRef app);

	/*! @brief Draws a frame of the graphics.
	 *
	 *  Use this version instead and call it repeatedly from your own mainloop if your program design
	 *  requires you to manage your own mainloop
	 *
	 *  @param[in] A VR application reference.
	 */
	virtual void runOneFrameOfApp(AbstractMVRAppRef app);

	/*! @brief Initialize thread specific variables
	 *
	 * Called from each renderthread to initialize needed variables
	 *
	 * @param[in] A thread specific id
	 * @param[in] Window reference for the calling renderthread.
	 */
	virtual void initializeContextSpecificVars(int threadId, WindowRef window);

	/*! @brief Initialize Log
	 *
	 *  Sets up the logging
	 */
	virtual void initializeLogging();

	/*! @brief Add Input Device Drivers
		 *
		 *  Allows the ability to add input devices.
	*/
	void addInputDeviceDriver(MinVR::framework::InputDeviceDriverRef driver);

protected:

	/*! @brief Creates windows and viewports
	 *
	 *  Called from init to create windows and viewports based on the vrsetup file
	 *  
	 *  @sa init
	 */
	virtual void setupPlugins();

	/*! @brief Creates windows and viewports
	 *
	 *  Called from init to create windows and viewports based on the vrsetup file
	 *
	 *  @sa init
	 */
	virtual void setupWindowsAndViewports();


	/*! @brief Creates a window
	 *
	 *  Called from setupWindowsAndViewports to create a window
	 *
	 *  @param[in] Window settings specifying the dimensions, position, and properties of the window
	 *  @param[in] Array of cameras corresponding with viewports for the new window.
	 *
	 *  @sa setupWindowsAndViewports
	 */
	virtual WindowRef createWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras) = 0;

	/*! @brief Creates Input Devices
	 *
	 *  Called from init to create input devices based on the vrsetup file
	 *  
	 *  @sa init
	 */
	virtual void setupInputDevices();

	/*! @brief Creates render threads.
	 *
	 *  Creates a new thread for each window specified in the vrsetup file. The threads are used
	 *  for multi-threaded rendering.
	 */
	virtual void setupRenderThreads();

	/*! @brief Poll the input devices for input.
	 *
	 *  Iterates through the input devices and windows polling each for input.
	 *  the input is sorted based on increasing time.
	 */
	virtual void pollUserInput();

	/*! @brief Updates head positions.
	 *
	 *  Updates each camera in every window with the new head location.
	 */
	virtual void updateProjectionForHeadTracking();

	AbstractMVRAppRef         _app;
	ConfigMapRef      _configMap;
	std::vector<EventRef> _events;
	std::vector<WindowRef>  _windows;
	std::vector<MinVR::framework::InputDeviceRef> _inputDevices;
	std::vector<MinVR::framework::InputDeviceDriverRef> _inputDeviceDrivers;
	std::vector<RenderThreadRef> _renderThreads;
	Mutex _threadsInitializedMutex;
	ConditionVariable _threadsInitializedCond;
	Mutex _startRenderingMutex;
	ConditionVariable _startRenderingCond;
	Mutex _renderingCompleteMutex;
	ConditionVariable _renderingCompleteCond;
	std::shared_ptr<Barrier> _swapBarrier;
	TimeStamp _syncTimeStart;
	unsigned long _frameCount;
	MinVR::framework::plugin::PluginManager _pluginManager;
};

} // end namespace

#endif


