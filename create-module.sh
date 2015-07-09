#!/bin/sh

# Check if the module name was given as command line argument
if [ -z $1 ]; then
  echo "Usage: create-modules.sh <module-name>"
  exit 1
fi

# Module's name (e.g. test_module)
module=$1
# Generate an uppercase version for header include guards (e.g. TEST_MODULE)
moduleUpper=$(echo $1 | tr '[:lower:]' '[:upper:]')
# Generate a camelcase version for the class name (e.g. TestModule)
moduleCamel=$(echo $1 | sed -e 's/_\([a-z]\)/\u\1/g' -e 's/^[a-z]/\u&/')

cd external/modules
mkdir -p "$module"
cd "$module"
mkdir -p src
mkdir -p include

cat > CMakeLists.txt << EOF
set(SOURCES
	"src/${module}.cpp"
	"src/interface.cpp"
)

set(HEADERS
	"include/${module}.h"
)

include_directories(include)
add_library($module MODULE \${SOURCES} \${HEADERS})
target_link_libraries($module PRIVATE lms)
EOF

cat > README.md << EOF
# $module

## Data channels

## Config

## Dependencies
EOF

cd include

cat > "${module}.h" << EOF
#ifndef ${moduleUpper}_H
#define ${moduleUpper}_H

#include <lms/datamanager.h>
#include <lms/module.h>

class $moduleCamel : public lms::Module {
public:
    bool initialize();
    bool deinitialize();
    bool cycle();
};

#endif // ${moduleUpper}_H
EOF

cd ../src

cat > "${module}.cpp" << EOF
#include "${module}.h"

${moduleCamel}::initialize() {
    return true;
}

${moduleCamel}::deinitialize() {
    return true;
}

${moduleCamel}::cycle() {
    return true;
}
EOF

cat > "interface.cpp" << EOF
#include "${module}.h"

extern "C" {
void* getInstance () {
    return new ${moduleCamel}();
}
}
EOF
