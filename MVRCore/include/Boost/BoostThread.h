/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef BOOSTTHREAD_H_
#define BOOSTTHREAD_H_

#include <boost/thread.hpp>

namespace MinVR
{

typedef boost::mutex Mutex;
typedef boost::condition_variable ConditionVariable;
typedef boost::barrier Barrier;
typedef boost::thread Thread;
typedef boost::unique_lock<Mutex> UniqueMutexLock;

}


#endif /* BOOSTTHREAD_H_ */
