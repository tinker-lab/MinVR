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

#include <iostream>
#include <memory>
#include <exception>

//////////////////////////////////////////////////////////////////////
//
// The OVR types header contains the OS detection macros:
//  OVR_OS_WIN32, OVR_OS_MAC, OVR_OS_LINUX (and others)
//

#if (defined(WIN64) || defined(WIN32))
#define OS_WIN
#elif (defined(__APPLE__))
#define OS_OSX
#else
#define OS_LINUX
#endif

#if defined(OS_WIN)
#define ON_WINDOWS(runnable) runnable()
#define ON_MAC(runnable)
#define ON_LINUX(runnable)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef NOMINMAX
#elif defined(OS_OSX)
#define ON_WINDOWS(runnable)
#define ON_MAC(runnable) runnable()
#define ON_LINUX(runnable)
#elif defined(OS_LINUX)
#define ON_WINDOWS(runnable)
#define ON_MAC(runnable)
#define ON_LINUX(runnable) runnable()
#endif


#define __STDC_FORMAT_MACROS 1
#define GLM_FORCE_RADIANS

#define FAIL(X) throw std::runtime_error(X)

#ifdef OVR_OS_LINUX
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL
//

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;


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

#include <OVR_CAPI_GL.h>

namespace MinVR {

struct FramebufferWraper {
  glm::uvec2 size;
  GLuint fbo{0};
  GLuint color{0};
  GLuint depth{ 0 };

  virtual ~FramebufferWraper() {
    release();
  }

  void allocate() {
    release();
    glGenRenderbuffers(1, &depth);
    assert(depth);
    glGenTextures(1, &color);
    assert(color);
    glGenFramebuffers(1, &fbo);
    assert(fbo);
  }

  void release() {
    if (fbo) {
      glDeleteFramebuffers(1, &fbo);
      fbo = 0;
    }
    if (color) {
      glDeleteTextures(1, &color);
      color = 0;
    }
    if (depth) {
      glDeleteRenderbuffers(1, &depth);
      depth = 0;
    }
  }

  static bool checkStatus(GLenum target = GL_FRAMEBUFFER) {
    GLuint status = glCheckFramebufferStatus(target);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      return true;
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      std::cerr << "framebuffer incomplete attachment" << std::endl;
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      std::cerr << "framebuffer missing attachment" << std::endl;
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      std::cerr << "framebuffer incomplete draw buffer" << std::endl;
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      std::cerr << "framebuffer incomplete read buffer" << std::endl;
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      std::cerr << "framebuffer incomplete multisample" << std::endl;
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      std::cerr << "framebuffer incomplete layer targets" << std::endl;
      break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
      std::cerr << "framebuffer unsupported internal format or image" << std::endl;
      break;

    default:
      std::cerr << "other framebuffer error" << std::endl;
      break;
    }

    return false;
  }

  void init(const glm::uvec2 & size) {
    this->size = size;
    allocate();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glBindTexture(GL_TEXTURE_2D, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, size.x, size.y);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.x, size.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    if (!checkStatus()) {
      FAIL("Could not create a valid framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void activate() {
    assert(fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, size.x, size.y);
  }
};

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
	ovrTexture _eyeTextures[2];
	ovrVector3f _eyeOffsets[2];
	ovrEyeType _currentEye{ ovrEye_Count };
	ovrEyeRenderDesc _eyeRenderDescs[2];
	glm::mat4 _projections[2];
	ovrPosef _eyePoses[2];
	FramebufferWraper _eyeFbos[2];
	unsigned int _frame{ 0 };
};

class OculusRiftDisplayFactory : public StereoDisplayFactory {
public:
	OculusRiftDisplayFactory();
	virtual ~OculusRiftDisplayFactory();

	StereoDisplayRef create(const std::string& type);
};

} /* namespace MinVR */

#endif /* OCULUSRIFTDISPLAY_H_ */
