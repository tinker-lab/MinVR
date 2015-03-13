/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/AppKits/AppKit_G3D9/example/source/G3D9DemoApp.cpp 

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

#include "G3D9DemoApp.H"

using namespace G3D;
using namespace MinVR;

G3D9DemoApp::G3D9DemoApp() : MinVR::AbstractMVRApp()
{
}

G3D9DemoApp::~G3D9DemoApp()
{
}

void G3D9DemoApp::doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime)
{
	for(int i=0; i < events.size(); i++) {
		if (events[i]->getName() == "kbd_ESC_down") {
			exit(0);
		}
		//std::cout << events[i]->getName()<<std::endl;
	}
}

void G3D9DemoApp::initializeContextSpecificVars(int threadId, MinVR::WindowRef window)
{
	initVBO(threadId);

	RenderDevice* rd = dynamic_cast<WindowG3D9*>(window.get())->getRenderDevice();
	rd->setColorClearValue(Color4(0.2f, 0.2f, 0.2f, 1.0f));

}

void G3D9DemoApp::initVBO(int threadId)
{
	// Allocate on CPU
	Array<Vector3> cpuVertex;
	Array<Vector3> cpuNormals;
	Array<Color3> cpuColors;
	Array<int> cpuIndex;

	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(1, 1,-1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1, 1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(1, 1,-1));
	cpuVertex.append(Vector3(1,-1,-1));

	// normal array
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));

	// color array
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(0, 1, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(0, 1, 1));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 1, 1));
	cpuColors.append(Color3(0, 0, 1));

	for(int i=0; i < cpuVertex.size(); i++) {
		cpuIndex.append(i);
	}


	// Upload to GPU
	shared_ptr<VertexBuffer> vbuf = VertexBuffer::create(((sizeof(Vector3)*2) * cpuVertex.size()) + (sizeof(Color3)*cpuColors.size()) + (sizeof(int) * cpuIndex.size()));
	//_vbuffer.reset(vbuf.get());
	_gpuVertex[threadId] = AttributeArray(cpuVertex, vbuf);
	_gpuNormals[threadId] = AttributeArray(cpuNormals, vbuf);
	_gpuColors[threadId] = AttributeArray(cpuColors, vbuf);
	_gpuIndex[threadId] = IndexStream(cpuIndex, vbuf);
}

void G3D9DemoApp::postInitialization()
{
}

void G3D9DemoApp::drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window)
{
	G3D::RenderDevice* rd = dynamic_cast<WindowG3D9*>(window.get())->getRenderDevice();

	rd->beginFrame();
	rd->pushState();

	glm::dmat4 translate = glm::translate(glm::dmat4(1.0f), glm::dvec3(0.0f, 0.0f, -5.0f));
	glm::dvec2 rotAngles(-20.0, 45.0);
	glm::dmat4 rotate1 = glm::rotate(translate, rotAngles.y, glm::dvec3(0.0,1.0,0.0));
	camera->setObjectToWorldMatrix(glm::rotate(rotate1, rotAngles.x, glm::dvec3(1.0,0,0)));

	rd->setShadeMode(RenderDevice::SHADE_SMOOTH);

	rd->beginIndexedPrimitives();
	rd->setVertexArray(_gpuVertex[threadId]);
	rd->setNormalArray(_gpuNormals[threadId]);
	rd->setColorArray(_gpuColors[threadId]);
	rd->sendIndices(PrimitiveType::TRIANGLES, _gpuIndex[threadId]);
	rd->endIndexedPrimitives();
		
	
	/*
	rd->beginPrimitive(G3D::PrimitiveType::TRIANGLES);
	rd->setColor(G3D::Color3(1.f, 0.f, 0.f));
	rd->setNormal(Vector3(0,0,1));
	rd->sendVertex(Vector3(-0.3f, -0.2f, -1.f));
	rd->setColor(G3D::Color3(0.f, 1.0f, 0.f));
	rd->sendVertex(Vector3(0.3f, -0.2f, -1.0f));
	rd->setColor(G3D::Color3(0.f, 0.f, 1.f));
	rd->sendVertex(Vector3(0.f, 0.3f, -1.f));
	rd->endPrimitive();
	*/

	rd->popState();
	rd->endFrame();
}
