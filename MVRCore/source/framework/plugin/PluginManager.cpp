/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <framework/plugin/PluginManager.h>

namespace MinVR {
namespace framework {
namespace plugin {

PluginManager::PluginManager(PluginInterface *interface) : _interface(interface) {

}

PluginManager::~PluginManager() {
}

} /* namespace plugin */
} /* namespace framework */
} /* namespace MinVR */
