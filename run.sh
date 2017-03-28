#!/bin/bash

# Init checking
if [[ $# > 2 ]]; then
  printf You cannot have that many options.
  exit 1
elif [[ $# == 0 ]]; then
  printf "Use:\n    -cm or --cmake: runs cmake from build folder.\n    -mk or --make: runs make from build folder.\n    -ex or --exec: executes target from this directory.\n    -cl or --clean: cleans build directory. If added as a second option the it will also clean the build directory before executing the action."
  exit 1
fi

# Variables
BUILD_DIR="build"
EXEC_FOLDER="examples"
TARGET="game"
OPT1=$1
OPT2=$2

# Functions
function cmake_func {
  if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
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
  elif [ ! -f "$BUILD_DIR"/Makefile ]; then
    cd $BUILD_DIR
    cmake ..
    make
  elif [ -f "$BUILD_DIR"/Makefile ]; then
    cd $BUILD_DIR
    make
  fi

  cd $EXEC_FOLDER
  ./"$TARGET"
}

function clean_func {
  if [ -d "$BUILD_DIR" ]; then
    rm -rf $BUILD_DIR
  fi
}

# Args evalutation
case $OPT1 in
  -cm|--cmake)
    MODE=CMAKE # Invoke cmake
    shift
    ;;
  -mk|--make)
    MODE=MAKE # Invoke make
    shift
    ;;
  -ex|--exec)
    MODE=EXEC # Execute program
    shift
    ;;
  -me)
    MODE=ME # Make and Exec
    shift
    ;;
  -cl|--clean)
    MODE=CLEAN # Clean build directory
    shift
    ;;
  *)
    printf "Invalid first option.\nUse:\n    -cm or --cmake: runs cmake from build folder.\n    -mk or --make: runs make from build folder.\n    -ex or --exec: executes target from this directory.\n    -cl or --clean: cleans build directory. If added as a second option the it will also clean the build directory before executing the action."
    exit 1
    ;;
esac

if [[ -n $OPT2 ]] && [[ $OPT1 != $OPT2 ]]; then
  case $OPT2 in
    -cl|--clean)
      clean_func
      shift
      ;;
    *)
      printf "Invalid second option. Ignoring...\n"
      ;;
  esac
fi

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
  CLEAN)
    clean_func
    shift
    ;;
esac

