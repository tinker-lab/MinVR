/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and
maintained by the University of Minnesota's Interactive Visualization Lab.

File: MinVR/MVRCore/include/framework/plugin/SharedLibrary.h

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
