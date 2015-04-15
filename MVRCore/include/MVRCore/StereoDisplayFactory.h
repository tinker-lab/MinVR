/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef STEREODISPLAYFACTORY_H_
#define STEREODISPLAYFACTORY_H_

#include <string>
#include "StereoDisplay.h"

namespace MinVR {

typedef std::shared_ptr<class StereoDisplayFactory> StereoDisplayFactoryRef;

class StereoDisplayFactory {
public:
	virtual ~StereoDisplayFactory() {}

	virtual StereoDisplayRef create(const std::string& type) = 0;
};

} /* namespace MinVR */

#endif /* STEREODISPLAYFACTORY_H_ */
