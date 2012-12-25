#! /bin/sh

#
# ign_logging.sh - bash script include file for logging interfaces
# Copyright (C) 2012, Project Inglenook (http://www.project-inglenook.co.uk)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

########################################

# initialize globals and create new log file....
LOG_NAMESPACE="inglenook.logging"
LOG_FILE=$(ign_log_write start -n `basename $0` -p $$)

########################################
# log()
# writes out a log message to the console and XML log file.
# $1 - OPTIONAL - message (Note: if no message provided - nothing will be emitted).
# $2 - OPTIONAL - category(Note: see "man ign_log_write" for number references).
########################################
function log()
{
    # default the category to information
    CATEGORY=3

    # check if another category has been specified
    if [ $# -gt 1 ]; then
        CATEGORY=$2;
    fi

    # write the message to the log file.
    eval "ign_log_write -a write -f $LOG_FILE -N $LOG_NAMESPACE -C $CATEGORY -m \"$1\""
}

########################################
# log_debug()
# writes a debug log out.
########################################
function log_debug()
{
    # write out the log message
    log "$1" 1
}

########################################
# log_verbose()
# writes a verbose log out.
########################################
function log_verbose()
{
    # write out a verbose log message
    log "$1" 2
}
########################################
# log_info()
# writes an informational log out.
########################################
function log_info()
{
    # write out the log message
    log "$1" 3
}

########################################
# log_warning()
# writes a warning log out.
########################################
function log_warning()
{
    # write out the log message
    log "$1" 4
}

########################################
# log_error()
# writes an error log out.
########################################
function log_error()
{
    # write out a error log message
    log "$1" 5
}

########################################
# log_fatal()
# writes an fatal log out.
########################################
function log_fatal()
{
    # write out a fatal log message
    log "$1" 6
}

########################################
# close_log_file()
# function to call to close the log
########################################
function close_log_file()
{
    # close the log file
    eval ign_log_write close $LOG_FILE
}

# setup exit trapping
trap close_log_file EXIT
