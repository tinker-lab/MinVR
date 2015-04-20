/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PLUGININTERFACE_H_
#define PLUGININTERFACE_H_

#include "framework/InputDevice.h"

namespace MinVR {
namespace framework {
namespace plugin {

class PluginInterface {
public:
	virtual ~PluginInterface() {}

	virtual void addGraphicsDriver() = 0;
	virtual void addInputDeviceDriver(InputDeviceDriverRef driver) = 0;
	virtual void addStereoDriver() = 0;
	virtual void addDisplayDriver() = 0;
};

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */

#endif /* PLUGININTERFACE_H_ */
