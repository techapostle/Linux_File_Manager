# This script builds the project

# if ../build directory does not exist, create it
if [ ! -d "../build" ]; then
    mkdir ../build
fi

# change to the build directory
cd ../build

# run cmake to configure the project
cmake ..

# build the project
make