/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <framework/plugin/PluginManager.h>
#include "framework/plugin/PluginInterface.h"
#include "framework/plugin/PluginFramework.h"
#include "framework/plugin/Plugin.h"
#include "log/Logger.h"

namespace MinVR {
namespace framework {
namespace plugin {

PluginManager::PluginManager(PluginInterface *interface) : _interface(interface) {
}

PluginManager::~PluginManager() {
}

void PluginManager::loadPlugin(const std::string& filePath) {
	SharedLibraryRef lib = SharedLibraryRef(new SharedLibrary(filePath));
	if (lib->isLoaded())
	{
		typedef int version_t();
		version_t* getVersion = lib->loadSymbol<version_t>("getMinVRPluginFrameworkVersion");
		if (getVersion() != getMinVRPluginFrameworkVersion())
		{
			MinVR::Logger::getInstance().assertMessage(false, "Cannot load plugin: " + filePath + " - Incorrect framework version");
			return;
		}

		typedef MinVR::framework::plugin::PluginRef load_t();
		load_t* loadPlugin = lib->loadSymbol<load_t>("loadPlugin");
		if (loadPlugin == NULL)
		{
			return;
		}
		PluginRef plugin = loadPlugin();
		if (!plugin->registerPlugin(_interface))
		{
			MinVR::Logger::getInstance().assertMessage(false, "Failed registering plugin: " + filePath);
			return;
		}

		_plugins.push_back(lib);
	}
}

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */
