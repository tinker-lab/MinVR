/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and
maintained by the University of Minnesota's Interactive Visualization Lab.

File: MinVR/MVRCore/source/log/BasicLogger.h

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

#include <log/BasicLogger.h>
#include <assert.h>

namespace MinVR {

BasicLogger::BasicLogger(std::shared_ptr< std::ostream > stream) : _ostreamPtr(stream) {
	_ostream = _ostreamPtr.get();
}

BasicLogger::BasicLogger(std::ostream *stream) : _ostream(stream) {
}

BasicLogger::~BasicLogger() {
}

void BasicLogger::init()
{
}

void BasicLogger::log(const std::string& message, const std::string& attributeName, const std::string& attributeValue)
{
	std::ostream& stream = getStream();
	stream << attributeName << " " << attributeValue << ": " << message << std::endl;
	stream.flush();
}

void BasicLogger::assertMessage(bool expression, const std::string& message)
{
	if (!expression)
	{
		std::ostream& stream = getStream();
		stream << "Assert: " << message << std::endl;
		stream.flush();
		assert(expression);
	}
}

std::ostream& BasicLogger::getStream() {
	return *_ostream;
}

} /* namespace MinVR */
