/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef BASICFILESYSTEM_H_
#define BASICFILESYSTEM_H_

#include "io/FileSystem.h"

namespace MinVR {

class BasicFileSystem : public FileSystem {
public:
	BasicFileSystem();
	virtual ~BasicFileSystem();

	bool exists(const std::string& fileName);
	std::string concatPath(const std::string& pathA, const std::string& pathB);
	std::vector<std::string> listDirectory(const std::string& path, bool directories);
};

} /* namespace MinVR */

#endif /* BASICFILESYSTEM_H_ */
