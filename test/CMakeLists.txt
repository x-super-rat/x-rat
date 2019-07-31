cmake_minimum_required(VERSION 3.5)
project(x-rat_test)

find_package(GTest REQUIRED)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/common/include")
include_directories("${CMAKE_SOURCE_DIR}/server/include")
include_directories("${CMAKE_SOURCE_DIR}/server/src")
include_directories("${CMAKE_SOURCE_DIR}/server")
include_directories(${GTEST_INCLUDE_DIRS})

set(XRAT_PEER_HDR
    UtilitiesTest.h
    IPv4BitMapTest.h
    )

set(XRAT_PEER_SRC
    main.cpp
    )
set(DEPENDS
    x-rat_common
    ${GTEST_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    x-rat_server-static
    )

add_executable(${PROJECT_NAME} ${XRAT_PEER_HDR} ${XRAT_PEER_SRC})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})
