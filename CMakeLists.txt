#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
set(CMAKE_CXX_FLAGS_DEBUG 
    "${CMAKE_CXX_FLAGS_DEBUG} -g -Weverything -DDEBUG -Wno-c++98-compat")
set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -DNDEBUG -Wno-c++98-compat")

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
    ./src/Texture.h)



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_part/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_part/build/)

add_executable(newWin  ${SRC})
target_link_libraries(newWin WindLog GL X11 GLU log4cplus SDL2 GLEW freeimage)
