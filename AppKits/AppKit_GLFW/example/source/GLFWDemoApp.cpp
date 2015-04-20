/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/AppKits/AppKit_GLFW/example/source/GLFWDemoApp.cpp

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


#include "GLFWDemoApp.H"

#include "MVRCore/StringUtils.H"
#include "MVRCore/CameraOffAxis.H"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace MinVR;

GLFWDemoApp::GLFWDemoApp() : MinVR::AbstractMVRApp()
{
}

GLFWDemoApp::~GLFWDemoApp()
{
	for(std::map<int, GLuint>::iterator iterator = _vboId.begin(); iterator != _vboId.end(); iterator++) {
		glDeleteBuffers(1, &iterator->second);
	}
}

void GLFWDemoApp::doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime)
{
	for(int i=0; i < events.size(); i++) {
		std::cout << events[i]->getName() <<std::endl;
	}
}

void GLFWDemoApp::initializeContextSpecificVars(int threadId, WindowRef window)
{
	initGL();
	initVBO(threadId);
	initLights();

	glClearColor(0.f, 0.3f, 1.f, 1.f);
	//glClearColor(1.f, 0.f, 0.f, 1.f);

	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "openGL ERROR in initializeContextSpecificVars: "<<err<<std::endl;
	}
}

namespace Attribute {
  enum {
    Position = 0,
    TexCoord0 = 1,
    Normal = 2,
    Color = 3,
    TexCoord1 = 4,
    InstanceTransform = 5,
  };
}

static const char * VERTEX_SHADER =
"#version 330\n"

"uniform mat4 Projection;"
"uniform mat4 View;"
"uniform mat4 Model;"
"uniform vec3 cameraPos;"

"layout(location = 0) in vec4 Position;"
"layout(location = 2) in vec3 Normal;"

"out vec3 vertNormal;"

"void main(void)"
"{"
" vertNormal = Normal;"
" gl_Position = Projection * View * Model * Position;"
"}";

static const char * FRAGMENT_SHADER =
"#version 330\n"
"in vec3 vertNormal;"

"out vec4 fragColor;"

"void main(void)"
"{"
" fragColor = vec4(1.0, 0.0, 0.0, 1.0);"
"}";

// helper to check and display for shader compiler errors
bool check_shader_compile_status(GLuint obj) {
	GLint status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		GLint length;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetShaderInfoLog(obj, length, &length, &log[0]);
		std::cerr << &log[0];
		return false;
	}
	return true;
}
// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
	GLint status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		GLint length;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetProgramInfoLog(obj, length, &length, &log[0]);
		std::cerr << &log[0];
		return false;
	}
	return true;
}

void compileShader(const GLuint &shader, const std::string& code)
{
	const char *source;
	int length;

	source = code.c_str();
	length = code.size();
	glShaderSource(shader, 1, &source, &length);
	glCompileShader(shader);
	check_shader_compile_status(shader);
}

std::string loadFile(const std::string &shader)
{
	return shader;
}

void GLFWDemoApp::initVBO(int threadId)
{
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	// vertex coords array for glDrawArrays() =====================================
	// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
	// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
	// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
	// array is 108 floats (36 * 3 = 108).
	GLfloat vertices[]  = { 1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,      // v0-v1-v2 (front)
						   -1.0f,-1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 1.0f,      // v2-v3-v0

							1.0f, 1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f,-1.0f,-1.0f,      // v0-v3-v4 (right)
							1.0f,-1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 1.0f,      // v4-v5-v0

							1.0f, 1.0f, 1.0f,   1.0f, 1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,      // v0-v5-v6 (top)
						   -1.0f, 1.0f,-1.0f,  -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,      // v6-v1-v0

						   -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,      // v1-v6-v7 (left)
						   -1.0f,-1.0f,-1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,      // v7-v2-v1.0

						   -1.0f,-1.0f,-1.0f,   1.0f,-1.0f,-1.0f,   1.0f,-1.0f, 1.0f,      // v7-v4-v3 (bottom)
							1.0f,-1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f,-1.0f,-1.0f,      // v3-v2-v7

							1.0f,-1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,      // v4-v7-v6 (back)
						   -1.0f, 1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   1.0f,-1.0f,-1.0f };    // v6-v5-v4

	// normal array
	GLfloat normals[]   = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
							0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

							1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
							1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

							0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
							0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

						   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
						   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

							0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
							0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

							0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
							0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4

	// color array
	GLfloat colors[]    = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
							1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

							1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
							0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

							1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
							0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

							1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
							0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

							0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
							1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

							0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
							0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4



	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR initVBO: "<<err<<std::endl;
	}
    // create vertex buffer objects, you need to delete them when program exits
    // Try to put both vertex coords array, vertex normal array and vertex color in the same buffer object.
    // glBufferDataARB with NULL pointer reserves only memory space.
    // Copy actual data with 2 calls of glBufferSubDataARB, one for vertex coords and one for normals.
    // target flag is GL_ARRAY_BUFFER_ARB, and usage flag is GL_STATIC_DRAW_ARB
	_vboId[threadId] = GLuint(0);
	glGenBuffers(1, &_vboId[threadId]);
    glBindBuffer(GL_ARRAY_BUFFER, _vboId[threadId]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals)+sizeof(colors), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals), sizeof(colors), colors);  // copy colours after normals
	
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR initVBO2: "<<err<<std::endl;
	}

	std::cout << _vertexShader << "hi" << std::endl;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, loadFile(VERTEX_SHADER));

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, loadFile(FRAGMENT_SHADER));

	// create program
	_shaderProgram = glCreateProgram();
	// attach shaders
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	// link the program and check for errors
	glLinkProgram(_shaderProgram);
	check_program_link_status(_shaderProgram);

	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR initVBOaaaaaaaaa: "<<err<<std::endl;
	}

	//exit(0);
 }
 
void GLFWDemoApp::initGL()
{
	glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);

     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);
	
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR initGL: "<<err<<std::endl;
	}
}

void GLFWDemoApp::initLights()
{
	// set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0.5, 0, 3, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration

	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR initLights: "<<err<<std::endl;
	}
}

void GLFWDemoApp::postInitialization()
{
}

void GLFWDemoApp::drawGraphics(int threadId, AbstractCameraRef camera, WindowRef window)
{
	GLenum err;

	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERRORe: "<<err<<std::endl;
	}

	glUseProgram(_shaderProgram);

	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERRORd: "<<err<<std::endl;
	}

	 // obtain location of projection uniform
	GLint Model_location = glGetUniformLocation(_shaderProgram, "Model");
	GLint View_location = glGetUniformLocation(_shaderProgram, "View");
	GLint Projection_location = glGetUniformLocation(_shaderProgram, "Projection");
	GLint cameraPos_location = glGetUniformLocation(_shaderProgram, "cameraPos");



	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERRORc: "<<err<<std::endl;
	}

	 // calculate ViewProjection matrix
	glm::mat4 Projection = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.f);
	MinVR::CameraOffAxis* offAxisCamera = dynamic_cast<MinVR::CameraOffAxis*>(camera.get());
	Projection = offAxisCamera->getLastAppliedProjectionMatrix();
	// translate the world/view position
	//glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -20.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -2.0f));
	//glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
	if (_xRotationAngle != 0.0f)
	{
		model = glm::rotate(model, _xRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERRORb: "<<err<<std::endl;
	}

	model = glm::rotate(model, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));


	//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));

	// dimensions
	// 1,343.4 mi
	// 1,234.6 mi
	// 12.32 mi
	model = glm::scale(model, glm::vec3(1.0f, -1.0f/100.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5, 0.5f));
	// make the camera rotate around the origin
	//View = glm::rotate(View, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//View = glm::rotate(View, -22.5f*_time, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 View = glm::mat4(offAxisCamera->getLastAppliedViewMatrix());
	// set the uniform

	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR1: "<<err<<std::endl;
	}

	glUniformMatrix4fv(Model_location, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(View_location, 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(Projection));

	//glClearColor(1.f, 0.f, 0.f, 1.f);

	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR1: "<<err<<std::endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboId[threadId]);

    // enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // before draw, specify vertex and index arrays with their offsets
    glNormalPointer(GL_FLOAT, 0, (void*)(sizeof(GLfloat)*108));
    glColorPointer(3, GL_FLOAT, 0, (void*)((sizeof(GLfloat)*108)+(sizeof(GLfloat)*108)));
    glVertexPointer(3, GL_FLOAT, 0, 0);

	glm::dmat4 translate = glm::translate(glm::dmat4(1.0f), glm::dvec3(0.0f, 0.0f, -5.0f));
	glm::dvec2 rotAngles(-20.0, 45.0);
	glm::dmat4 rotate1 = glm::rotate(translate, rotAngles.y, glm::dvec3(0.0,1.0,0.0));
	camera->setObjectToWorldMatrix(glm::rotate(rotate1, rotAngles.x, glm::dvec3(1.0,0,0)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR2: "<<err<<std::endl;
	}


	while((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "GLERROR3: "<<err<<std::endl;
	}

	/*
	camera->setObjectToWorldMatrix(glm::mat4(1.0));
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.3f, -0.2f, -1.f);
	glColor3f(0.f, 1.0f, 0.f);
	glVertex3f(0.3f, -0.2f, -1.0f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.3f, -1.f);
	glEnd();
	*/
}
