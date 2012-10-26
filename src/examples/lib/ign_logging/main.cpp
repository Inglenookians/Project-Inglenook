/*
 * main.cpp: Main software entry point for logging demonstrator.
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

// standard library includes
#include <stdlib.h>
#include <string>
#include <sstream>

// boost (http://boost.org) includes
#include <boost/locale.hpp>
#include <boost/exception/all.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

// inglenook includes
//#define INGLENOOK_LOG_FILE "/dev/null"
#include "../common/logging.h"

/**
 * Log testing thread.
 * This thread is designed to test the capabilities of the log_client object.
 * It is designed to be run as a thread, with several siblings. This tests thread safety and load capacities.
 * @param id thread id (index) within starting array.
 * @param no_outputs character array delimited software arguments
 */
void loud_thread(int id, int no_outputs)
{
	// this simplifies accessing the logging interface (iout).
	using namespace inglenook::logging;
	using namespace inglenook;

	// change the thread specific default category to warning.
	ilog.default_entry_type(category::warning);

	// notify callers that we have started.
	ilog.info() << "thread #" << id << " started." << lf::end;

	// spam a few messages out to the log.
	for(int i = 0; i < no_outputs; i++)
	{
		// write the message, lots of places for bad thread safety to show...
		ilog << "this is message #" << (i+1) << " of " <<
				no_outputs << " from thread " << id << lf::end;
	}

	// notify thread complete (broken up a bit to test).
	ilog.info() << "thread #" << id;
	ilog << " has completed!";
	ilog << lf::end;

}

/**
 * Logging daemon software entry point.
 * Execution starts at this method. Everything the daemon is begins here.
 * @param arg_c number of command line arguments.
 * @param arg_v character array delimited software arguments
 */
int main(int arg_c, char* arg_v[])
{
	// configure some local name spaces
	using namespace boost::locale;

	// software return code, this will usually be EXIT_SUCCESS but can be modified by
	// either the daemons return result, or the try/catch handler for this method.
	int application_return_code = EXIT_SUCCESS;

	try
	{

		// number of threads, and number of messages to spawn.
		const int NO_THREADS = 55; // more threads than queue space
		const int NO_MESSAGES = 100; // lots of messages...

		// thread collection
		std::shared_ptr<boost::thread> threads[NO_THREADS];

		// print out a small introduction block...
		std::cout << translate("This is an example of logging; and is for development and testing."                                                                  ) << std::endl;
		std::cout << translate("This demo shows the expected use of the inglenook logging and output mechanism (inglenook::ilog)"                                    ) << std::endl;
		std::cout << translate("Its is also a test of thread safety and severe load conditions. Launching ") << NO_THREADS << " / " << NO_MESSAGES                     << std::endl;
		std::cout << "================================================================================================"                                                << std::endl;
		std::cout << ""                                                                                                                                                << std::endl;

		using namespace inglenook;
		using namespace inglenook::logging;

		// modify the default name space (if you do not do this the name space "inglenook.anonymous" will be used.
		log_output->default_namespace("inglenook.logging.example");

		// example of building up a log entry manually
		auto le = std::shared_ptr<log_entry>(new log_entry());
		le->message("This message has some additional information tied in with it.");
		le->log_namespace("inglenook.logging.example.manual");
		le->extended_data("variable 1", "value 1");
		le->extended_data("variable 2", "value 2");
		le->entry_type(category::warning);
		log_output->add_entry(le);

		// same message built on a stream
		ilog.warning() << ns("inglenook.logging.example.stream")
				    << "This message has some additional information tied in with it"
				    << log_data("variable 1", "value 1") << log_data("variable 2", "value 2")
				    << lf::end;

		// start all the threads
		for(int i = 0; i < NO_THREADS; i++)
		{
			// create a new thread to spam logs with...
			threads[i] = std::shared_ptr<boost::thread>(
					new boost::thread(loud_thread, i, NO_MESSAGES));
		}

		// wait for all threads to re-join
		for(int i = 0; i < NO_THREADS; i++)
			threads[i]->join();

		// indicate end of main to ensure all log messages are flushed..
		std::cout << "<< end of main() >>" <<std::endl;

	}
	catch(...)
	{
		//
		// if we are catching here something has gone terribly wrong, we don't
		// know what's been thrown. nothing should ever be caught by this error handler
		// under normal execution paths.

		// write out the exception string
		auto error_string = translate("{1} has encountered a fatal exception. No additional information is available.");

		// write out some status information to the console regards this fault.
		//std::cout << format(error_string) % LogDaemon::SoftwareName << std::endl;
		std::cerr << boost::current_exception_diagnostic_information() << std::endl;

		// encountered exception - no longer running on success...
		application_return_code = EXIT_FAILURE;

	}


	// return the success of this method.
	return application_return_code;

}
