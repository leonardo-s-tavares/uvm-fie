#!/bin/bash

echo Creating the build directory!
cmake -E make_directory build
cd build
echo Generating makefiles!
cmake -DBUILD_STATIC_LIBS=ON -DSC_SIGNAL_WRITE_CHECK=DISABLE -DSC_DEFAULT_WRITER_POLICY=SC_MANY_WRITERS ..
echo Building the application!
make

# https://stackoverflow.com/questions/39640212/sc-signalt-cannot-have-more-than-one-driver-error-systemc
