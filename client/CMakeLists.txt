cmake_minimum_required(VERSION 3.5)
project(x-rat_client)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/common/include")

set(XRAT_CLIENT_VERSION_MAJOR ${XRAT_BASE_VERSION_MAJOR})
set(XRAT_CLIENT_VERSION_MINOR ${XRAT_BASE_VERSION_MINOR})
set(XRAT_CLIENT_VERSION_PATCH ${XRAT_BASE_VERSION_PATCH})
set(XRAT_CLIENT_VERSION ${XRAT_CLIENT_VERSION_MAJOR}.${XRAT_CLIENT_VERSION_MINOR}.${XRAT_CLIENT_VERSION_PATCH})
add_definitions(-DXRAT_CLIENT_VERSION="${XRAT_CLIENT_VERSION}")

set(XRAT_CLIENT_HDR
    x-rat-client.h
    )
set(XRAT_CLIENT_SRC
    x-rat-client.cpp
    )
set(XRAT_CLIENT_EXEC_HDR
    x-rat-client-main.h
    )
set(XRAT_CLIENT_EXEC_SRC
    x-rat-client-main.cpp
    )
set(DEPENDS
    x-rat_common
    ${MALLOC_LIB}
    )

add_library(${PROJECT_NAME}-obj OBJECT ${XRAT_CLIENT_HDR} ${XRAT_CLIENT_SRC})
add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(${PROJECT_NAME}-shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
target_link_libraries(${PROJECT_NAME}-static ${DEPENDS})
target_link_libraries(${PROJECT_NAME}-shared ${DEPENDS})

set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        VERSION ${XRAT_CLIENT_VERSION}
        SOVERSION ${XRAT_CLIENT_VERSION_MAJOR})

add_executable(${PROJECT_NAME} ${XRAT_CLIENT_EXEC_HDR} ${XRAT_CLIENT_EXEC_SRC})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)

install(FILES ${XRAT_CLIENT_HDR}
   DESTINATION include)
install(TARGETS ${PROJECT_NAME}-shared ${PROJECT_NAME}-static
    DESTINATION "lib/${CMAKE_LIBRARY_ARCHITECTURE}")
