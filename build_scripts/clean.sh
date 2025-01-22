# This script removes all the build artifacts from the project.

# if ../build directory exists, remove it
if [ -d "../build" ]; then
    rm -rf ../build
fi