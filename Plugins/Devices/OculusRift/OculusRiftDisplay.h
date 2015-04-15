/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef OCULUSRIFTDISPLAY_H_
#define OCULUSRIFTDISPLAY_H_

#include "MVRCore/Display/StereoDisplayFactory.h"

#include <glm/glm.hpp>
// The oculus SDK requires it's own macros to distinguish between
// operating systems, which is required to setup the platform specific
// values, like window handles
#if defined(OS_WIN)
#define OVR_OS_WIN32
#elif defined(OS_OSX)
#define OVR_OS_MAC
#elif defined(OS_LINUX)
#define OVR_OS_LIUNX
#endif

#include <OVR_CAPI_GL.h>

namespace MinVR {

class OculusRiftDisplay : public StereoDisplay {
public:
	OculusRiftDisplay();
	virtual ~OculusRiftDisplay();

	void render(int threadId, WindowRef window, AbstractMVRAppRef app);
	void initializeContextSpecificVars(int threadId, WindowRef window);
private:
	ovrHmd _hmd;
	glm::uvec2 _hmdNativeResolution;
	glm::ivec2 _hmdDesktopPosition;
	ovrEyeRenderDesc _eyeRenderDescs[2];
};

class OculusRiftDisplayFactory : public StereoDisplayFactory {
public:
	OculusRiftDisplayFactory();
	virtual ~OculusRiftDisplayFactory();

	StereoDisplayRef create(const std::string& type);
};

} /* namespace MinVR */

#endif /* OCULUSRIFTDISPLAY_H_ */
