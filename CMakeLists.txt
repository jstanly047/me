cmake_minimum_required(VERSION 3.16.3)
project(ME LANGUAGES CXX)

set(PROJ_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
set(MSG_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/message")
#set(MOCK_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/include")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy-3.9;-checks=*")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)

#******************************************************************************
# External Libraries
#*******************************************************************************
find_package(Protobuf REQUIRED)


############################################################################
#Build Core Libraries
############################################################################

file(GLOB MSG_SRC  ./src/message/*.cc)
add_library(msg STATIC ${MSG_SRC})
target_link_libraries(msg protobuf::libprotobuf)
target_include_directories(msg PUBLIC ${MSG_INC_DIR} ${Protobuf_INCLUDE_DIRS})

file(GLOB ME_SRC  ./src/me/*.cpp)
add_library(me STATIC ${ME_SRC})
target_include_directories(me PUBLIC ${PROJ_INC_DIR})

file(GLOB ME_SOCKET_SRC  ./src/me/socket/*.cpp)
add_library(me_socket STATIC ${ME_SOCKET_SRC})
target_include_directories(me_socket PUBLIC ${PROJ_INC_DIR} ${MSG_INC_DIR})
target_link_libraries(me_socket me msg)

file(GLOB THREAD_SRC  ./src/me/thread/*.cpp)
add_library(me_thread STATIC ${THREAD_SRC})
target_link_libraries(me_thread me_socket)

############################################################################
# Flags and links
############################################################################
add_definitions(-std=c++2a -DPROJECT_BASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" -m64 -Wno-int-to-pointer-cast -Wno-deprecated-declarations -fexceptions -DIL_STD -DILOUSEMT -D_REENTRANT -DILM_REENTRANT  -fstrict-aliasing -pedantic -Wall -Werror  -Wl,--no-as-needed -ldl)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS_DEBUG "-g ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

############################################################################
# Build Solutions
############################################################################
add_executable(MEClient
                  ./src/MEClient.cpp)
target_link_libraries(MEClient
                      me_socket)

add_executable(MEServer
                  ./src/MEServer.cpp)
target_link_libraries(MEServer
                      me_thread)


#target_link_libraries(BPMSEngine_${SOLUTION_LOWER})
