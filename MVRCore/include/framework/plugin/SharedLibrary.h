/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef SHAREDLIBRARY_H_
#define SHAREDLIBRARY_H_

#include <string>
#include <memory>

#if defined(WIN32)
	#include <Windows.h>
	typedef HMODULE HandleType;
#else
	#include <dlfcn.h>
	typedef void * HandleType;
#endif

namespace MinVR {
namespace framework {
namespace plugin {

typedef std::shared_ptr<class SharedLibrary> SharedLibraryRef;

class SharedLibrary {
public:
	SharedLibrary(const std::string &filePath, bool autoLoad = true);
	virtual ~SharedLibrary();

	void load();
	void unload();

	bool isLoaded() {
		return _isLoaded;
	}

	template<typename TSig>
	TSig * loadSymbol(const std::string &functionName)
	{
		return reinterpret_cast<TSig *>(loadSymbolInternal(functionName));
	}

protected:
	void* loadSymbolInternal(const std::string &functionName);


private:
	std::string _filePath;
	bool _isLoaded;
	HandleType _lib;
};

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */

#endif /* SHAREDLIBRARY_H_ */
