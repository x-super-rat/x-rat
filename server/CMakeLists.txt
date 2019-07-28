cmake_minimum_required(VERSION 3.5)
project(x-rat_server)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/common/include")

set(XRAT_SERVER_VERSION_MAJOR ${XRAT_BASE_VERSION_MAJOR})
set(XRAT_SERVER_VERSION_MINOR ${XRAT_BASE_VERSION_MINOR})
set(XRAT_SERVER_VERSION_PATCH ${XRAT_BASE_VERSION_PATCH})
set(XRAT_SERVER_VERSION ${XRAT_SERVER_VERSION_MAJOR}.${XRAT_SERVER_VERSION_MINOR}.${XRAT_SERVER_VERSION_PATCH})
add_definitions(-DXRAT_SERVER_VERSION="${XRAT_SERVER_VERSION}")

set(XRAT_SERVER_HDR
    x-rat-server.h
    )
set(XRAT_SERVER_SRC
    x-rat-server.cpp
    )
set(XRAT_SERVER_EXEC_SRC
    x-rat-server-main.h
    )
set(XRAT_SERVER_EXEC_HDR
    x-rat-server-main.cpp
    )
set(DEPENDS
    x-rat_common
    )

add_library(${PROJECT_NAME}-obj OBJECT ${XRAT_SERVER_HDR} ${XRAT_SERVER_SRC})
add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(${PROJECT_NAME}-shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
target_link_libraries(${PROJECT_NAME}-static ${DEPENDS})
target_link_libraries(${PROJECT_NAME}-shared ${DEPENDS})

set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        VERSION ${XRAT_SERVER_VERSION}
        SOVERSION ${XRAT_SERVER_VERSION_MAJOR})

add_executable(${PROJECT_NAME} ${XRAT_SERVER_EXEC_HDR} ${XRAT_SERVER_EXEC_SRC})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)

install(FILES ${XRAT_SERVER_HDR}
   DESTINATION include)
install(TARGETS ${PROJECT_NAME}-shared ${PROJECT_NAME}-static
    DESTINATION "lib/${CMAKE_LIBRARY_ARCHITECTURE}")
