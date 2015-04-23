/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <string>
#include <vector>
#include "framework/plugin/SharedLibrary.h"
#include "framework/plugin/PluginInterface.h"

namespace MinVR {
namespace framework {
namespace plugin {

class PluginManager {
public:
	PluginManager(PluginInterface *iface);
	virtual ~PluginManager();

	void loadPlugin(const std::string& filePath);

private:
	PluginInterface *_interface;
	std::vector<SharedLibraryRef> _plugins;
};

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */

#endif /* PLUGINMANAGER_H_ */
