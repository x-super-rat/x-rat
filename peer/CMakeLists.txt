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
    )

add_library(${PROJECT_NAME}-obj OBJECT ${XRAT_PEER_HDR} ${XRAT_PEER_SRC})
add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(${PROJECT_NAME}-shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
target_link_libraries(${PROJECT_NAME}-static ${DEPENDS})
target_link_libraries(${PROJECT_NAME}-shared ${DEPENDS})

set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        VERSION ${XRAT_PEER_VERSION}
        SOVERSION ${XRAT_PEER_VERSION_MAJOR})

install(FILES ${XRAT_PEER_HDR}
   DESTINATION include)
install(TARGETS ${PROJECT_NAME}-shared ${PROJECT_NAME}-static
    DESTINATION "lib/${CMAKE_LIBRARY_ARCHITECTURE}")
