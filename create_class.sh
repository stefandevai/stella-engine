#!/bin/bash

echo "Type the name of your class with_underscores"

read class_name

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

