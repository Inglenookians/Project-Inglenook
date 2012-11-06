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
#include <boost/exception/all.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

// inglenook includes
#include <ign_logging/logging.h>

/**
 * Log testing thread.
 * This thread is designed to test the capabilities of the log_client object under load and demonstrate the
 * simplicity of using the library with threads (no work required!)
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

	// notify caller that we have started.
	ilog.info() << "thread #" << id << " started." << lf::end;

	// spam a few messages out to the log.
	for(int i = 0; i < no_outputs; i++)
	{
		// write the message, lots of places for bad thread safety to show...
		ilog << "this is message #" << (i+1) << " of " <<
				no_outputs << " from thread " << id << lf::end;
	}

	// notify thread complete
	ilog.info() << "thread #" << id
			<< " has completed!" << lf::end;

}

/**
 * Threading logging samples entry point.
 * Execution starts at this method. Everything the daemon is begins here.
 * @param arg_c number of command line arguments.
 * @param arg_v character array delimited software arguments
 */
int main(int arg_c, char* arg_v[])
{

	// software return code, this will usually be EXIT_SUCCESS but can be modified by
	// either the daemons return result, or the try/catch handler for this method.
	int application_return_code = EXIT_SUCCESS;

	using namespace inglenook;
	using namespace inglenook::logging;

	// number of threads, and number of messages to spawn.
	const int NO_THREADS = 55; // more threads than queue space
	const int NO_MESSAGES = 100; // lots of messages...

	// thread collection
	std::shared_ptr<boost::thread> threads[NO_THREADS];

	// modify the default name space (if you do not do this the name space "inglenook.anonymous" will be used.
	log_output->default_namespace("inglenook.logging.example");

	// print out a small introduction block...
	ilog.info()  << "This is an example of logging; and is for demonstration purposes.";
	ilog << std::endl;
	ilog	 << "This demo shows the expected use of the inglenook logging and output mechanism (inglenook::ilog)"                     << std::endl
				 << "Its is also a test of thread safety and severe load conditions."                                                      << std::endl
				 << "Launching "  << NO_THREADS << " threads / " << NO_MESSAGES    << "message per thread" 								   << std::endl
				 << "================================================================================================"                     << std::endl
				 << ""                                                                                                                     << lf::end;

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
	ilog.info() << "The main() method has ended." << lf::end;

}
