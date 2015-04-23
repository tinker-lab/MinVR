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
		std::string error;
#if defined(WIN32)
		_lib = LoadLibraryA(_filePath.c_str());
#else
		dlerror();
		_lib = dlopen(_filePath.c_str(), RTLD_NOW);//RTLD_LAZY);
		error = dlerror();
#endif

		if (!_lib) {
			MinVR::Logger::getInstance().assertMessage(false, "Could not load library: " + _filePath + " - " + error);
			return;
		}

		_isLoaded = true;
	}
}

void SharedLibrary::unload() {
	if (_isLoaded)
	{
		std::string error;
#if defined(WIN32)
		BOOL result = FreeLibrary(_lib);
#else
		dlerror();
		int result = dlclose(_lib);
		error = dlerror();
#endif
		if(result != 0) {
			MinVR::Logger::getInstance().assertMessage(false, "Could not unload library: " + _filePath + " - " + error);
			return;
		}

		_isLoaded = false;
	}
}

void* SharedLibrary::loadSymbolInternal(const std::string &functionName) {
	if (_isLoaded)
	{
#if defined(WIN32)
		FARPROC symbol =GetProcAddress(_lib, functionName.c_str());
		if (!symbol) {
			MinVR::Logger::getInstance().assertMessage(false, "Cannot load symbol: " + functionName + " - " + "");

			return NULL;
		}

		return symbol;
#else
		void* symbol = (void*) dlsym(_lib, functionName.c_str());
		const char* dlsym_error = dlerror();
		if (dlsym_error) {
			MinVR::Logger::getInstance().assertMessage(false, "Cannot load symbol: " + functionName + " - " + dlsym_error);
			dlerror();

			return NULL;
		}

		return symbol;
#endif

	}

	return NULL;
}

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */
