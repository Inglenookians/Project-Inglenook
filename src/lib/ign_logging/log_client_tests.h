#pragma once
/*
* logging_client_tests.h: Test routines for the log_client class (log_client.cpp/h)
* Copyright (C) 2012, Project Inglenook (http://www.project-inglenook.co.uk)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LOG_TEST_NAME

// boost (http://boost.org) includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "log_client.h"

namespace inglenook
{

namespace logging
{

//
// log_client_tests__ctor
// log client construction tests, ensure that property initialization
// remains predictable and doesn't change unintentionally.
BOOST_AUTO_TEST_CASE ( log_client_tests__ctor )
{
    // create logging client interface
    auto test_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto _log_writer = log_writer::create_from_stream(test_stream);
    log_client _log_client(_log_writer);
    
    // ensure all the defaults are exactly what we expected
    BOOST_CHECK(_log_client.default_namespace()   == _log_writer->default_namespace());
    BOOST_CHECK(_log_client.default_entry_type()  == _log_writer->default_entry_type());
}

//
// log_client_tests__assignment
// log client property assignment tests, ensure that property assignments work
// as expected and that read / write assignments are working.
BOOST_AUTO_TEST_CASE ( log_client_tests__assignment )
{
    // resources
    const std::string fallback_namespace = "fallback";
    const category fallback_category = category::fatal;
    const std::string fallback_namespace_restored = "kcabllaf";
    const category fallback_category_restored = category::debugging;
    const std::string first_namespace = "first.namespace";
    const category first_category = category::warning;
    const std::string second_namespace = "second.namespace";;
    const category second_category = category::information;

    // create logging client interface
    auto test_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto _log_writer = log_writer::create_from_stream(test_stream);
    log_client _log_client(_log_writer);

    // change the expected defaults
    _log_writer->default_namespace(fallback_namespace);
    _log_writer->default_entry_type(fallback_category);
    BOOST_CHECK(_log_client.default_namespace()   == fallback_namespace);
    BOOST_CHECK(_log_client.default_entry_type()  == fallback_category);

    // change the client level settings
    _log_client.default_namespace(first_namespace);
    _log_client.default_entry_type(first_category);
    BOOST_CHECK(_log_client.default_namespace()   == first_namespace);
    BOOST_CHECK(_log_client.default_entry_type()  == first_category);

    // change the client level settings again (to catch reassignment bugs)
    _log_client.default_namespace(second_namespace);
    _log_client.default_entry_type(second_category);
    BOOST_CHECK(_log_client.default_namespace()   == second_namespace);
    BOOST_CHECK(_log_client.default_entry_type()  == second_category);

    // clear client level assignments, then tweak fallback values to ensure
    // settings are properly cleared.
    _log_client.clear_default_namespace();
    _log_client.clear_default_entry_type();
    _log_writer->default_namespace(fallback_namespace_restored);
    _log_writer->default_entry_type(fallback_category_restored);
    BOOST_CHECK(_log_client.default_namespace()   == fallback_namespace_restored);
    BOOST_CHECK(_log_client.default_entry_type()  == fallback_category_restored);
}

//
// log_client_tests__stream
// stream operator tests to ensure that the stream operators are impacting the
// log client as they are expected to.
BOOST_AUTO_TEST_CASE ( log_client_tests__stream )
{
    // create logging client interface
    auto test_stream = std::shared_ptr<std::stringstream>(new std::stringstream());

    { //scoping to discard log_client when done and check results

        auto _log_writer = log_writer::create_from_stream(test_stream, false, false);
        log_client _log_client(_log_writer);

        /////////////////////////////////
        // check switches and stream operators
        //////////////////////////////////

        // check debug switch and string insertion
        _log_client.debug() << "1";
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::debugging);
        BOOST_CHECK(_log_client.buffer()->message() == "1");

        // check trace switch and integer insertion
        _log_client.trace() << 2;
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::verbose);
        BOOST_CHECK(_log_client.buffer()->message() == "12");

        // check information switch and float insertion
        _log_client.info() << 3.4;
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::information);
        BOOST_CHECK(_log_client.buffer()->message() == "123.4");

        // check warning switch and long insertion
        _log_client.warning() << (long)5;
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::warning);
        BOOST_CHECK(_log_client.buffer()->message() == "123.45");

        // check error switch and char insertion
        _log_client.error() << '6';
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::error);
        BOOST_CHECK(_log_client.buffer()->message() == "123.456");

        // check zero switch and bool  insertion
        _log_client << true;
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::error);
        BOOST_CHECK(_log_client.buffer()->message() == "123.4561");

        // check fatal switch and fn() insertion
        _log_client.fatal() << std::endl;
        BOOST_CHECK(_log_client.buffer()->entry_type() == category::fatal);
        BOOST_CHECK(_log_client.buffer()->message() == "123.4561\n");

        /////////////////////////////////
        // check extended data
        //////////////////////////////////

        // make sure the extended data buffer is still empty
        BOOST_CHECK(_log_client.buffer()->extended_data().size() == 0);

        // set up some data to feed in to extended data, and build a
        // reference dictionary we can use to check the one we build up.
        const std::string key1 = "key1";
        const std::string value1 = "value1";
        const std::string key2 = "key2";
        const std::string value2 = "value2";
        std::map<std::string, std::string> check_data;
        check_data[key1] = value1;
        check_data[key2] = value2;

        // add the first item.
        _log_client << log_data(key1, value1);

        // check buffer is as we expect
        auto buffer = _log_client.buffer()->extended_data();
        BOOST_CHECK(buffer.size() == 1);
        for(auto data = buffer.begin(); data != buffer.end(); data++)
        {
            BOOST_CHECK(data->second == check_data[data->first]);
        }

        // add the second item
        _log_client << log_data(key2, value2);

        // check buffer is as we expect
        buffer = _log_client.buffer()->extended_data();
        BOOST_CHECK(buffer.size() == 2);
        for(auto data = buffer.begin(); data != buffer.end(); data++)
        {
            BOOST_CHECK(data->second == check_data[data->first]);
        }

        // remote the second item
        _log_client << log_data(key2, "");

        // check buffer is as we expect
        buffer = _log_client.buffer()->extended_data();
        BOOST_CHECK(buffer.size() == 1);
        for(auto data = buffer.begin(); data != buffer.end(); data++)
        {
            BOOST_CHECK(data->second == check_data[data->first]);
        }

        /////////////////////////////////
        // check namespace
        //////////////////////////////////

        const std::string first_log_namespace = "inglenook.logging.test.rewrite";
        const std::string second_log_namespace = "rewrite.test.logging.inglenook";

        // assign and check
        _log_client << ns(first_log_namespace);
        BOOST_CHECK(_log_client.buffer()->log_namespace() == first_log_namespace);

        // assign and check
        _log_client << ns(second_log_namespace);
        BOOST_CHECK(_log_client.buffer()->log_namespace() == second_log_namespace);

        // try to ensure that the log writer has chance to write anything it feels it should
        boost::this_thread::yield();
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        BOOST_CHECK(test_stream->str().length() == 0);

        // check flush operator
        _log_client << lf::end;

    } // at end of scope log_client and log_writer should be discarded, forcing log_writer
      // to flush all content and rejoin primary execution thread.

    // check result.
    BOOST_CHECK(test_stream->str().length() > 0);
}

} // namespace inglenook::logging

} // namespace inglenook

