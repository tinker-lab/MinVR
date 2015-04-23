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

	template<typename T>
	inline T* getInterface()
	{
		return dynamic_cast<T*>(this);
	}
};

class MinVRInterface : public PluginInterface {
public:
	virtual ~MinVRInterface() {}

	virtual void addGraphicsDriver() {};
	virtual void addInputDeviceDriver(InputDeviceDriverRef driver) = 0;
	virtual void addStereoDriver() {};
	virtual void addDisplayDriver() {};
};

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */

#endif /* PLUGININTERFACE_H_ */
