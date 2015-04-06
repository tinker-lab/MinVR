/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef DEVICEFACTORY_H_
#define DEVICEFACTORY_H_

#include "AbstractInputDevice.H"
#include <string>
#include "MVRCore/ConfigMap.H"

namespace MinVR {

typedef std::shared_ptr<class DeviceFactory> DeviceFactoryRef;

class DeviceFactory {
public:
	virtual ~DeviceFactory() {}

	virtual AbstractInputDeviceRef createInputDevice(const std::string& type, const std::string& name, ConfigMapRef devicesMap) = 0;
};

} /* namespace MinVR */

#endif /* DEVICEFACTORY_H_ */
