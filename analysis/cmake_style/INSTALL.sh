#!/bin/bash

# error flag
ERROR_FLAG=0


#Set some colours for the error messages
CYAN="\033[36m"
END="\033[0m"

#This is where we handle arguments passed to the script
#0  Re-build programs and libraries that have been modified since last build
#1  Complete re-build -- remove everything from the build directory and start over
#99 De-bugging - build verbosely
MODE=$1
if [ -z "$1" ]; then
    echo "Usage " $0 " 0 <re-build modified bin / libs> 1 <re-build all>"
    ERROR_FLAG=1
fi

# we need to check for AraRoot, which is a dependency
if [ -z "$ARA_ROOT_DIR" ]; then
    echo -e $CYAN "\tError: Need to set ARA_ROOT_DIR environmental variable"
    echo -e "\t\tThis should point to where the source code for AraRoot is located" $END
    ERROR_FLAG=1
fi

#Handling the exit status from before
if [ $ERROR_FLAG == 1 ]; then
    exit 1
fi
mkdir -p ./build

# if recompiling from scratch, clean up everything first
if [ $MODE == 1 ] || [ $MODE == 99 ] ; then
    rm -rf ./build/*
fi

cd build
cmake ../

if [ $MODE == 99 ] ; then
    make
    VERBOSE=1
else
    make
fi

make
if [ $? -eq 0 ] ; then
    echo "build - success "
    make install
else
    echo "build - fail "
fi