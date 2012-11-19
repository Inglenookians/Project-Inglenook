#! /bin/sh
. ./ign_logging.sh

#
# ign_log_write_sample.sh - bash script demonstrating logging from a script
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

# you don't have to set a namespace - if you do not the 
# default namespace of "inglenook.logging" will be applied
LOG_NAMESPACE="inglenook.examples.ign_log_write"

# writing our information the easy way
log "hello world from out script..."

# and as before - all the other varieties - in full
log_debug 	"I'm a debug message"		# NOTE: filtering will prevent these being shown. 
log_verbose 	"I'm a verbose message"		# NOTE: filtering will prevent these being shown.
log_info	"I'm a status message"
log_warning	"I'm a warning message"
log_error	"I'm an error message"
log_fatal	"Doh! I'm a fatal message"

# and thats all there is too it - log creation and cleanup are handled using auto-magic.
