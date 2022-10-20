cmake_minimum_required(VERSION 3.14)
PROJECT(cmake-test)                       #设定工程名

# 设置用debug还是release模式。debug允许断点，而release更快
#set( CMAKE_BUILD_TYPE Debug )
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE 1)
# 设置编译选项
set(CXX_FLAGS
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    # -Wall
    # -Wextra
    # -Werror
    # -Wconversion
    # -Wno-unused-parameter
    # -Wold-style-cast
    # -Woverloaded-virtual
    # -Wpointer-arith
    # -Wshadow
    # -Wwrite-strings
    -march=native
    -std=c++17
    -rdynamic
    -pthread
)
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O2 -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#设定可执行二进制文件的目录
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

#设定存放编译出来的库文件的目录
SET( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) 

#设定ctp运行目录 如果没有则新建
SET( CTP_DIR ${PROJECT_SOURCE_DIR}/bin/ctp)
execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CTP_DIR})

SET(DEPS_DIR ${PROJECT_SOURCE_DIR}/deps)

#设定头文件目录
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES( ${DEPS_DIR}/include)
#并且把该目录设为连接目录
LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/lib)
LINK_DIRECTORIES( ${DEPS_DIR}/lib)

# 
find_package (Threads REQUIRED)
# target_link_libraries(main PRIVATE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)

#增加子文件夹(注意我们的src路径是通过子文件夹的形式添加的，在这里面也是可以些CMakeLists.txt的)
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/src)
# 或则是ADD_SUBDIRECTORY( src)

ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/include/pubsub/test)