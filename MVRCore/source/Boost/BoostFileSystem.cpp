/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <Boost/BoostFileSystem.h>
#include <boost/filesystem.hpp>

namespace MinVR {

BoostFileSystem::BoostFileSystem() {
	// TODO Auto-generated constructor stub

}

BoostFileSystem::~BoostFileSystem() {
	// TODO Auto-generated destructor stub
}

bool BoostFileSystem::exists(const std::string& fileName) {
	return boost::filesystem::exists(fileName);
}

std::string BoostFileSystem::concatPath(const std::string& pathA,
		const std::string& pathB) {
	return (boost::filesystem::path(pathA) / boost::filesystem::path(pathB)).string();
}

} /* namespace MinVR */
