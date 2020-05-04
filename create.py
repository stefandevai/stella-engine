#!/bin/env python

import argparse
import os.path

def create(path, name):
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

    header_boilerplate = ('#pragma once\n'
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
        print(f'Created new WIDGET on \'{header_path}\' and \'{source_path}\'')

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

    header_boilerplate = ('#pragma once'
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
        print(f'Created new WIDGET on \'{header_path}\' and \'{source_path}\'')

    if os.path.isfile(header_path) or os.path.isfile(source_path):
        print(f'Either {header_path} or {source_path} already exist.\nDo you want to override them?')
        answer = input("Answer: ")
        if answer == 'y':
            write_class_to_file()
        else:
            print('Okay.')
    else:
        write_class_to_file()


parser = argparse.ArgumentParser()
parser.add_argument('-p', '--path', help='header path of the new class')
parser.add_argument('-n', '--name', help='name of the class')
args = parser.parse_args()

if args.path and args.name:
    create(args.path, args.name)
else:
    parser.print_help()
