/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <io/BasicFileSystem.h>
#include <fstream>

namespace MinVR {

BasicFileSystem::BasicFileSystem() {
	// TODO Auto-generated constructor stub

}

BasicFileSystem::~BasicFileSystem() {
	// TODO Auto-generated destructor stub
}

bool BasicFileSystem::exists(const std::string& fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

std::string BasicFileSystem::concatPath(const std::string& pathA, const std::string& pathB)
{
	char slash = '/';
	return pathA + slash + pathB;
}

} /* namespace MinVR */
