/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and
maintained by the University of Minnesota's Interactive Visualization Lab.

File: MinVR/MVRCore/source/Boost/BoostLogger.cpp

Original Author(s) of this File:
	Dan Orban, 2015, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
	...

-----------------------------------------------------------------------------------
Copyright (c) 2015 Regents of the University of Minnesota
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* The name of the University of Minnesota, nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
================================================================================ */

#include <Boost/BoostLogger.h>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/attributes/constant.hpp>
#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
#include <fstream>
#include <iostream>

namespace MinVR {

BoostLogger::BoostLogger() {
}

BoostLogger::~BoostLogger() {
	// TODO Auto-generated destructor stub
}

} /* namespace MinVR */

void MinVR::BoostLogger::log(const std::string& message,
		const std::string& attributeName, const std::string& attributeValue) {
	boost::log::sources::logger logger;
	logger.add_attribute(attributeName, boost::log::attributes::constant< std::string >(attributeValue));
	BOOST_LOG(logger) << message;
}

void MinVR::BoostLogger::init() {
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();

    // Create a backend and attach a couple of streams to it
    boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
    backend->add_stream(boost::shared_ptr< std::ostream >(&std::cout, boost::empty_deleter()));
    backend->add_stream(boost::shared_ptr< std::ostream >(new std::ofstream("log.txt")));

    // Enable auto-flushing after each log record written
    backend->auto_flush(true);

    // Wrap it into the frontend and register in the core.
    // The backend requires synchronization in the frontend.
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
    boost::shared_ptr< sink_t > sink(new sink_t(backend));
    core->add_sink(sink);
	boost::log::add_common_attributes();
	sink->set_filter
    (
	boost::log::trivial::severity >= boost::log::trivial::info || (boost::log::expressions::has_attr(tag_attr) && tag_attr == "MinVR Core")
    );
}

void MinVR::BoostLogger::assertMessage(bool expression, const std::string& message)
{
	BOOST_ASSERT_MSG(expression, message.c_str());
}
