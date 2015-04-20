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

#include <dlfcn.h>
typedef void * HandleType;

namespace MinVR {
namespace framework {
namespace plugin {

class SharedLibrary {
public:
	SharedLibrary(const std::string &filePath, bool autoLoad = true);
	virtual ~SharedLibrary();

	void load();
	void unload();

	template<typename TSignature>
	TSignature * loadSymbol(const std::string &functionName)
	{
		return reinterpret_cast<TSignature *>(loadSymbolInternal(functionName));
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
