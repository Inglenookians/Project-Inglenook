#pragma once
/*
* logging_writer_tests.h: Test routines for the log_writer class (log_writer.cpp/h)
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

// standard includes
// #include <regex> // gcc regex is non-functional, using boost instead.
#include <ctime>

// boost (http://boost.org) includes
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/scope_exit.hpp>
#include <boost/regex.hpp>

// inglenook includes
#include "log_writer.h"

namespace inglenook
{

namespace logging
{

/**
 * Creates a new log entry with specified parameters
 * @param submit_as category of message to create.
 * @param message message body for the log entry.
 * @param log_namespace entry namespace.
 * @returns shared pointer to log entry
 */
std::shared_ptr<log_entry> create_log_entry(const category& submit_as, const std::string& message, const std::string& log_namespace)
{
    // create and return shared pointer
    auto le = std::shared_ptr<log_entry>(new log_entry());
    le->message(message);
    le->log_namespace(log_namespace);
    le->entry_type(submit_as);
    return le;
}

/**
 * Test scenario for log_writer.
 * Creates a log writer and entry with the specified parameters, and returns the xml, cout and cerr results of
 * passing that entry in to the writer.
 * @param _log_entry [log_entry] log entry to write
 * @param xml_filter [log_writer] XML serialiation threshold.
 * @param console_filter [log_writer] Console serialiation threshold.
 * @param default_namespace [log_writer] default fallback namespace
 * @param xml_out [output] how the entry appears in XML.
 * @param console_cout_out [output] how the entry appears in cout.
 * @param console_cerr_out [output] how the entry appears in cerr.
 */
void run_writer(std::shared_ptr<log_entry> _log_entry,
                const category& xml_filter, const category& console_filter, const std::string& default_namespace,
                std::string& xml_out, std::string& console_cout_out, std::string& console_cerr_out)
{
    // initialize the stream writer and clear any pre-amble from the output stream
    auto test_xml_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto _log_writer = log_writer::create_from_stream(test_xml_stream, false, false);
    _log_writer->default_namespace(default_namespace);
    _log_writer->console_threshold(console_filter);
    _log_writer->xml_threshold(xml_filter);

    // capture standard output (so we can check what is coming out)
    auto test_console_cout_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto test_console_cerr_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto cout_buffer = std::cout.rdbuf();
    auto cerr_buffer = std::cerr.rdbuf();

    // reassign the buffers
    std::cout.rdbuf(test_console_cout_stream->rdbuf());
    std::cerr.rdbuf(test_console_cerr_stream->rdbuf());

    // setup scoped exit to ensure we reset std::cout
    BOOST_SCOPE_EXIT( (&cout_buffer) (&cerr_buffer))
    {
        std::cout.rdbuf(cout_buffer);
        std::cerr.rdbuf(cerr_buffer);
    } BOOST_SCOPE_EXIT_END

    // ensure we are running no output yet
    BOOST_CHECK(test_xml_stream->str() == "");
    BOOST_CHECK(test_console_cout_stream->str() == "");
    BOOST_CHECK(test_console_cerr_stream->str() == "");

    // create a log entry that we expect to see...
    _log_writer->add_entry(_log_entry);

    // reset the writer (this should flush ouput, and cause the logging method to rejoins the
    // primary execution thread) which gaurentees all output has been finished and is ready for
    // evaluation.
    _log_writer.reset();

    // capture the results of the two opreations
    console_cout_out = test_console_cout_stream->str();
    console_cerr_out = test_console_cerr_stream->str();
    xml_out = test_xml_stream->str();
}

/**
 * Test scenario for log_writer.
 * Creates a log writer and entry with the specified parameters, and returns the xml, cout and cerr results of
 * passing that entry in to the writer.
 * @param submit_as [log_entry] category of message to create.
 * @param message [log_entry] message body for the log entry.
 * @param log_namespace [log_entry] entry namespace.
 * @param xml_filter [log_writer] XML serialiation threshold.
 * @param console_filter [log_writer] Console serialiation threshold.
 * @param default_namespace [log_writer] default fallback namespace
 * @param xml_out [output] how the entry appears in XML.
 * @param console_cout_out [output] how the entry appears in cout.
 * @param console_cerr_out [output] how the entry appears in cerr.
 */
void run_writer(const category& submit_as, const std::string& message, const std::string& log_namespace,
                const category& xml_filter, const category& console_filter, const std::string& default_namespace,
                std::string& xml_out, std::string& console_cout_out, std::string& console_cerr_out)
{
    // run writer with specified parameters.
    auto le = create_log_entry(submit_as, message, log_namespace);
    run_writer(le, xml_filter, console_filter, default_namespace, xml_out, console_cout_out, console_cerr_out);
}

/**
 * Filter test scenario for log_writer.
 * Use to test filter scenario's with run_writer_with_filter, allows us to drop 3 parameters.
 * @param submit_as [log_entry] category of message to create.
 * @param xml_filter [log_writer] XML serialiation threshold.
 * @param console_filter [log_writer] Console serialiation threshold.
 * @param xml_out [output] how the entry appears in XML.
 * @param console_cout_out [output] how the entry appears in cout.
 * @param console_cerr_out [output] how the entry appears in cerr.
 */
void filter_scenario(const category& entry_category, const category& xml_filter, const category& console_filter,
                     std::string& xml_out, std::string& console_cout_out, std::string& console_cerr_out)
{
    // resources
    const std::string message    = "log_writer_tests__filteration";
    const std::string ns         = "inglenook.logging.tests";
    const std::string default_ns = "inglenook.logging.tests.fallback";

    // run the writer
    run_writer(
            entry_category, message, ns,
            xml_filter, console_filter, default_ns,
            xml_out, console_cout_out, console_cerr_out);
}

//
// log_writer_tests__ctor_dtor
// checks all the default values for correctness. This should ensure that
// the default values are predictable, and that the read accessors appear
// to be working as expected (these will be further tested). it also ensures
// that the basic XML framework appears to be correctly formed.
BOOST_AUTO_TEST_CASE ( log_writer_tests__ctor_dtor )
{
    // resources
    const std::string expected_xml_part1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><inglenook-log-file xmlns=\"http://schemas.project-inglenook.co.uk/0.00-DEVELOPMENT/schemas/file-formats/inglenook-log-file.xsd\"><process-id pid=\"";
    const std::string expected_xml_part2 = "\"><binary-name><![CDATA[ign_logging_tests]]></binary-name><binary-version><![CDATA[";
    const std::string expected_xml_part3 = "]]></binary-version><log-writer-version><![CDATA[v1.0.0000]]></log-writer-version></process-id><log-entries></log-entries></inglenook-log-file>";

    // create logging interface
    auto test_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto _log_writer = log_writer::create_from_stream(test_stream);

    // ensure all the defaults are exactly what we expected
    BOOST_CHECK(_log_writer->default_namespace()   == "inglenook.anonymous");
    BOOST_CHECK(_log_writer->default_entry_type()  == category::information);
    BOOST_CHECK(_log_writer->xml_threshold()       == category::information);
    BOOST_CHECK(_log_writer->console_threshold()   == category::information );
    BOOST_CHECK(_log_writer->process_name()        ==  "ign_logging_tests" );

    // a bit fuzzy but not a lot we can do here (any better ideas?)...
    BOOST_CHECK(_log_writer->pid() > 0 ); // make sure we have some pid?

    // store pid for next test (validating XML work) and reset
    int pid = _log_writer->pid();
    _log_writer.reset();

    // verify that the logging mechanism started, and closed the file correctly.
    auto expected_xml = std::shared_ptr<std::stringstream>(new std::stringstream());
    (*expected_xml.get()) << expected_xml_part1 << pid << expected_xml_part2
            << inglenook::core::application::version() << expected_xml_part3;
    
    BOOST_CHECK(test_stream->str() == expected_xml->str());
}

//
// log_writer_tests__ctor_dtor_specific
// checks construction using specific PID and application names.
BOOST_AUTO_TEST_CASE ( log_writer_tests__ctor_dtor_specific )
{
    // resources
    const std::string expected_xml_part1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><inglenook-log-file xmlns=\"http://schemas.project-inglenook.co.uk/0.00-DEVELOPMENT/schemas/file-formats/inglenook-log-file.xsd\"><process-id pid=\"";
    const std::string expected_xml_part2 = "\"><binary-name><![CDATA[";
    const std::string expected_xml_part3 = "]]></binary-name><binary-version><![CDATA[";
    const std::string expected_xml_part4 = "]]></binary-version><log-writer-version><![CDATA[v1.0.0000]]></log-writer-version></process-id><log-entries></log-entries></inglenook-log-file>";

    const pid_type application_pid = 1234;
    const std::string application_name = "test-script-name";

    // create logging interface
    auto test_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto _log_writer = log_writer::create_from_stream(test_stream, true, true, application_pid, application_name);

    // ensure all the defaults are exactly what we expected
    BOOST_CHECK(_log_writer->default_namespace()   == "inglenook.anonymous");
    BOOST_CHECK(_log_writer->default_entry_type()  == category::information);
    BOOST_CHECK(_log_writer->xml_threshold()       == category::information);
    BOOST_CHECK(_log_writer->console_threshold()   == category::information );
    BOOST_CHECK(_log_writer->process_name()        == application_name );
    BOOST_CHECK(_log_writer->pid()                 == application_pid );

    // store pid for next test (validating XML work) and reset
    int pid = _log_writer->pid();
    _log_writer.reset();

    // verify that the logging mechanism started, and closed the file correctly.
    auto expected_xml = std::shared_ptr<std::stringstream>(new std::stringstream());
    (*expected_xml.get()) << expected_xml_part1 << application_pid << expected_xml_part2
            << application_name << expected_xml_part3 << inglenook::core::application::version() << expected_xml_part4;

    BOOST_CHECK(test_stream->str() == expected_xml->str());
}


//
// log_writer_tests__ctor_dtor_otr
// When in off-the-record mode the log_writer does not write disk. This is flagged by providing
// a nullptr to a create_from_stream(), these tests are designed to make sure this being set to
// a null reference does not cause any issues.
BOOST_AUTO_TEST_CASE ( log_writer_tests__ctor_dtor_otr )
{
    // resources
    const std::string message_body_cout = "message body";
    const std::string message_ns_cout = "inglenook.logging.tests";
    const std::string message_body_cerr = "message body.fail";
    const std::string message_ns_cerr = "inglenook.logging.tests.fail";

    // first we'll create the stream. we'll even tell it to emit XML header / footers.
    auto _log_writer = log_writer::create_from_stream(nullptr, true, true);

    // create a test entries
    auto test_entry_cerr = create_log_entry(category::fatal, message_body_cerr, message_ns_cerr);
    auto test_entry_cout = create_log_entry(category::information, message_body_cout, message_ns_cout);

    // capture standard output (so we can check what is coming out)
    auto test_console_cout_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto test_console_cerr_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto cout_buffer = std::cout.rdbuf();
    auto cerr_buffer = std::cerr.rdbuf();

    // reassign the buffers
    std::cout.rdbuf(test_console_cout_stream->rdbuf());
    std::cerr.rdbuf(test_console_cerr_stream->rdbuf());

    // setup scoped exit to ensure we reset std::cout
    BOOST_SCOPE_EXIT( (&cout_buffer) (&cerr_buffer))
    {
        std::cout.rdbuf(cout_buffer);
        std::cerr.rdbuf(cerr_buffer);
    } BOOST_SCOPE_EXIT_END

    // add two entries to stream
    _log_writer->add_entry(test_entry_cout);
    _log_writer->add_entry(test_entry_cerr);

    // if we didn't crash things are looking good - just cleanup to be sure
    // reset will also flush output.
    _log_writer.reset();

    // check output was what was expected.
    BOOST_CHECK(test_console_cout_stream->str() == message_body_cout + "\n");
    BOOST_CHECK(test_console_cerr_stream->str() == message_body_cerr + "\n");
}

//
// log_writer_tests__assignments
// These tests ensure that the properties associated with this class function as 
// advertised, that setting and recalling the properties gives expected results.
BOOST_AUTO_TEST_CASE ( log_writer_tests__assignments )
{
    // declare resources
    const std::string first_namespace = "inglenook.test.logging";
    const std::string second_namespace = "logging.test.inglenook";
    const std::string third_namespace = "";

    // create a writer, operating on a defined stream.
    auto test_stream = std::shared_ptr<std::stringstream>(new std::stringstream());
    auto _log_writer = log_writer::create_from_stream(test_stream, false, false);

    // reset the test stream
    test_stream->str(std::string());
    test_stream->clear();

    // check assignment and recall of log_namespace property
    _log_writer->default_namespace(first_namespace);         BOOST_CHECK(_log_writer->default_namespace() == first_namespace);
    _log_writer->default_namespace(second_namespace);        BOOST_CHECK(_log_writer->default_namespace() == second_namespace);
    _log_writer->default_namespace(third_namespace);         BOOST_CHECK(_log_writer->default_namespace() == third_namespace);

    // check assignment and recall of default_entry_type property
    _log_writer->default_entry_type(category::debugging);    BOOST_CHECK(_log_writer->default_entry_type() == category::debugging);
    _log_writer->default_entry_type(category::verbose);      BOOST_CHECK(_log_writer->default_entry_type() == category::verbose);
    _log_writer->default_entry_type(category::information);  BOOST_CHECK(_log_writer->default_entry_type() == category::information);
    _log_writer->default_entry_type(category::warning);      BOOST_CHECK(_log_writer->default_entry_type() == category::warning);
    _log_writer->default_entry_type(category::error);        BOOST_CHECK(_log_writer->default_entry_type() == category::error);
    _log_writer->default_entry_type(category::fatal);        BOOST_CHECK(_log_writer->default_entry_type() == category::fatal);
    
    // check assignment and recall of xml_threshold property
    _log_writer->xml_threshold(category::debugging);         BOOST_CHECK(_log_writer->xml_threshold() == category::debugging);
    _log_writer->xml_threshold(category::verbose);           BOOST_CHECK(_log_writer->xml_threshold() == category::verbose);
    _log_writer->xml_threshold(category::information);       BOOST_CHECK(_log_writer->xml_threshold() == category::information);
    _log_writer->xml_threshold(category::warning);           BOOST_CHECK(_log_writer->xml_threshold() == category::warning);
    _log_writer->xml_threshold(category::error);             BOOST_CHECK(_log_writer->xml_threshold() == category::error);
    _log_writer->xml_threshold(category::fatal);             BOOST_CHECK(_log_writer->xml_threshold() == category::fatal);

    // check assignment and recall of console_threshold property
    _log_writer->console_threshold(category::debugging);     BOOST_CHECK(_log_writer->console_threshold() == category::debugging);
    _log_writer->console_threshold(category::verbose);       BOOST_CHECK(_log_writer->console_threshold() == category::verbose);
    _log_writer->console_threshold(category::information);   BOOST_CHECK(_log_writer->console_threshold() == category::information);
    _log_writer->console_threshold(category::warning);       BOOST_CHECK(_log_writer->console_threshold() == category::warning);
    _log_writer->console_threshold(category::error);         BOOST_CHECK(_log_writer->console_threshold() == category::error);
    _log_writer->console_threshold(category::fatal);         BOOST_CHECK(_log_writer->console_threshold() == category::fatal);

    // nothing should have been written to the stream during this test
    boost::this_thread::yield(); boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    BOOST_CHECK(test_stream->str() == "");
}

//
// log_writer_tests__filteration
// These tests ensure that logging filteration is working as expected.
BOOST_AUTO_TEST_CASE ( log_writer_tests__filteration )
{
    // resources
    std::string xml_output("");
    std::string console_cout_output("");
    std::string console_cerr_output("");

    //
    // NOTE: these are not tests of the content, only tests of the output filters acting as expected.
    //

    // increment of serious error message, from unrestricted logging through to no logging. 
    filter_scenario(category::fatal, category::unspecified, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::fatal, category::debugging, category::debugging, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::fatal, category::information, category::information, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::fatal, category::fatal, category::fatal, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::fatal, category::no_log, category::no_log, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");

    // increment of standard message, from unrestricted logging through to no logging. 
    filter_scenario(category::information, category::unspecified, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::debugging, category::debugging, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::information, category::information, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::warning, category::warning, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::fatal, category::fatal, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::no_log, category::no_log, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");

    // making sure no_log and unspecified categories never get handled
    filter_scenario(category::unspecified, category::unspecified, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::unspecified, category::no_log, category::no_log, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::unspecified, category::debugging, category::debugging, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::no_log, category::unspecified, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::no_log, category::no_log, category::no_log, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::no_log, category::debugging, category::debugging, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");

    // thus far xml and console serialization have been tested in pairs, now testing unmatched pairs...
    filter_scenario(category::debugging, category::unspecified, category::no_log, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::debugging, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::information, category::fatal, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::fatal, category::information, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::error, category::fatal, category::information, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::error, category::information, category::fatal, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::error, category::fatal, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::fatal, category::debugging, category::information, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::information, category::debugging, category::verbose, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");

    // finally check that the switch between cerr/cout based on entry_type is working
    filter_scenario(category::debugging, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::verbose, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::information, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");
    filter_scenario(category::warning, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::error, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");
    filter_scenario(category::fatal, category::no_log, category::unspecified, xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output != "");

    const std::string message = "message";
    const std::string log_namespace = "inglenook.logging.test";
    const std::string not_supplied = "";

    // demonstration of working format (ensures below are failing because of the intended shortcoming)
    run_writer(category::information, message, log_namespace,
               category::information, category::information, log_namespace,
               xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");

    // ensure that items without a namespace when they arrive at the writer are rejected.
    run_writer(category::information, message, not_supplied,
               category::information, category::information, log_namespace,
               xml_output, console_cout_output, console_cerr_output);
    // we expect the "inglenook.anonymous" namespace to be added to blank entries.
    BOOST_CHECK(xml_output != ""); BOOST_CHECK(console_cout_output != "");  BOOST_CHECK(console_cerr_output == "");

    // ensure that items without a message when they arrive at the writer are rejected.
    run_writer(category::information, not_supplied, log_namespace,
           category::information, category::information, log_namespace,
           xml_output, console_cout_output, console_cerr_output);
    BOOST_CHECK(xml_output == ""); BOOST_CHECK(console_cout_output == "");  BOOST_CHECK(console_cerr_output == "");
}

/**
 * Returns the number of seconds since the specified UTC time.
 * @param check_date UTC date and time to return delta of.
 * @returns the delta between UTC now and check_date in seconds.
 */
int time_delta_in_seconds(std::tm& check_date)
{
    // get gmt time
    time_t now = time(0);
    std::tm* timestamp_for_gmt = std::gmtime(&now);
    
    // we are about to use mktime, which is akward. make sure the time zone is checked
    auto environment_timezone = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();

    // make sure we unset the timezone.
    BOOST_SCOPE_EXIT((&environment_timezone))
    {
        if(environment_timezone)
        {
            setenv("TZ", environment_timezone, 1);
        }
        else
        {
            unsetenv("TZ");
        }
        tzset();
    } BOOST_SCOPE_EXIT_END

    // create time structures
    std::time_t _time_gmt  = std::mktime (timestamp_for_gmt);
    std::time_t _time_file = std::mktime (&check_date);

    // return the delta
    return(_time_gmt - _time_file);
}

/**
 * Takes a fragmented time and attempts to ensure it looks correct (within tolerance seconds).
 * @param year year component of date and time.
 * @param month month component of date and time.
 * @param day day component of date and time.
 * @param hour hour component of date and time.
 * @param minute minute component of date and time.
 * @param second second component of date and time.
 * @param tolerance number of seconds since "now" to allow time to drift.
 */
void build_and_check_time(int year, int month, int day, int hour, int minute, int second, int tolerance = 1)
{
    // check boundries
    BOOST_CHECK(year >= 2012);
    BOOST_CHECK(month > 0 && month < 13);
    BOOST_CHECK(day > 0 && day < 32);
    BOOST_CHECK(hour >= 0 && hour < 24);
    BOOST_CHECK(minute >= 0 && minute < 60);
    BOOST_CHECK(second >= 0 && second < 62); // 60-61 can occur on leap second.

    // build a tm struct to check time
    std::tm timestamp_on_filename;
    memset(&timestamp_on_filename, 0, sizeof(std::tm));
    timestamp_on_filename.tm_year = year - 1900;
    timestamp_on_filename.tm_mon = month - 1;
    timestamp_on_filename.tm_mday = day;
    timestamp_on_filename.tm_hour = hour;
    timestamp_on_filename.tm_min = minute;
    timestamp_on_filename.tm_sec = second;

    // check that time appears to be correct.
    BOOST_CHECK(time_delta_in_seconds(timestamp_on_filename) <= tolerance);
}

//
// log_writer_tests__default_log_file
// Test that the default log file suggested by the software appears to be correct
BOOST_AUTO_TEST_CASE ( log_writer_tests__default_log_file )
{
    // resources
    const std::string specific_application = "test-script.sh";
    const pid_type specific_pid = 1234;

    // initialize the stream writer and clear any pre-amble from the output stream
    boost::filesystem::path result = log_writer::default_log_path();
    std::string file_path = result.native();

    result  = log_writer::default_log_path(specific_pid, specific_application);
    std::string specific_file_path = result.native();
    
    // Linux and OSX support (officially tested and maintained).
#if defined(__linux__) || defined(__APPLE__)

    // setup regex elements to check results.
    const std::string expect_format = "^/var/log/inglenook/ign_logging_tests/([0-9]{4})([0-9]{2})([0-9]{2})-([0-9]{2})([0-9]{2})([0-9]{2})-([0-9]+)\\.xml$";
    boost::regex validate_result(expect_format);
    std::string::const_iterator start = file_path.begin();
    std::string::const_iterator end =  file_path.end();
    boost::match_results<std::string::const_iterator> items;
    boost::match_flag_type flags = boost::match_default;

    // attempt to validate the file path
    bool path_appears_valid = regex_search(start, end, items, validate_result, flags);
    BOOST_CHECK(path_appears_valid);

    // if the path appears valid, break it down a bit
    if(path_appears_valid)
    {
        using boost::lexical_cast;

        build_and_check_time(
            lexical_cast<int>(items[1]), // year
            lexical_cast<int>(items[2]), // month
            lexical_cast<int>(items[3]), // day
            lexical_cast<int>(items[4]), // hour
            lexical_cast<int>(items[5]), // minute
            lexical_cast<int>(items[6])  // second
        );

        // check the pid value appears reasonable.
        BOOST_CHECK(lexical_cast<int>(items[7]) > 0 );
    }
    
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
}

//
// log_writer_tests__ouput
// These tests ensure that the output of the writer is consitent with
// what is expected. They are aimed at testing only the serialization aspects of the software
BOOST_AUTO_TEST_CASE ( log_writer_tests__ouput )
{
    // resources
    std::string xml_output("");
    std::string console_cout_output("");
    std::string console_cerr_output("");

    const std::string message_first = "Lorem ipsum dolar sit amet";
    const std::string log_namespace = "inglenook.logging.test";
    const category log_category = category::information;
    const std::string default_namespace = "inglenook.logging.test";
    const category log_all = category::unspecified;

    // demonstration of working format (ensures below are failing because of the intended shortcoming)
    run_writer(log_category, message_first, log_namespace,
            log_all, log_all, default_namespace,
            xml_output, console_cout_output, console_cerr_output);

    // setup regex elements to check results.
    const std::string expect_format_first = "^<log-entry timestamp=\"([0-9]{4})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})((\\.([0-9]+))?)Z\" severity=\"([0-9]{1,2})\" ns=\"(.+)\"><message><!\\[CDATA\\[(.+)\\]\\]></message></log-entry>$";
    boost::regex validate_result_first(expect_format_first);
    std::string::const_iterator start = xml_output.begin();
    std::string::const_iterator end =  xml_output.end();
    boost::match_results<std::string::const_iterator> items;
    boost::match_flag_type flags = boost::match_default;

    // attempt to validate the file path
    bool xml_appears_valid = regex_search(start, end, items, validate_result_first, flags);
    BOOST_CHECK(xml_appears_valid); BOOST_CHECK(console_cout_output == message_first + "\n");  BOOST_CHECK(console_cerr_output == "");

    // validate the xml components if things look good.
    if(xml_appears_valid)
    {
        using boost::lexical_cast;

        build_and_check_time(
            lexical_cast<int>(items[1]), // year
            lexical_cast<int>(items[2]), // month
            lexical_cast<int>(items[3]), // day
            lexical_cast<int>(items[4]), // hour
            lexical_cast<int>(items[5]), // minute
            lexical_cast<int>(items[6])  // second
        );

        // check that time appears to be correct.
        BOOST_CHECK(message_first == items[12]);
        BOOST_CHECK(log_namespace == items[11]);
        BOOST_CHECK(lexical_cast<int>(items[10]) == (int)log_category);
    }

    const std::string extended_key_1 = "ek1";
    const std::string extended_key_2 = "ek2";
    const std::string extended_value_1 = "v1";
    const std::string extended_value_2 = "<v2/>";
    const std::string message_second = "this contains <bold>html</bold>";
    const std::string message_second_expected = "this contains &lt;bold&gt;html&lt;/bold&gt;";
    const std::string log_data = "<item key=\"ek1\"><![CDATA[v1]]></item><item key=\"ek2\"><![CDATA[&lt;v2/&gt;]]></item>";

    // create new log to test default namespace and extended data properties.
    auto le = create_log_entry(log_category, message_second, "");
    le->extended_data(extended_key_1, extended_value_1);
    le->extended_data(extended_key_2, extended_value_2);

    // demonstration of working format (ensures below are failing because of the intended shortcoming)
    run_writer(le, log_all, log_all, default_namespace,
            xml_output, console_cout_output, console_cerr_output);

    // setup regex elements to check results.
    const std::string expect_format_second = "^<log-entry timestamp=\"([0-9]{4})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})((\\.([0-9]+))?)Z\" severity=\"([0-9]{1,2})\" ns=\"(.+)\"><message><!\\[CDATA\\[(.+)\\]\\]></message><extended-data>(.+)</extended-data></log-entry>$";
    boost::regex validate_result_second(expect_format_second);
    start = xml_output.begin();
    end =  xml_output.end();

    // attempt to validate the file path
    xml_appears_valid = regex_search(start, end, items, validate_result_second, flags);
    BOOST_CHECK(xml_appears_valid); BOOST_CHECK(console_cout_output == message_second + "\n");  BOOST_CHECK(console_cerr_output == "");

    if(xml_appears_valid)
    {
        using boost::lexical_cast;

        // check that entry timestamp is good
        build_and_check_time(
            lexical_cast<int>(items[1]), // year
            lexical_cast<int>(items[2]), // month
            lexical_cast<int>(items[3]), // day
            lexical_cast<int>(items[4]), // hour
            lexical_cast<int>(items[5]), // minute
            lexical_cast<int>(items[6])  // second
        );

        // check that time appears to be correct.
        BOOST_CHECK(message_second_expected == items[12]);
        BOOST_CHECK("inglenook.logging.test" == items[11]); // inglenook.logging.test is the fallback used (assigned by run_writer());
        BOOST_CHECK(log_category == (category)lexical_cast<int>(items[10]));
        BOOST_CHECK(log_data == items[13]);
    }
}

} // namespace inglenook::logging

} // namespace inglenook
