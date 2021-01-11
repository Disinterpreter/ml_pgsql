#!/bin/sh

MODE=$1
PLATFORM=$2

if [ -z $MODE ]; then
    MODE="debug"
fi;

if [ -z $PLATFORM ]; then
    PLATFORM="64"
fi;

# clean up prev. builds
rm -rf {Bin,Build}

utils/premake5 gmake2

cd Build

make config="${MODE}_x${PLATFORM}" all
