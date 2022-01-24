# CMakeLists.txt

# Older versions of CMake are likely to work just fine but, since
# I don't know where to cut off I just use the version I'm using
cmake_minimum_required(VERSION "3.17")

# name of this example project
project(zhang)

# set OpenCV_DIR variable equal to the path to the cmake
# files within the previously installed opencv program
set(OpenCV_DIR /usr/local/Cellar/opencv/4.5.4_3/lib/cmake/opencv4)

# Tell compiler to use C++ 14 features which is needed because
# Clang version is often behind in the XCode installation
set(CMAKE_CXX_STANDARD 14)

# configure the necessary common CMake environment variables
# needed to include and link the OpenCV program into this
# demo project, namely OpenCV_INCLUDE_DIRS and OpenCV_LIBS
find_package(OpenCV REQUIRED)

# tell the build to include the headers from OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# specify the executable target to be built
add_executable(zhang zhang.cpp)
add_executable(zhang_optimized zhang_optimized.cpp)
add_executable(zhang_threads zhang_threads.cpp)


# tell it to link the executable target against OpenCV
target_link_libraries(zhang ${OpenCV_LIBS})
target_link_libraries(zhang_threads ${OpenCV_LIBS})
target_link_libraries(zhang_optimized ${OpenCV_LIBS})