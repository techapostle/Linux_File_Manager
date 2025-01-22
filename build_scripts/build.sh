# This script builds the project

# get the directory of the script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# change to the scripts directory
cd $DIR

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

# return to the previous directory
cd -