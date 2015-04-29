/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <io/BasicFileSystem.h>
#include <fstream>


#if defined(WIN32)
#include <Windows.h>
#else
#include <dirent.h>
#endif

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

std::vector<std::string> BasicFileSystem::listDirectory(
		const std::string& path, bool directories) {
	std::vector<std::string> files;

#if defined(WIN32)
    char search_path[300];
    sprintf(search_path, "%s*.*", path.c_str());
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(directories && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                files.push_back(fd.cFileName);
            }
            else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL))
            {
                files.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
#else
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
		  if (directories && ent->d_type == DT_DIR)
		  {
			  files.push_back(ent->d_name);
		  }
		  else if (ent->d_type == DT_REG)
		  {
			  files.push_back(ent->d_name);
		  }
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("");
	}
#endif

	return files;
}

} /* namespace MinVR */
