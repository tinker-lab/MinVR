/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <log/Logger.h>
#include <Boost/BoostLogger.h>
#include <memory>

namespace MinVR {

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

Logger& Logger::getInstance() {
	static std::auto_ptr<Logger> instance = std::auto_ptr<Logger>(new BoostLogger());
	return *instance;
}

} /* namespace MinVR */
