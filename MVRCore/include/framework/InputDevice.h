/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef INPUTDEVICE_H_
#define INPUTDEVICE_H_

#include <memory>

namespace MinVR {
namespace framework {

typedef std::shared_ptr<class InputDevice> InputDeviceRef;
typedef std::shared_ptr<class InputDeviceFactory> InputDeviceDriverRef;

class InputDevice {
public:
	virtual ~InputDevice() {}
};

class InputDeviceDriver {
public:
	virtual ~InputDeviceDriver() {}

	virtual InputDeviceRef create() = 0;
};

} /* namespace framework */
} /* namespace MinVR */

#endif /* INPUTDEVICE_H_ */
