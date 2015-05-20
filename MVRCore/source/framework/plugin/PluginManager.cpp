/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and
maintained by the University of Minnesota's Interactive Visualization Lab.

File: MinVR/MVRCore/source/framework/plugin/PluginManager.cpp

Original Author(s) of this File:
	Dan Orban, 2015, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
	...

-----------------------------------------------------------------------------------
Copyright (c) 2015 Regents of the University of Minnesota
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* The name of the University of Minnesota, nor the names of its
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

#include <framework/plugin/PluginManager.h>
#include "framework/plugin/PluginInterface.h"
#include "framework/plugin/PluginFramework.h"
#include "framework/plugin/Plugin.h"
#include "log/Logger.h"

namespace MinVR {
namespace framework {
namespace plugin {

PluginManager::PluginManager(PluginInterface *iface) : _interface(iface) {
}

PluginManager::~PluginManager() {
}

void PluginManager::loadPlugin(const std::string& filePath, const std::string& name) {

#if defined(WIN32)
	std::string path = filePath + "/bin/" + name + ".dll";
#elif defined(__APPLE__)
	std::string path = filePath + "/lib/lib" + name + ".dylib";
#else
	std::string path = filePath + "/lib/lib" + name + ".so";
#endif

	SharedLibraryRef lib = SharedLibraryRef(new SharedLibrary(path));
	if (lib->isLoaded())
	{
		typedef int version_t();
		version_t* getVersion = lib->loadSymbol<version_t>("getMinVRPluginFrameworkVersion");
		if (getVersion() != getMinVRPluginFrameworkVersion())
		{
			MinVR::Logger::getInstance().assertMessage(false, "Cannot load plugin: " + path + " - Incorrect framework version");
			return;
		}

		typedef MinVR::framework::plugin::Plugin* load_t();
		load_t* loadPlugin = lib->loadSymbol<load_t>("loadPlugin");
		if (loadPlugin == NULL)
		{
			return;
		}
		PluginRef plugin = PluginRef(loadPlugin());
		if (!plugin->registerPlugin(_interface))
		{
			MinVR::Logger::getInstance().assertMessage(false, "Failed registering plugin: " + path);
			return;
		}

		_plugins.push_back(lib);
	}
}

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */
