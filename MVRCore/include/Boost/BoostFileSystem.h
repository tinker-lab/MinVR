/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef BOOSTFILESYSTEM_H_
#define BOOSTFILESYSTEM_H_

#include "io/FileSystem.h"

namespace MinVR {

class BoostFileSystem : public FileSystem {
public:
	BoostFileSystem();
	virtual ~BoostFileSystem();

	bool exists(const std::string& fileName);
	std::string concatPath(const std::string& pathA, const std::string& pathB);
};

} /* namespace MinVR */

#endif /* BOOSTFILESYSTEM_H_ */
