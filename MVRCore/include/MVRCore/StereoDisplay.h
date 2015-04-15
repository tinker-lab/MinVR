/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef STEREODISPLAY_H_
#define STEREODISPLAY_H_

#include "MVRCore/AbstractMVRApp.H"

namespace MinVR {

typedef std::shared_ptr<class StereoDisplay> StereoDisplayRef;

class StereoDisplay {
public:
	virtual ~StereoDisplay() {}

	virtual void render(int threadId, WindowRef window, AbstractMVRAppRef app) = 0;
	virtual void initializeContextSpecificVars(int threadId, WindowRef window) {}
};

} /* namespace MinVR */

#endif /* STEREODISPLAY_H_ */
