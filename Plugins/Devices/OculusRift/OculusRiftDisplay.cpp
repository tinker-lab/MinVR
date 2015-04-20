/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "OculusRiftDisplay.h"

namespace MinVR {



namespace ovr {

  // Convenience method for looping over each eye with a lambda
  template <typename Function>
  inline void for_each_eye(Function function) {
    for (ovrEyeType eye = ovrEyeType::ovrEye_Left;
      eye < ovrEyeType::ovrEye_Count;
      eye = static_cast<ovrEyeType>(eye + 1)) {
      function(eye);
    }
  }

  inline mat4 toGlm(const ovrMatrix4f & om) {
    return glm::transpose(glm::make_mat4(&om.M[0][0]));
  }

  inline mat4 toGlm(const ovrFovPort & fovport, float nearPlane = 0.01f, float farPlane = 10000.0f) {
    return toGlm(ovrMatrix4f_Projection(fovport, nearPlane, farPlane, true));
  }

  inline vec3 toGlm(const ovrVector3f & ov) {
    return glm::make_vec3(&ov.x);
  }

  inline vec2 toGlm(const ovrVector2f & ov) {
    return glm::make_vec2(&ov.x);
  }

  inline uvec2 toGlm(const ovrSizei & ov) {
    return uvec2(ov.w, ov.h);
  }

  inline quat toGlm(const ovrQuatf & oq) {
    return glm::make_quat(&oq.x);
  }

  inline mat4 toGlm(const ovrPosef & op) {
    mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
    mat4 translation = glm::translate(mat4(), ovr::toGlm(op.Position));
    return translation * orientation;
  }

  inline ovrMatrix4f fromGlm(const mat4 & m) {
    ovrMatrix4f result;
    mat4 transposed(glm::transpose(m));
    memcpy(result.M, &(transposed[0][0]), sizeof(float) * 16);
    return result;
  }

  inline ovrVector3f fromGlm(const vec3 & v) {
    ovrVector3f result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    return result;
  }

  inline ovrVector2f fromGlm(const vec2 & v) {
    ovrVector2f result;
    result.x = v.x;
    result.y = v.y;
    return result;
  }

  inline ovrSizei fromGlm(const uvec2 & v) {
    ovrSizei result;
    result.w = v.x;
    result.h = v.y;
    return result;
  }

  inline ovrQuatf fromGlm(const quat & q) {
    ovrQuatf result;
    result.x = q.x;
    result.y = q.y;
    result.z = q.z;
    result.w = q.w;
    return result;
  }
}

OculusRiftDisplay::OculusRiftDisplay() {

	if (!ovr_Initialize()) {
	    FAIL("Failed to initialize the Oculus SDK");
	}

	std::cout << "OculusRiftDisplay" << std::endl;
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

	ovr_Shutdown();
}

void OculusRiftDisplay::render(int threadId, WindowRef window, AbstractMVRAppRef app) {
	/*glDrawBuffer(GL_BACK);
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
	}*/

	GLenum err;

	++_frame;

    ovrHmd_GetEyePoses(_hmd, _frame, _eyeOffsets, _eyePoses, nullptr);

    ovrHmd_BeginFrame(_hmd, _frame);
    for (int i = 0; i < 2; ++i) {
      ovrEyeType eye = _currentEye = _hmd->EyeRenderOrder[i];
      const ovrRecti & vp = _eyeTextures[eye].Header.RenderViewport;

  	while((err = glGetError()) != GL_NO_ERROR) {
  		std::cout << "GLERRORh: "<<err<<std::endl;
  	}

      // Render the scene to an offscreen buffer
      _eyeFbos[eye].activate();
      for (int v=0; v < window->getNumViewports(); v++)
      {
      		MinVR::Rect2D viewport = window->getViewport(v);
      		glViewport(viewport.x0()+viewport.width()/2, viewport.y0(), viewport.width()/2, viewport.height());
      		if (eye == ovrEye_Left)
      		{
          		window->getCamera(v)->applyProjectionAndCameraMatricesForLeftEye();
      		}
      		else
      		{
          		window->getCamera(v)->applyProjectionAndCameraMatricesForRightEye();
      		}
      		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      		while((err = glGetError()) != GL_NO_ERROR) {
      			std::cout << "GLERRORssss: "<<err<<std::endl;
      		}

      		app->drawGraphics(threadId, window->getCamera(v), window);
      }
      //renderScene(projections[eye], ovr::toGlm(eyePoses[eye]));
    }


	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERRORg: "<<err<<std::endl;
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ovrHmd_EndFrame(_hmd, _eyePoses, _eyeTextures);


	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERRORf: "<<err<<std::endl;
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

	std::cout << "initializeContextSpecificVars" << std::endl;

    /*if (!ovrHmd_ConfigureTracking(_hmd,
      ovrTrackingCap_Orientation | ovrTrackingCap_Position | ovrTrackingCap_MagYawCorrection, 0)) {
      FAIL("Could not attach to sensor device");
    }

    memset(_eyeTextures, 0, 2 * sizeof(ovrGLTexture));

    ovr::for_each_eye([&](ovrEyeType eye){
      ovrSizei eyeTextureSize = ovrHmd_GetFovTextureSize(_hmd, eye, _hmd->MaxEyeFov[eye], 1.0f);
      ovrTextureHeader & eyeTextureHeader = _eyeTextures[eye].Header;
      eyeTextureHeader.TextureSize = eyeTextureSize;
      eyeTextureHeader.RenderViewport.Size = eyeTextureSize;
      eyeTextureHeader.API = ovrRenderAPI_OpenGL;
    });*/

    // Init GL
    ovrGLConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
    cfg.OGL.Header.RTSize = ovr::fromGlm(uvec2(500,500));
    cfg.OGL.Header.Multisample = 0;

    int distortionCaps = 0
    		| ovrDistortionCap_Vignette
    		| ovrDistortionCap_Chromatic
    		| ovrDistortionCap_Overdrive
    		| ovrDistortionCap_TimeWarp
    		;

    ON_LINUX([&]{
    	// This cap bit causes the SDK to properly handle the
    	// Rift in portrait mode.
    	distortionCaps |= ovrDistortionCap_LinuxDevFullscreen;

    	// On windows, the SDK does a good job of automatically
    	// finding the correct window.  On Linux, not so much.

    	NativeWindow nWindow = window->getNativeWindow();
    	cfg.OGL.Disp = nWindow.nativeDisplay;
    	cfg.OGL.Win = nWindow.nativeWindow;
    });

    if (!ovrHmd_ConfigureTracking(_hmd,
      ovrTrackingCap_Orientation | ovrTrackingCap_Position | ovrTrackingCap_MagYawCorrection, 0)) {
      FAIL("Could not attach to sensor device");
    }

    memset(_eyeTextures, 0, 2 * sizeof(ovrGLTexture));

    ovr::for_each_eye([&](ovrEyeType eye){
      ovrSizei eyeTextureSize = ovrHmd_GetFovTextureSize(_hmd, eye, _hmd->MaxEyeFov[eye], 1.0f);
      ovrTextureHeader & eyeTextureHeader = _eyeTextures[eye].Header;
      eyeTextureHeader.TextureSize = eyeTextureSize;
      eyeTextureHeader.RenderViewport.Size = eyeTextureSize;
      eyeTextureHeader.API = ovrRenderAPI_OpenGL;
    });

    int configResult = ovrHmd_ConfigureRendering(_hmd, &cfg.Config,
    		distortionCaps, _hmd->MaxEyeFov, _eyeRenderDescs);

    ovr::for_each_eye([&](ovrEyeType eye){
    	const ovrEyeRenderDesc & erd = _eyeRenderDescs[eye];
    	ovrMatrix4f ovrPerspectiveProjection = ovrMatrix4f_Projection(erd.Fov, OVR_DEFAULT_IPD * 4, 100000.0f, true);
    	_projections[eye] = ovr::toGlm(ovrPerspectiveProjection);
    	_eyeOffsets[eye] = erd.HmdToEyeViewOffset;

    	// Allocate the frameBuffer that will hold the scene, and then be
    	// re-rendered to the screen with distortion
    	auto & eyeTextureHeader = _eyeTextures[eye];
    	_eyeFbos[eye].init(ovr::toGlm(eyeTextureHeader.Header.TextureSize));
    	// Get the actual OpenGL texture ID
    	((ovrGLTexture&)eyeTextureHeader).OGL.TexId = _eyeFbos[eye].color;
    });

    ovrHmd_RecenterPose(_hmd);
}

} /* namespace MinVR */

extern "C"
{
	MinVR::StereoDisplayFactoryRef getStereoDisplayFactory() {
		return MinVR::StereoDisplayFactoryRef(new MinVR::OculusRiftDisplayFactory());
	}
}
