/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "OculusRiftDisplay.h"

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "GL/glext.h"

#include <string.h>

using namespace glm;

namespace MinVR {

OculusRiftDisplay::OculusRiftDisplay() {
	_hmd = ovrHmd_Create(0);
	if (nullptr == _hmd) {
		ovrHmdType defaultHmdType = ovrHmd_DK2;
		_hmd = ovrHmd_CreateDebug(defaultHmdType);
		_hmdDesktopPosition = ivec2(100, 100);
		_hmdNativeResolution = uvec2(1200, 800);
	} else {
		_hmdDesktopPosition = ivec2(_hmd->WindowsPos.x, _hmd->WindowsPos.y);
		_hmdNativeResolution = ivec2(_hmd->Resolution.w, _hmd->Resolution.h);
	}
}

OculusRiftDisplay::~OculusRiftDisplay() {
	ovrHmd_Destroy(_hmd);
	_hmd = nullptr;
}

void OculusRiftDisplay::render(int threadId, WindowRef window, AbstractMVRAppRef app) {
	glDrawBuffer(GL_BACK);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Left Eye
	for (int v=0; v < window->getNumViewports(); v++) {
		MinVR::Rect2D viewport = window->getViewport(v);
		glViewport(viewport.x0(), viewport.y0(), viewport.width()/2, viewport.height());
		window->getCamera(v)->applyProjectionAndCameraMatricesForLeftEye();
		app->drawGraphics(threadId, window->getCamera(v), window);
	}
	// Right Eye
	for (int v=0; v < window->getNumViewports(); v++) {
		MinVR::Rect2D viewport = window->getViewport(v);
		glViewport(viewport.x0()+viewport.width()/2, viewport.y0(), viewport.width()/2, viewport.height());
		window->getCamera(v)->applyProjectionAndCameraMatricesForRightEye();
		app->drawGraphics(threadId, window->getCamera(v), window);
	}
}

MinVR::OculusRiftDisplayFactory::OculusRiftDisplayFactory() {
}

MinVR::OculusRiftDisplayFactory::~OculusRiftDisplayFactory() {
}

StereoDisplayRef MinVR::OculusRiftDisplayFactory::create(
		const std::string& type) {
	if (type == "OculusRift")
	{
		return StereoDisplayRef(new OculusRiftDisplay());
	}

	return NULL;
}

void OculusRiftDisplay::initializeContextSpecificVars(int threadId,
		WindowRef window) {
    ovrGLConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
    ovrSizei size;
    size.w = _hmd->Resolution.w;
    size.h = _hmd->Resolution.h;
    cfg.OGL.Header.RTSize = size;
    cfg.OGL.Header.Multisample = 0;

    int distortionCaps = 0
      | ovrDistortionCap_Vignette
      | ovrDistortionCap_Chromatic
      | ovrDistortionCap_Overdrive
      | ovrDistortionCap_TimeWarp
      ;

    /*ON_LINUX([&]{
      // This cap bit causes the SDK to properly handle the
      // Rift in portrait mode.
      distortionCaps |= ovrDistortionCap_LinuxDevFullscreen;

      // On windows, the SDK does a good job of automatically
      // finding the correct window.  On Linux, not so much.
      cfg.OGL.Disp = glfwGetX11Display();
      cfg.OGL.Win = glfwGetX11Window(window);
    });*/

    int configResult = ovrHmd_ConfigureRendering(_hmd, &cfg.Config,
      distortionCaps, _hmd->MaxEyeFov, _eyeRenderDescs);

    /*ovrGLTexture eyeTexture[2];
    for (int i = 0; i < ovrEye_Count; i++)
    {
    	eyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
    	eyeTexture[0].OGL.Header.TextureSize = RenderTargetSize;
    	eyeTexture[0].OGL.Header.RenderViewport = eyes[0].RenderViewport;
        eyeTexture[0].OGL.TexId = textureId;
    }

    ovr::for_each_eye([&](ovrEyeType eye){
      const ovrEyeRenderDesc & erd = eyeRenderDescs[eye];
      ovrMatrix4f ovrPerspectiveProjection = ovrMatrix4f_Projection(erd.Fov, OVR_DEFAULT_IPD * 4, 100000.0f, true);
      projections[eye] = ovr::toGlm(ovrPerspectiveProjection);
      eyeOffsets[eye] = erd.HmdToEyeViewOffset;

      // Allocate the frameBuffer that will hold the scene, and then be
      // re-rendered to the screen with distortion
      auto & eyeTextureHeader = eyeTextures[eye];
      eyeFbos[eye].init(ovr::toGlm(eyeTextureHeader.Header.TextureSize));
      // Get the actual OpenGL texture ID
      ((ovrGLTexture&)eyeTextureHeader).OGL.TexId = eyeFbos[eye].color;
    });*/
}

} /* namespace MinVR */

extern "C"
{
	MinVR::StereoDisplayFactoryRef getStereoDisplayFactory() {
		return MinVR::StereoDisplayFactoryRef(new MinVR::OculusRiftDisplayFactory());
	}
}
