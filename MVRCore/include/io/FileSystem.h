/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>

namespace MinVR {

class FileSystem {
public:
	virtual ~FileSystem();

	static FileSystem& getInstance();
	virtual bool exists(const std::string& fileName) = 0;
	virtual std::string concatPath(const std::string& pathA, const std::string& pathB) = 0;
};

} /* namespace MinVR */

#endif /* FILESYSTEM_H_ */
