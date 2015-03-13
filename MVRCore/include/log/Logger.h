/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

namespace MinVR {

class Logger {
public:
	virtual ~Logger();

	static Logger& getInstance();
	virtual void init() = 0;
	virtual void log(const std::string& message, const std::string& attributeName, const std::string& attributeValue) = 0;
	virtual void assertMessage(bool expression, const std::string& message) = 0;
};

} /* namespace MinVR */

#endif /* LOGGER_H_ */
