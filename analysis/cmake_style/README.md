# cmake_style

## Quick Start
This is the simple CMake style version of an analysis. It is slightly more complicated.

Type `bash INSTALL.sh 0`. Then you can run the executable like`./bin/exampleAnalysis /path/to/L0/file.root`.

## Adding your own code

The executables you would like to compile should be stored in `src`. The compiled executables will be placed in `bin`.

To add your own executable, you should follow the style of `exampleAnalysis.cxx` in the `CMakeLists.txt` file. For example, if you prepared some code in the file `YourExecSourceFile.cxx` and would like to compile it into an executable called `NameOfYourExec`, you would:

1. Add your executable: 

   `add_executable NameOfYourExec ${SRC_DIR}/YourExecSourceFile.cxx`

2. Set the linking:

   `$target_link_libraries( NameOfYourExec ${ARAEVENT_LIBRARIES} ${LIBROOTFFTWWRAPPER_LIBRARIES} ${ROOT_LIBRARIES} ${ZLIB_LIBRARIES}`

3. Set the installation:

   `install(TARGETS NameOfYourExec DESTINATION ${BIN_DIR}/`

We have tried to make the build process easy by providing a shell script. You can just run `bash INSTALL.sh 0`. You can also run the build with `1` or `99` as arguments. If you call the `1`, it will rebuild everything, where the `0` will only rebuild if changes have been made to the source files. `99` will perform a recomile like `1`, but do so more verbosely.

Note that this is "easier" than the steps to normally compile a CMake project, where you would go into a build directory, type `cmake ../`, then `make`.