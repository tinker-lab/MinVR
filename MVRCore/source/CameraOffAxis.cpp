/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/MVRCore/source/CameraOffAxis.cpp

Original Author(s) of this File: 
	Daniel Keefe, 2004, Brown University (originally VRG3D/ProjectionVRCamera.cpp)
	
Author(s) of Significant Updates/Modifications to the File:
	Bret Jackson, 2013, University of Minnesota (adapted to MinVR and glm)

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


#include "MVRCore/CameraOffAxis.H"
#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace MinVR
{

CameraOffAxis::CameraOffAxis(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 botLeft, glm::dvec3 botRight,
	glm::dmat4 initialHeadFrame, double interOcularDistance, 
	double nearClipDist, double farClipDist) : AbstractCamera()
{
	_topLeft = topLeft;
	_topRight = topRight;
	_botLeft = botLeft;
	_botRight = botRight;
	_headFrame = initialHeadFrame;
	_iod = interOcularDistance;
	_nearClip = nearClipDist;
	_farClip = farClipDist;
	_halfWidth = glm::length(_topRight - _topLeft) / 2.0;
	_halfHeight = glm::length(_topRight - _botRight) / 2.0;

	glm::dvec3 center = (topLeft + topRight + botLeft + botRight);
	center.x = center.x / 4.0;
	center.y = center.y / 4.0;
	center.z = center.z / 4.0;
	glm::dvec3 x = glm::normalize(topRight - topLeft);
	glm::dvec3 y = glm::normalize(topLeft - botLeft);
	glm::dvec3 z = glm::normalize(glm::cross(x, y));
	glm::dmat4 tile2room(x.x, x.y, x.z, 0,
						y.x, y.y, y.z, 0,
						z.x, z.y, z.z, 0,
						center.x, center.y, center.z, 1);
	//glm::mat4 tile2room(x.x, y.x, z.x, center.x,
	//					x.y, y.y, z.y, center.y,
	//					x.z, y.z, z.z, center.z,
	//					0.0, 0.0, 0.0, 1);
	_room2tile = glm::inverse(tile2room);
}

CameraOffAxis::~CameraOffAxis()
{
}

void CameraOffAxis::updateHeadTrackingFrame(glm::dmat4 newHeadFrame)
{
	_headFrame = newHeadFrame;

	// 1. Get the center of the camera (the eye) position from the head position
	glm::dmat4 head2Room = _headFrame;
	glm::dmat4 leftEye2Room = getLeftEyeFrame();
	glm::dmat4 rightEye2Room = getRightEyeFrame();
  
	// 2. Setup projection matrix
	glm::dvec3 head = glm::column((_room2tile * head2Room), 3).xyz();
	glm::dvec3 left = glm::column((_room2tile * leftEye2Room), 3).xyz();
	glm::dvec3 right = glm::column((_room2tile * rightEye2Room), 3).xyz();
	

	double lHead = (-_halfWidth - head.x);
	double rHead = (_halfWidth - head.x);
	double lLeft = (-_halfWidth - left.x);
	double rLeft = (_halfWidth - left.x);
	double bLeft =  (-_halfHeight - left.y);
	double tLeft = (_halfHeight - left.y);
	double kLeft = _nearClip / left.z;
	double lRight = (-_halfWidth - right.x);
	double rRight = (_halfWidth - right.x);
	double bRight =  (-_halfHeight - right.y);
	double tRight = (_halfHeight - right.y);
	double kRight = _nearClip / right.z;

	double b = (-_halfHeight - head.y);
	double t = (_halfHeight - head.y);
	double dist = head.z;
	double k = _nearClip / dist;

	// 3. Add eye position to the projection (eye is in tile coordinates)
	glm::dmat4 r2t = glm::column(glm::dmat4(1.0), 3, glm::dvec4(-head, 1.0)) * _room2tile;
	glm::dmat4 r2tLeft = glm::column(glm::dmat4(1.0), 3, glm::dvec4(-left, 1.0)) * _room2tile;
	glm::dmat4 r2tRight = glm::column(glm::dmat4(1.0), 3, glm::dvec4(-right, 1.0)) * _room2tile;

	_projection = invertYMat() * perspectiveProjection(lHead*k, rHead*k, b*k, t*k, _nearClip, _farClip);
	_projectionLeft = invertYMat() * perspectiveProjection(lLeft*kLeft, rLeft*kLeft, bLeft*kLeft, tLeft*kLeft, _nearClip, _farClip);
	_projectionRight = invertYMat() * perspectiveProjection(lRight*kRight, rRight*kRight, bRight*kRight, tRight*kRight, _nearClip, _farClip);

	_view = r2t;//.inverse();
	_viewLeft = r2tLeft;//.inverse();
	_viewRight = r2tRight;//.inverse();
}

glm::dmat4 CameraOffAxis::invertYMat()
{
	static glm::dmat4 M(1,  0, 0, 0,
					  0, -1, 0, 0,
					  0,  0, 1, 0,
					  0,  0, 0, 1);
	return M;
}

glm::dmat4 CameraOffAxis::perspectiveProjection(double left, double right, double bottom, double top, double nearval, double farval, float upDirection)
{
    double x, y, a, b, c, d;

    x = (2.0*nearval) / (right-left);
    y = (2.0*nearval) / (top-bottom);
    a = (right+left) / (right-left);
    b = (top+bottom) / (top-bottom);

    if (farval >= std::numeric_limits<double>::max()) {
       // Infinite view frustum
       c = -1.0;
       d = -2.0 * nearval;
    } else {
       c = -(farval+nearval) / (farval-nearval);
       d = -(2.0*farval*nearval) / (farval-nearval);
    }

    MinVR::Logger::getInstance().assertMessage(abs(upDirection) == 1.0, "upDirection must be -1 or +1");
    y *= upDirection;
    b *= upDirection;

    return glm::dmat4(x, 0, 0, 0,
					 0, y, 0, 0,
					 a, b, c, -1,
					 0, 0, d, 0);
}

glm::dmat4 CameraOffAxis::getLeftEyeFrame()
{
	return _headFrame * glm::column(glm::dmat4(1.0), 3, glm::dvec4(-_iod/2.0, 0.0, 0.0, 1.0));
}

glm::dmat4 CameraOffAxis::getRightEyeFrame()
{
	return _headFrame * glm::column(glm::dmat4(1.0), 3, glm::dvec4(_iod/2.0, 0.0, 0.0, 1.0));
}

void CameraOffAxis::applyProjectionAndCameraMatrices()
{
	applyProjectionAndCameraMatrices(_projection, _view);
}

void CameraOffAxis::applyProjectionAndCameraMatricesForLeftEye()
{
	applyProjectionAndCameraMatrices(_projectionLeft, _viewLeft);
}

void CameraOffAxis::applyProjectionAndCameraMatricesForRightEye()
{
	applyProjectionAndCameraMatrices(_projectionRight, _viewRight);
}

void CameraOffAxis::setObjectToWorldMatrix(glm::dmat4 obj2World)
{
	_object2World = obj2World;
	glMatrixMode(GL_MODELVIEW);
	glm::dmat4 modelView = _currentViewMatrix * _object2World;
	GLfloat matrix[16];
    for (int c = 0; c < 4; ++c) {
		for(int r = 0; r < 4; ++r) {
			matrix[c*4+r] = (float)modelView[c][r];
		}
    }
	glLoadMatrixf(matrix);
}

glm::dmat4 CameraOffAxis::getObjectToWorldMatrix()
{
	return _object2World;
}


void CameraOffAxis::applyProjectionAndCameraMatrices(const glm::dmat4& projectionMat, const glm::dmat4& viewMat)
{
	_currentViewMatrix = viewMat;
	_currentProjMatrix = projectionMat;
	glMatrixMode(GL_PROJECTION);
    GLfloat matrix[16];
    for (int c = 0; c < 4; ++c) {
		for(int r = 0; r < 4; ++r) {
			matrix[c*4+r] = (float)projectionMat[c][r];
		}
    }
	glLoadMatrixf(matrix);

	glMatrixMode(GL_MODELVIEW);
	glm::dmat4 modelView = viewMat * _object2World;
	for (int c = 0; c < 4; ++c) {
		for(int r = 0; r < 4; ++r) {
			matrix[c*4+r] = (float)modelView[c][r];
		}
    }
	glLoadMatrixf(matrix);
}

glm::dmat4 CameraOffAxis::getLastAppliedProjectionMatrix()
{
	return _currentProjMatrix;
}

glm::dmat4 CameraOffAxis::getLastAppliedModelMatrix()
{
	return _object2World;
}

glm::dmat4 CameraOffAxis::getLastAppliedViewMatrix()
{
	return _currentViewMatrix;
}

glm::dvec3 CameraOffAxis::getLookVector()
{
	glm::dvec3 right = _topRight-_topLeft;
	right *= 0.5;
	glm::dvec3 down = _botLeft-_topLeft;
	down *= 0.5;
	glm::dvec3 filmPlaneCtr = _topLeft + right + down;
	glm::dvec3 headPos = glm::column(_headFrame, 3).xyz;
	return glm::normalize(filmPlaneCtr - headPos);
}

glm::dvec3 CameraOffAxis::getTopLeft() const {
	return _topLeft;
}

glm::dvec3 CameraOffAxis::getTopRight() const {
	return _topRight;
}

glm::dvec3 CameraOffAxis::getBottomLeft() const {
	return _botLeft;
}

glm::dvec3 CameraOffAxis::getBottomRight() const {
	return _botRight;
}

} // End namespace

