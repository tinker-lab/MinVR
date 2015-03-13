/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef BOOSTTIME_H_
#define BOOSTTIME_H_


#include <boost/date_time/posix_time/posix_time.hpp>

namespace MinVR
{

#define getCurrentTime() boost::posix_time::microsec_clock::local_time()
typedef boost::posix_time::ptime TimeStamp;
typedef boost::posix_time::time_duration Duration;
#define getDuration(a,b) a - b
#define getSeconds(duration) duration.total_seconds()

}


#endif /* BOOSTTIME_H_ */
