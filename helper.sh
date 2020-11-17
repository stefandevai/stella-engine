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
TARGET_DIR="nikte2"
TARGET="nikte2"
OPT1=$1
OPT2=$2

# Functions
# Runs CMake
function cmake_func {
  if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
  fi
  
  cd $BUILD_DIR
  cmake ..
}

# Runs cmake and make
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

  make -j 4
}

# Execute target
function exec_func {
	if [ ! -d "$BUILD_DIR" ]; then
		mkdir $BUILD_DIR
		cd $BUILD_DIR
		cmake ..
		make -j 4
	elif [ ! -f "$BUILD_DIR"/Makefile ]; then
		cd $BUILD_DIR
		cmake ..
		make -j 4
	elif [ ! -f "$BUILD_DIR"/"$TARGET_DIR"/"$TARGET" ]; then
		cd $BUILD_DIR
		make -j 4
	else
		cd $BUILD_DIR
	fi

	cp -r ../nikte2/assets example

	cd $TARGET_DIR
	./"$TARGET"
}

# Cleans build dir partially or completely
function clean_func {
	case $1 in
		-c|--clean)
			if [ -d "$BUILD_DIR/$TARGET_DIR" ]; then
				rm -rf "$BUILD_DIR/$TARGET_DIR"
				rm -rf "$BUILD_DIR/assets"
			fi
			shift
			;;
		-ca|--clean-all)
			if [ -d "$BUILD_DIR" ]; then
				rm -rf $BUILD_DIR/example
				rm -rf $BUILD_DIR/nikte2
				rm -rf $BUILD_DIR/src
				rm -rf $BUILD_DIR/test
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

# Creates the boilerplate for a cpp class
function create_class {
	if [[ -n $OPT2 ]]; then
		class_name=$OPT2
	else
		echo "Type the name of your class with_underscores"
		read class_name
	fi

	class_camel_name=""
	names=$(echo $class_name | tr "_" " ")

	for name in $names
	do
		name="$(tr '[:lower:]' '[:upper:]' <<< ${name:0:1})${name:1}"
		class_camel_name="$class_camel_name$name"
	done

	echo "#pragma once

class ${class_camel_name} {
  public:
    ${class_camel_name}();
    ~${class_camel_name}();
};
" > "$class_name.h"


	echo "#include \"${class_name}.h\"

${class_camel_name}::${class_camel_name}() {

}

${class_camel_name}::~${class_camel_name}() {

}
" > "$class_name.cpp"
}

# Formats cpp code using clang-format
function format_code {
	find include/ -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format --verbose -i -style=file
	find src/ -type f \( -iname "*.hpp" -or -iname "*.cpp" ! -iname "lemmatizer.cpp" \) | xargs clang-format --verbose -i -style=file
	find nikte2/ -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format --verbose -i -style=file
  find test/ -type f \( -iname "*.h" -o -iname "*.cpp" ! -iname "catch.cpp" \) | xargs clang-format --verbose -i -style=file
}

function copy_assets {
	cp -r nikte2/assets $BUILD_DIR/nikte2
}

function copy_scripts {
	cp -r nikte2/scripts $BUILD_DIR/nikte2
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
    MODE=EXECUTE # Execute program
    shift
    ;;
  -be)
    MODE=BE # Make and Exec
    shift
    ;;
  -c|--clean|-ca|--clean-all)
    MODE=CLEAN # Clean build directory
    shift
    ;;
  --create-class)
  	MODE=CREATE_CLASS
  	shift
		;;
	-f|--format-code)
		MODE=FORMAT_CODE # Formats cpp code using clang-format
		shift
		;;
	-cp|--copy-assets)
		MODE=COPY_ASSETS
		shift
		;;
  -bt|--build-tests)
    TARGET_DIR="test"
    TARGET="tests"
    MODE=MAKE # Invoke make
    shift
    ;;
  -rt|--run-tests)
    TARGET_DIR="test"
    TARGET="tests"
    MODE=EXECUTE # Execute program
    shift
    ;;
  -t|--test)
    TARGET_DIR="test"
    TARGET="tests"
    MODE=BE # Make and Exec
    shift
    ;;
  *)
    printf "Invalid option.\n"
  	print_options
    exit 1
    ;;
esac

if [[ -n $OPT2 ]] && [[ $OPT1 != $OPT2 ]] && [[ $MODE != CREATE_CLASS ]]; then
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
  EXECUTE)
		copy_scripts
    exec_func
    shift
    ;;
  BE)
    make_func
    cd ..
    exec_func
    shift
    ;;
  CLEAN)
		clean_func $OPT1
    shift
    ;;
	CREATE_CLASS)
		create_class
		shift
		;;
	FORMAT_CODE)
		format_code
		shift
		;;
	COPY_ASSETS)
		copy_assets
		copy_scripts
		shift
		;;
esac
