cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)

# first, we set up some paths and directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# you may additionally need to specify your fftw associated libraries
# set(FFTW_LIBRARIES "$ENV{PLATFORM_DIR}/lib/libfftw3.so.3.4.4")
# set(FFTW_INCLUDES "$ENV{PLATFORM_DIR}/include")

# now, we define our project
project(MyAnalysis)
find_package(ROOT REQUIRED COMPONENTS MathMore Gui)
find_package(libRootFftwWrapper REQUIRED)
find_package(sqlite3 REQUIRED)
find_package(zlib REQUIRED)
find_package(AraEvent REQUIRED)

# set up directories to include
Set(INCLUDE_DIRECTORIES ${ARAEVENT_INCLUDES} ${LIBROOTFFTWWRAPPER_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})
include_directories( ${INCLUDE_DIRECTORIES} )

# define our executables
add_executable(
	exampleAnalysis
	${SRC_DIR}/exampleAnalysis.cxx
)
# linking
target_link_libraries(
	exampleAnalysis 
	${ARAEVENT_LIBRARIES} 
	${LIBROOTFFTWWRAPPER_LIBRARIES} 
	${ROOT_LIBRARIES} ${ZLIB_LIBRARIES}
)
# installation
install(
	TARGETS exampleAnalysis
	DESTINATION ${BIN_DIR}/
)
