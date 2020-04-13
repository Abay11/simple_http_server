CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

project(HTTP_SERVER)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    message(STATUS "System detected as Windows. Trying to determine compiler path...")
    set(BOOST_INCLUDEDIR "")

endif()

find_package(Boost REQUIRED COMPONENTS 
    system
)

IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)


add_executable(HTTP_SERVER main.cpp
Logger.h
HttpServer.cpp
HttpServer.h
 Request.cpp
 Request.h
 Response.cpp
 Response.h
 Slave.cpp
 Slave.h
)

target_link_libraries(HTTP_SERVER LINK_PUBLIC ${Boost_LIBRARIES})

set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/build")