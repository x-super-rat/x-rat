cmake_minimum_required(VERSION 3.5)
project(x-rat_common)

include_directories("include")
include_directories(${MBEDTLS_INCLUDE_DIRS})

set(XRAT_COMMON_VERSION_MAJOR ${XRAT_BASE_VERSION_MAJOR})
set(XRAT_COMMON_VERSION_MINOR ${XRAT_BASE_VERSION_MINOR})
set(XRAT_COMMON_VERSION_PATCH ${XRAT_BASE_VERSION_PATCH})
set(XRAT_COMMON_VERSION ${XRAT_COMMON_VERSION_MAJOR}.${XRAT_COMMON_VERSION_MINOR}.${XRAT_COMMON_VERSION_PATCH})
add_definitions(-DXRAT_COMMON_VERSION="${XRAT_COMMON_VERSION}")

set(XRAT_COMMON_HDR
    include/x_rat_common.h
    include/Utilities.h
    include/AbstractEncryptionProvider.h
    include/AESGCMEncryptionProvider.h
    include/AESCTREncryptionProvider.h
    )
set(XRAT_COMMON_SRC
    src/x_rat_common.cpp
    src/Utilities.cpp
    src/AbstractEncryptionProvider.cpp
    src/AESGCMEncryptionProvider.cpp
    src/AESCTREncryptionProvider.cpp
    )
set(DEPENDS
    ${MBEDTLS_LIBRARIES}
    )

add_library(${PROJECT_NAME} STATIC ${XRAT_COMMON_HDR} ${XRAT_COMMON_SRC})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})
