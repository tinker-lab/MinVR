/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PLUGIN_H_
#define PLUGIN_H_

#include "framework/plugin/PluginFramework.h"
#include "framework/plugin/PluginInterface.h"
#include <memory>

namespace MinVR {
namespace framework {
namespace plugin {

typedef std::shared_ptr<class Plugin> PluginRef;

class Plugin {
public:
	virtual ~Plugin() {}

	virtual bool registerPlugin(PluginInterface *interface) = 0;
};

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */


extern "C"
{
	int getMinVRPluginFrameworkVersion() {
		return MINVR_PLUGIN_FRAMEWORK_VERSION;
	}
}

/*

extern "C"
{
	MinVR::framework::plugin::PluginRef loadPlugin() {
		return MinVR::framework::plugin::PluginRef(new MinVR::framework::plugin::Plugin());
	}
}
*/

#endif /* PLUGIN_H_ */
