# This script removes all the build artifacts from the project.

# get the directory of the script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# change to the scripts directory
cd $DIR

# if ../build directory exists, remove it
if [ -d "../build" ]; then
    rm -rf ../build
fi

# return to the previous directory
cd -