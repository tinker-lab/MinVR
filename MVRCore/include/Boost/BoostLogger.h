/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef BOOSTLOGGER_H_
#define BOOSTLOGGER_H_

#include "log/Logger.h"

namespace MinVR {

class BoostLogger : public Logger {
public:
	BoostLogger();
	virtual ~BoostLogger();

	void init();
	void log(const std::string& message, const std::string& attributeName, const std::string& attributeValue);
	void assertMessage(bool expression, const std::string& message);
};

} /* namespace MinVR */

#endif /* BOOSTLOGGER_H_ */
