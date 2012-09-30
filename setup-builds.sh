#
# setup-builds.sh: Helper script for setting up build directories for debug and release binaries.
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

# Clean up previous builds
rm -rf build

# Make new build directories
mkdir -p build/debug
mkdir -p build/release

# Setup debug
cd build/debug;
cmake ../.. -DCMAKE_BUILD_TYPE=debug

# Setup release
cd ../release
cmake ../.. -DCMAKE_BUILD_TYPE=release
