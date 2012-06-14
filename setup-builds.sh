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
