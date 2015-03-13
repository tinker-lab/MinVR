/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <io/FileSystem.h>
#include <Boost/BoostFileSystem.h>

#include <memory>

namespace MinVR {

FileSystem::~FileSystem() {
	// TODO Auto-generated destructor stub
}

FileSystem& FileSystem::getInstance() {
	static std::auto_ptr<FileSystem> instance = std::auto_ptr<FileSystem>(new BoostFileSystem());
	return *instance;
}

} /* namespace MinVR */
