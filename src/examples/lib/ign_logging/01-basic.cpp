/*
 * 01-basic.cpp: Main software entry point for logging demonstrator.
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

// inglenook includes
#include <ign_logging/logging.h>	// this header is required for all logging.
#include "version.h"

/**
 * Main software entry point
 * This program demonstrates on the most simple level, how to use the logging library,
 * note that code in this sample does not conform to usual release standards as it has
 * been simplified to make the sample as clear as possible.
 * @param arg_c number of command line arguments.
 * @param arg_v character array delimited software arguments
 */
int main(int arg_c, char* arg_v[])
{

	// this simplifies accessing the logging interface (ilog).
	using namespace inglenook::logging;
	using namespace inglenook;

	// every application that requires logging, or that uses libraries that utilize logging must first call
	// initialize logging. it is the applications responsibility to do this, and it should be done as soon
	// as possible to ensure it is available.
	initialize_logging();

	// the easiest way to use the logging system is to use the stream operators.
	log_info() << "Hello World" << lf::end;

	// note in the above line we are writing to the standard information log. in addition,
	// you can write to the follow other log types
	log_debug()		<< "Debugging messages should be written to log_debug()" 		<< lf::end;
	log_trace() 	<< "Verbose messages should be written to log_trace()" 			<< lf::end;
	log_warning() 	<< "Warning messages should be written to log_warning()" 		<< lf::end;
	log_error() 	<< "Non-fatal error messages should be written to log_error()" 	<< lf::end;
	log_fatal()		<< "Fatal error messages should be written to log_fatal()" 		<< lf::end;

	// IMPORTANT: be aware that the logs are filtered, as such despite all the above messages
	//   being sent to the log manager, they may not be actually written to disk, or displayed
	//   on the screen. The thresholds at which a log is written to disk, or printed to the
	//   screen can be set independently.

	// All messages are finished with an lf::end, this means you can break entries up over lines.
	// (note: this is thread safe, no additional thread safety work is required on your behalf).
	int a = 1, b = 2;
	log_info() << "The value of a + b is ";
	int c = a + b;
	log() << c << lf::end;

	// Note: when we write the answer to the log (the c variable), because the category is already
	//    set to info() we do not need to specify this again.

	// IMPORTANT: technically no category needs to be assigned however a strongly recommended inglenook
	//	  convention to always start a log entry with the category set as it make the code easier to read.
	//    if you don't set a category at all a default one will be assigned.

	// you can also use this behaviour to change the log category, the following message will be
	// written to the log as an error entry because the last write was to the error category.
	log_info() << "Attempting to remove cute from Kittens: ";
	log_error() << "Don't be ridiculous" << lf::end;

	// every message we have written has not been namespaced. inglenook groups messages in namespaces
	// to help group associated logs. We can specify the namespace of an log entry with the ns()
	// stream manipulator. The following example writes to the examples.logging.01 namespace
	log_info() << ns("examples.logging.01") << "A well mannered \"Hello World\"" << lf::end;

	// obviously, this will get tedious if we do this on every message, most functionality should
	// be grouped within a given context. As such we can set a default namespace with:
	ilog->default_namespace("examples.logging.01.default");

	// Note: the ilog (log_client) default namespace is thread specific (and as such thread safe).
	//    It has to be set per thread. If you do not do this, messages are written to the log_output
	//    (log_writer) default namespace. By default log_output's default namespace is
	//    "inglenook.anonymous". Because of this it is often more convient to reset the writers default
	//    namespace instead, this can be done as follows:
	log_output->default_namespace("examples.logging.01");

	// IMPORTANT: the default namespace on the log_writer is NOT thread safe. It should be set once,
	//    immediately when the program is started. Modifification of this variable after this point
	//    is strongly discouraged (I know, I have here), as in a multithreaded environment, the
	//    result of reassigning this variable is undefined.

	// IMPORTANT: namespaces are NOT sanitized when written to disk. As such they should never be
	//    used in such a way in which user input is able to dictate the namespace text. This could
	//    be used to compromise the systems security.

	// finally, sometimes it is nice to attach a little more context to a log. To support this we have
	// the log_data() stream manipulator.
	log_warning() << "This is a 'levels of something' warning!"
				   << log_data("level of something", "17,000")
				   << log_data("acceptable level",   "15,000")
				   << lf::end;

	// Note: data and values can only be strings currently. Sorry, you'll have to stringify anything you need
	//      yourself.

	// IMPORTANT: the log_data key (first parameter) is NOT sanitized when written to disk. As such it
	//    should never be used in such a way in which user input is able to dictate the key text. This could
	//    be used to compromise the systems security.

	// finally; if for some reason you wish to manually build a log entry. you can, this is how:
	auto le = std::shared_ptr<log_entry>(new log_entry());
	le->message("This is a hand crafted, finely made log message.");
	le->log_namespace("examples.logging.01");
	le->extended_data("Hand made", "You bet");
	le->extended_data("Why", "Who knows");
	le->entry_type(category::information);
	log_output->add_entry(le);

	// this ends the quick introduction to the basics of logging.
	// if you run this; you might want to delete the log file
	// hint: /var/log/inglenook/[app-name]/
	//

}
