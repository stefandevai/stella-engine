#!/usr/bin/env python

import argparse
import shutil
import os

BUILD_DIR = 'build'
TARGET_DIR = 'example'
TARGET = 'example'

def build(include_editor, is_production):
    old_cwd = os.getcwd()

    build_path = f'{old_cwd}/{BUILD_DIR}'

    if os.path.isdir(build_path) == False:
        os.mkdir(build_path)

    os.chdir(build_path)
    os.system('cmake -DBUILD_EDITOR={} -DSTELLA_DEBUG_MODE={} ..'.format('ON' if include_editor else 'OFF', 'OFF' if is_production else 'ON'))
    os.system('make -j 4')
    os.chdir(old_cwd)

def execute():
    old_cwd = os.getcwd()

    build_path = f'{old_cwd}/{BUILD_DIR}'
    target_path = f'{build_path}/{TARGET_DIR}'
    imgui_path = f'{target_path}/config/imgui.ini'

    if os.path.isdir(build_path) == False:
        build(True, False)

    if os.path.exists(imgui_path):
        os.remove(imgui_path)

    os.chdir(target_path)
    os.system(f'./{TARGET}')
    os.chdir(old_cwd)

def clean():
    cwd = os.getcwd()
    build_path = f'{cwd}/{BUILD_DIR}'
    cmake_cache_path = f'{build_path}/CMakeCache.txt'
    target_dir_path = f'{build_path}/{TARGET_DIR}'

    if not os.path.isdir(build_path):
        return

    if os.path.exists(cmake_cache_path):
        os.remove(cmake_cache_path)

    if os.path.isdir(target_dir_path):
        shutil.rmtree(target_dir_path)

def clean_all():
    cwd = os.getcwd()
    build_path = f'{cwd}/{BUILD_DIR}'

    if not os.path.isdir(build_path):
        return

    shutil.rmtree(build_path)

def format_code():
    os.system('find include/ -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format --verbose -i -style=file')
    os.system('find src/ -type f \( -iname "*.hpp" -or -iname "*.cpp" ! -iname "lemmatizer.cpp" \) | xargs clang-format --verbose -i -style=file')
    os.system(f'find {TARGET_DIR} -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format --verbose -i -style=file')
    os.system('find test/ -type f \( -iname "*.h" -o -iname "*.cpp" ! -iname "catch.cpp" \) | xargs clang-format --verbose -i -style=file')

def main():
    parser = argparse.ArgumentParser(description='Stella Engine development scripts.')

    parser.add_argument('--build', '-b', action='store_true', help='build example')
    parser.add_argument('--no-editor', '-n', action='store_false', help='build example without the editor')
    parser.add_argument('--production', '-p', action='store_true', help='build an optimized version of the example')
    parser.add_argument('--execute', '-e', action='store_true', help='execute example')
    parser.add_argument('--clean', '-c', action='store_true', help='clean cmake cache and assets')
    parser.add_argument('--clean-all', action='store_true', help='delete build directory')
    parser.add_argument('--format', '-f', action='store_true', help='format code using clang-format')

    args = parser.parse_args()

    if (args.clean):
        clean()

    if (args.clean_all):
        clean_all()

    if (args.build):
        build(args.no_editor, args.production)

    if (args.execute):
        execute()

    if (args.format):
        format_code()

if __name__ == '__main__':
    main()
