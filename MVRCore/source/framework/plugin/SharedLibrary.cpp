/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <framework/plugin/SharedLibrary.h>

#include "log/Logger.h"

namespace MinVR {
namespace framework {
namespace plugin {

SharedLibrary::SharedLibrary(const std::string &filePath, bool autoLoad) : _filePath(filePath), _isLoaded(false) {
	if (autoLoad)
	{
		load();
	}
}

SharedLibrary::~SharedLibrary() {
	unload();
}

void SharedLibrary::load() {
	if (!_isLoaded)
	{
		_lib = dlopen(_filePath.c_str(), RTLD_NOW);//RTLD_LAZY);
		if (!_lib) {
			const char* error = dlerror();
			MinVR::Logger::getInstance().assertMessage(false, "Could not load library: " + _filePath + " - " + error);
			dlerror();
			return;
		}

		_isLoaded = true;
	}
}

void SharedLibrary::unload() {
	if (_isLoaded)
	{
		int result = dlclose(_lib);
		if(result != 0) {
			const char* error = dlerror();
			MinVR::Logger::getInstance().assertMessage(false, "Could not unload library: " + _filePath + " - " + error);
			dlerror();
			return;
		}

		_isLoaded = false;
	}
}

void* SharedLibrary::loadSymbolInternal(const std::string &functionName) {
	if (_isLoaded)
	{
		void* symbol = (void*) dlsym(_lib, functionName.c_str());
		const char* dlsym_error = dlerror();
		if (dlsym_error) {
			MinVR::Logger::getInstance().assertMessage(false, "Cannot load symbol: " + functionName + " - " + dlsym_error);
			dlerror();

			return NULL;
		}

		return symbol;
	}

	return NULL;
}

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */
