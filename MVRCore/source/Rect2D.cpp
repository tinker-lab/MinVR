/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/MVRCore/source/Rect2D.cpp

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


#include "MVRCore/Rect2D.H"

namespace MinVR {

Rect2D::Rect2D()
{
}

Rect2D::Rect2D(int width, int height, int xpos, int ypos)
{
	_width = width;
	_height = height;
	_x = xpos;
	_y = ypos;
}

Rect2D::~Rect2D()
{
}

Rect2D Rect2D::xywh(int x, int y, int width, int height)
{
	return Rect2D(width, height, x, y);
}

int Rect2D::width()
{
	return _width;
}

int Rect2D::height()
{
	return _height;
}

int Rect2D::x0()
{
	return _x;
}
	
int Rect2D::y0()
{
	return _y;
}

int Rect2D::x1()
{
	return _x+_width;
}

int Rect2D::y1()
{
	return _y+_height;
}

void Rect2D::scale(glm::dvec2 scaleValues){
	_height *= scaleValues.y;
	_width *= scaleValues.x;
	_x *= scaleValues.x;
	_y *= scaleValues.y;
}

} // namespace