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
		HandleType _lib = dlopen(_filePath.c_str(), RTLD_NOW);//RTLD_LAZY);
		if (!_lib) {
			MinVR::Logger::getInstance().assertMessage(!_lib, "Could not load library: " + _filePath);
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
			MinVR::Logger::getInstance().assertMessage(false, "Could not unload library: " + _filePath);
			dlerror();
			return;
		}

		_isLoaded = false;
	}
}

void* SharedLibrary::loadSymbolInternal(const std::string &functionName) {
	void* symbol = (void*) dlsym(_lib, "functionName");
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		MinVR::Logger::getInstance().assertMessage(false, "Cannot load symbol: " + functionName);
		dlerror();

		return NULL;
	}

	return symbol;
}

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */
