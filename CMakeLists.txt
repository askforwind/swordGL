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
    ./src/shader.h
    ./src/Batch.h
    ./src/Batch.cpp
    ./src/BatchManager.h
    ./src/BatchManager.cpp
    ./src/Math.h
    ./src/Math.cpp
    ./src/Root.h
    ./src/Root.cpp
    ./src/Texture.cpp
    ./src/Texture.h
	./src/config.h
    ./dependence/LogManager.cpp
	./src/Camera.h
	./src/Camera.cpp
	./src/Node.h
	./src/Node.cpp
	./src/FPSCamera.h
	./src/FPSCamera.cpp
	./src/ResourceDef.h
	./src/Skeleton.h
	./src/Skeleton.cpp
	./src/FreeImageCodec.h
	./src/FreeImageCodec.cpp
	./src/ResourceGroup.h
	./src/ResourceGroup.cpp
	./src/ResourceDef.h
	./src/AssimpCodec.h
	./src/AssimpCodec.cpp
	./src/Timer.h)


set(CHECK_OPENGL_FUNCTION 1)

set(SWORD_RELEASE 0)

ADD_DEFINITIONS(-DSWORD_BUILD_SHARE)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/Dependence/include/precompile.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/Dependence/include/precompile.h"
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Dependence/lib)

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Dependence/include)

message(${CMAKE_LIBRARY_PATH})

find_package(log4cplus REQUIRED)
find_package(SDL REQUIRED )
find_package(FreeImage REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependence/include)

set(LIBS ${LIBS} ${LOG4CPLUS_LIBRARY} ${SDL_LIBRARY} ${FREEIMAGE_LIBRARY}
		${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${ASSIMP_LIBRARY} ${SDLMAIN_LIBRARY})

add_executable(sword  ${SRC})
target_link_libraries(sword  ${LIBS})
