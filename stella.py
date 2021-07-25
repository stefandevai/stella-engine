#!/usr/bin/env python

import argparse
import shutil
import sys
import os

BUILD_DIR = 'build'
TARGET_DIR = 'example'
TARGET = 'example'

# ================================================================================
# Create class utils
# ================================================================================
def create(name, path):
    if path == 'include/editor/widgets':
        create_widget(name)
    else:
        create_class(path, name)

def create_widget(name):
    header_path = f'include/editor/widgets/{name}.hpp'
    source_path = f'src/editor/widgets/{name}.cpp'
    relative_header_path = header_path.replace('include/', '')
    capitalized_name = ''.join(x.title() for x in name.split('_'))
    spaced_name = ' '.join(x.title() for x in name.split('_'))

    header_boilerplate = ('#pragma once\n\n'
                          '#include \"widget.hpp\"\n\n'
                          'namespace stella\n'
                          '{\n'
                          'namespace widget\n'
                          '{\n\n'
                          f'  class {capitalized_name} : public Widget\n'
                          '  {\n'
                          '    public:\n'
                          f'      {capitalized_name} ();\n'
                          '      void render();\n\n'
                          '    private:\n'
                          '  };\n\n'
                          '} // namespace widget\n'
                          '} // namespace stella\n\n')

    source_boilerplate = (f'#include \"{relative_header_path}\"\n\n'
                          'namespace stella\n'
                          '{\n'
                          'namespace widget\n'
                          '{\n\n'
                          f'  {capitalized_name}::{capitalized_name} ()\n'
                          f'  : Widget ("{spaced_name}")\n'
                          '  {\n\n'
                          '  }\n\n'
                          f'  void {capitalized_name}::render ()\n'
                          '  {\n\n'
                          '  }\n\n'
                          '} // namespace widget\n'
                          '} // namespace stella\n\n')

    def write_class_to_file():
        with open(header_path, 'w') as f:
            f.write(header_boilerplate)
        with open(source_path, 'w') as f:
            f.write(source_boilerplate)
        print(f'Created a new WIDGET on \'{header_path}\' and \'{source_path}\'')

    if os.path.isfile(header_path) or os.path.isfile(source_path):
        print(f'Either {header_path} or {source_path} already exist.\nDo you want to override them?')
        answer = input("Answer: ")
        if answer == 'y':
            write_class_to_file()
        else:
            print('Okay.')
    else:
        write_class_to_file()

def create_class(path, name):
    if path[-1] != '/':
        path += '/'
    if path[1] == '/':
        path = path[1:]

    header_path = f'{path}{name}.hpp'
    source_path = path.replace('include', 'src') + f'{name}.cpp'
    relative_header_path = header_path.replace('include/', '')
    capitalized_name = ''.join(x.title() for x in name.split('_'))

    header_boilerplate = ('#pragma once\n\n'
                          'namespace stella\n'
                          '{\n'
                          'namespace \n'
                          '{\n\n'
                          f'  class {capitalized_name}\n'
                          '  {\n'
                          '    public:\n'
                          f'      {capitalized_name} ();\n'
                          '    private:\n'
                          '  };\n\n'
                          '} // namespace \n'
                          '} // namespace stella\n\n')

    source_boilerplate = (f'#include \"{relative_header_path}\"\n\n'
                          'namespace stella\n'
                          '{\n'
                          'namespace \n'
                          '{\n\n'
                          f'  {capitalized_name}::{capitalized_name} ()\n'
                          '  {\n\n'
                          '  }\n\n'
                          '} // namespace \n'
                          '} // namespace stella\n\n')

    def write_class_to_file():
        with open(header_path, 'w') as f:
            f.write(header_boilerplate)
        with open(source_path, 'w') as f:
            f.write(source_boilerplate)
        print(f'Created a new CLASS on \'{header_path}\' and \'{source_path}\'')

    if os.path.isfile(header_path) or os.path.isfile(source_path):
        print(f'Either {header_path} or {source_path} already exist.\nDo you want to override them?')
        answer = input("Answer: ")
        if answer == 'y':
            write_class_to_file()
        else:
            print('Okay.')
    else:
        write_class_to_file()

# ================================================================================
# Build utils
# ================================================================================
def build(include_editor, is_production):
    old_cwd = os.getcwd()

    build_path = f'{old_cwd}/{BUILD_DIR}'

    if os.path.isdir(build_path) == False:
        os.mkdir(build_path)

    os.chdir(build_path)
    os.system('cmake -DBUILD_EDITOR={} -DSTELLA_DEBUG_MODE={} ..'.format('ON' if include_editor else 'OFF', 'OFF' if is_production else 'ON'))
    os.system('make -j 4')
    os.chdir(old_cwd)

def execute(has_built):
    old_cwd = os.getcwd()

    build_path = f'{old_cwd}/{BUILD_DIR}'
    target_path = f'{build_path}/{TARGET_DIR}'
    imgui_path = f'{target_path}/config/imgui.ini'

    if os.path.isdir(build_path) == False:
        build(True, False)

    if os.path.exists(imgui_path):
        os.remove(imgui_path)

    # # Copy necessary support files
    # if not has_built:
    #     os.system(f'cp -r {TARGET_DIR}/assets {target_path}')
    #     os.system(f'cp -r {TARGET_DIR}/scenes {target_path}')
    #     os.system(f'cp -r {TARGET_DIR}/scripts {target_path}')
    #     os.system(f'cp {TARGET_DIR}/config.json {target_path}')

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

class CustomParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('Argparse error: %s\n' % message)
        self.print_help()
        sys.exit(2)

def main():
    parser = argparse.ArgumentParser(description='Stella Engine development scripts.')

    # Define options
    parser.add_argument('--build', '-b', action='store_true', help='build example')
    parser.add_argument('--no-editor', '-n', action='store_false', help='build example without the editor')
    parser.add_argument('--production', '-p', action='store_true', help='build an optimized version of the example')
    parser.add_argument('--execute', '-e', action='store_true', help='execute example')
    parser.add_argument('--clean', '-c', action='store_true', help='clean cmake cache and assets')
    parser.add_argument('--clean-all', action='store_true', help='delete build directory')
    parser.add_argument('--format', '-f', action='store_true', help='format code using clang-format')
    parser.add_argument('--create-class', nargs=2, metavar=('name', 'path'), help='create a class with a template')

    # Print help if no options were given
    if len(sys.argv) == 1:
      parser.print_help(sys.stderr)
      sys.exit(1)

    args = parser.parse_args()

    # Select action according to the options provided
    if args.create_class:
        create(args.create_class[0], args.create_class[1])

    if (args.clean):
        clean()

    if (args.clean_all):
        clean_all()

    if (args.build):
        build(args.no_editor, args.production)

    if (args.execute):
        execute(args.build)

    if (args.format):
        format_code()

    if os.path.exists(f'{BUILD_DIR}/compile_commands.json'):
        os.system(f'cp {BUILD_DIR}/compile_commands.json ./')

if __name__ == '__main__':
    main()
