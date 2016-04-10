cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE Debug)

PROJECT(swordGL)

if(Unix)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
set(CMAKE_CXX_FLAGS_DEBUG 
   "${CMAKE_CXX_FLAGS_DEBUG} -g -Weverything -DDEBUG -Wno-c++98-compat")
set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -DNDEBUG -Wno-c++98-compat")
endif()

set(SRC ./src/Platform.h
    ./src/main.cpp
    ./src/RenderWindow.h
    ./src/RenderWindow.cpp
    ./src/shader.cpp
    ./src/shader.hpp
    ./src/Application.h
    ./src/Application.cpp
    ./src/Batch.h
    ./src/Batch.cpp
    ./src/AppInputHandle.cpp
    ./src/BatchManager.h
    ./src/BatchManager.cpp
    ./src/Math.h
    ./src/Math.cpp
    ./src/Root.h
    ./src/Root.cpp
    ./src/Texture.cpp
    ./src/Texture.h
    ./dependence/LogManager.cpp)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Dependence/lib)

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Dependence/include)

message(${CMAKE_LIBRARY_PATH})

find_package(log4cplus REQUIRED)
find_package(SDL REQUIRED )
find_package(FreeImage REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependence/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependence)

set(LIBS ${LIBS} ${LOG4CPLUS_LIBRARY} ${SDL_LIBRARY} ${FREEIMAGE_LIBRARY}
		${GLEW_LIBRARY} ${OPENGL_LIBRARY})

add_executable(sword  ${SRC})
target_link_libraries(sword  ${LIBS})
