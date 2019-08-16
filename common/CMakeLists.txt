cmake_minimum_required(VERSION 3.5)
project(x-rat_common)

include_directories("include")
include_directories(${MBEDTLS_INCLUDE_DIRS})

set(XRAT_COMMON_VERSION_MAJOR ${XRAT_BASE_VERSION_MAJOR})
set(XRAT_COMMON_VERSION_MINOR ${XRAT_BASE_VERSION_MINOR})
set(XRAT_COMMON_VERSION_PATCH ${XRAT_BASE_VERSION_PATCH})
set(XRAT_COMMON_VERSION ${XRAT_COMMON_VERSION_MAJOR}.${XRAT_COMMON_VERSION_MINOR}.${XRAT_COMMON_VERSION_PATCH})
add_definitions(-DXRAT_COMMON_VERSION="${XRAT_COMMON_VERSION}")

find_package(LibUV REQUIRED)

set(XRAT_COMMON_HDR
    include/x_rat_common.h
    include/Utilities.h
    include/CheesePacket.h
    include/PacketDecoder.h
    include/PacketEncoder.h
    include/AbstractEncryptionProvider.h
    include/AESGCMEncryptionProvider.h
    include/AESCTREncryptionProvider.h
    include/AbstractKeyManager.h
    include/SingleKeyManager.h
    include/PacketChainManager.h
    include/EncryptionManager.h
    include/XfrmManager.h
    include/ForwardManager.h
    include/PacketVector.h
    include/EncodeManager.h
    include/RandomPool.h
    )
set(XRAT_COMMON_SRC
    src/x_rat_common.cpp
    src/PacketDecoder.cpp
    src/PacketEncoder.cpp
    src/Utilities.cpp
    src/AbstractEncryptionProvider.cpp
    src/AESGCMEncryptionProvider.cpp
    src/AESCTREncryptionProvider.cpp
    src/AbstractKeyManager.cpp
    src/SingleKeyManager.cpp
    src/PacketChainManager.cpp
    src/EncryptionManager.cpp
    src/XfrmManager.cpp
    src/ForwardManager.cpp
    src/PacketVector.cpp
    src/EncodeManager.cpp
    src/RandomPool.cpp
    )
set(DEPENDS
    ${MBEDTLS_LIBRARIES}
    ${LIBUV_LIBRARIES}
    )

include_directories(${LIBUV_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${XRAT_COMMON_HDR} ${XRAT_COMMON_SRC})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})
