/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

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
