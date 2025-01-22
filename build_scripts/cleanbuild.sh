# This script is used to clean the build directory and rebuild the project

# get the directory of the script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# change to the scripts directory
cd $DIR

# Remove the build directory (../build) if it exists
if [ -d "../build" ]; then
    rm -rf ../build
fi

# Create the build directory
mkdir ../build

# Change to the build directory
cd ../build

# Run cmake to generate the build files
cmake ..

# Build the project
make

# return to the previous directory
cd -