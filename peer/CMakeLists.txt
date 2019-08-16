cmake_minimum_required(VERSION 3.5)
project(x-rat_peer)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/common/include")

set(XRAT_PEER_VERSION_MAJOR ${XRAT_BASE_VERSION_MAJOR})
set(XRAT_PEER_VERSION_MINOR ${XRAT_BASE_VERSION_MINOR})
set(XRAT_PEER_VERSION_PATCH ${XRAT_BASE_VERSION_PATCH})
set(XRAT_PEER_VERSION ${XRAT_PEER_VERSION_MAJOR}.${XRAT_PEER_VERSION_MINOR}.${XRAT_PEER_VERSION_PATCH})
add_definitions(-DXRAT_PEER_VERSION="${XRAT_PEER_VERSION}")

set(XRAT_PEER_HDR
    x_rat_peer.h
    )
set(XRAT_PEER_SRC
    x_rat_peer.cpp
    )
set(DEPENDS
    x-rat_common
    ${MALLOC_LIB}
    )

add_executable(${PROJECT_NAME} ${XRAT_PEER_HDR} ${XRAT_PEER_SRC})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})
