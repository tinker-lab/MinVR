/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/AppKits/AppKit_GLFW/example/include/GLFWDemoApp.H 

Original Author(s) of this File: 
	Bret Jackson, 2013, University of Minnesota
	
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

#ifndef GLFWDEMOAPP_H
#define GLFWDEMOAPP_H

#include "GL/glew.h"
#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/AbstractWindow.H"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MVRCore/Event.H"
#include <GLFW/glfw3.h>
#include <vector>
#include <map>

class GLFWDemoApp : public MinVR::AbstractMVRApp
{
public:
	GLFWDemoApp();
	~GLFWDemoApp();

	void doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime);
	void initializeContextSpecificVars(int threadId, MinVR::WindowRef window);
	void postInitialization();
	void drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window);

private:
	void initGL();
	void initVBO(int threadId);
	void initLights();

	std::map<int, GLuint> _vboId;
};

#endif
