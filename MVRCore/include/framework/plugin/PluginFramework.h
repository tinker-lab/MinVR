/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PLUGINFRAMEWORK_H_
#define PLUGINFRAMEWORK_H_

#define PLUGIN_FRAMEWORK_VERSION 0

#if defined(WIN32)
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif

#endif /* PLUGINFRAMEWORK_H_ */
