#!/bin/bash

# Init checking
function print_options {
  printf "Use:\n    -m or --make: runs cmake from build folder.\n    -b or --build: builds program.\n    -e or --execute: executes target.\n    -c or --clean: cleans build directory. If added as a second option the it will also clean the build directory before executing the action.\n"
}

if [[ $# > 2 ]]; then
  printf "You cannot have that many options."
  exit 1
elif [[ $# == 0 ]]; then
	print_options
  exit 1
fi

# Variables
BUILD_DIR="build"
TARGET_DIR="examples"
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
	elif [ ! -f "$BUILD_DIR"/"$TARGET_DIR"/"$TARGET" ]; then
		cd $BUILD_DIR
		make
	else
		cd $BUILD_DIR
	fi

	cd $TARGET_DIR
	./"$TARGET"
}

function clean_func {
	case $1 in
		-c|--clean)
			if [ -d "$BUILD_DIR/$TARGET_DIR" ]; then
				rm -rf "$BUILD_DIR/$TARGET_DIR"
			fi
			shift
			;;
		-ca|--clean-all)
			if [ -d "$BUILD_DIR" ]; then
				rm -rf $BUILD_DIR
			fi
			shift
			;;
		*)
			printf "Invalid clean option.\n"
			print_options
			exit 1
			;;
	esac
}

# Args evalutation
case $OPT1 in
  -m|--make)
    MODE=CMAKE # Invoke cmake
    shift
    ;;
  -b|--build)
    MODE=MAKE # Invoke make
    shift
    ;;
  -e|--execute)
    MODE=EXEC # Execute program
    shift
    ;;
  -be)
    MODE=ME # Make and Exec
    shift
    ;;
  -c|--clean|-ca|--clean-all)
    MODE=CLEAN # Clean build directory
    shift
    ;;
  *)
    printf "Invalid option.\n"
  	print_options
    exit 1
    ;;
esac

if [[ -n $OPT2 ]] && [[ $OPT1 != $OPT2 ]]; then
  case $OPT2 in
    -c|--clean)
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
		clean_func $OPT1
    shift
    ;;
esac

