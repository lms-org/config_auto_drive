#!/bin/sh

# Check if the library name was given as command line argument
if [ -z $1 ]; then
  echo "Usage: create-library.sh <library_name>"
  exit 1
fi

# Library's name (e.g. test_lib)
library=$1

cd external/libraries

if [ -e "$library" ]; then
  echo "Library directory is already existing: $library"
  exit 1
fi

mkdir "$library"
cd "$library"
mkdir src
mkdir include

cat > CMakeLists.txt << EOF
set(SOURCES
# src/file.cpp
)

set(HEADERS
# include/${library}/file.h
)

include_directories(include)
add_library($library SHARED \${SOURCES} \${HEADERS})
target_link_libraries($library PRIVATE lmscore)
EOF

cat > README.md << EOF
# $library

## Dependencies
EOF

cd include
touch CMakeSharedHeaders.txt
mkdir "$library"
