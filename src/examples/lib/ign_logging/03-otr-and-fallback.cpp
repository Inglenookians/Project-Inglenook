/*
 * 03-otr-and-fallback.cpp: Main software entry point for otr-logging and fallback demonstrator.
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
#include <ign_logging/logging.h>
#include "version.h"

/**
 * OTR and fallback logging sample entry point.
 * Execution starts at this method. Everything the sample is begins here.
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

	// up until now all the examples of the library have demonstrated its use. the following line shows what happens when you
	// misuse the library by not initializing it, at all.
	log_info() << "What just happened?" << lf::end;
	
	// an "off the record" logger does not write to the hard disk. if you want to create one of these (without using the fallback
	// behaviour) you can do this by calling inglenook::logging::initialize_logging_off_record() instead of initialize_logging()

}
