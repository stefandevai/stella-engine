#!/bin/bash

# Init checking
if [[ $# > 1 ]]; then
  printf You cannot have that many options.
  exit 1
elif [[ $# == 0 ]]; then
  printf "Use:\n    -c or --cmake: run cmake from build folder.\n    -m or --make: run make from build folder.\n    -e or --exec: execute target from this directory.\n"
  exit 1
fi

# Variables
BUILD_DIR="build"
TARGET="stella"
OPT=$1

# Functions
function cmake_func {
  if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
  fi
  
  cd $BUILD_DIR
  cmake ..
}

function make_func {
  if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    cmake ..
  elif [ ! -f "$BUILD_DIR"/Makefile ]; then
    cd $BUILD_DIR
    cmake ..
  else
    cd $BUILD_DIR
  fi

  make
}

function exec_func {
  if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    cmake ..
    make
    cd ..
  elif [ ! -f "$BUILD_DIR"/Makefile ]; then
    cd $BUILD_DIR
    cmake ..
    make
    cd ..
  fi

  ./"$BUILD_DIR"/"$TARGET"
}

# Args evalutation
case $OPT in
  -c|--cmake)
    MODE=CMAKE
    shift
    ;;
  -m|--make)
    MODE=MAKE
    shift
    ;;
  -e|--exec)
    MODE=EXEC
    shift
    ;;
  -me)
    MODE=ME # Make and Exec
    shift
    ;;
  *)
    printf "Invalid option.\nUse:\n    -c or --cmake: run cmake from build folder.\n    -m or --make: run make from build folder.\n    -e or --exec: execute target from this directory.\n"
    exit 1
    ;;
esac

case $MODE in
  CMAKE)
    cmake_func
    shift
    ;;
  MAKE)
    make_func
    shift
    ;;
  EXEC)
    exec_func
    shift
    ;;
  ME)
    make_func
    cd ..
    exec_func
    shift
    ;;
esac

