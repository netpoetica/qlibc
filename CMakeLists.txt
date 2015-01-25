CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(qlibc C)

SET(qlibc_VERSION "2.1.6.5")

# Tell CMake we will use CTest
ENABLE_TESTING()

# Tell CMake there is a CMakeLists.txt in the tests/ directory
ADD_SUBDIRECTORY(tests)

FILE(GLOB QLIBC_INCLUDE_FILES include/qlibc/*.h)
FILE(GLOB QLIBC_INCLUDE_CONTAINERS_FILES include/qlibc/containers/*.h)
FILE(GLOB QLIBC_INCLUDE_EXTENSIONS_FILES include/qlibc/extensionss/*.h)
FILE(GLOB QLIBC_INCLUDE_IPC_FILES include/qlibc/ipc/*.h)
FILE(GLOB QLIBC_INCLUDE_UTILITIES_FILES include/qlibc/utilities/*.h)
INCLUDE_DIRECTORIES(src/internal)
INCLUDE_DIRECTORIES(src/internal/compat)

INCLUDE("config.cmake")

IF(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${CMAKE_BINARY_DIR}/qlibc-config.h")
ELSEIF(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /FI ${CMAKE_BINARY_DIR}/qlibc-config.h")
ELSEIF(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${CMAKE_BINARY_DIR}/qlibc-config.h")
ELSE()
    MESSAGE(FATAL_ERROR "Your compiler (${CMAKE_C_COMPILER_ID}) isn't supported yet")
ENDIF()

SET(SRC_SUBPATHS
        containers/*.c
        utilities/*.c
        ipc/*.c
        internal/*.c)

SET(SRC_SUBPATHS_EXT
        extensions/*.c)

SET(SRC_PATHS src src/os/compat)
SET(QLIBC_LINK_LIBS )
SET(QLIBC_EXT_LINK_LIBS qlibc)

IF(HAVE_PTHREADS)
    LIST(APPEND QLIBC_LINK_LIBS pthread)
	LIST(APPEND QLIBC_EXT_LINK_LIBS pthread)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    SET(SRC_PATHS ${SRC_PATHS} src/os/linux src/os/nix)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(SRC_PATHS ${SRC_PATHS} src/os/windows)
	
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_IPC -DDISABLE_QHTTPCLIENT -DDISABLE_QLOG")
	LIST(APPEND QLIBC_LINK_LIBS "ws2_32")
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    SET(SRC_PATHS ${SRC_PATHS} src/os/nix)
ENDIF()

#Add os-related source paths for qlibc
FOREACH(TMP_path IN LISTS SRC_PATHS)
    FOREACH(TMP_subpath IN LISTS SRC_SUBPATHS)
		LIST(APPEND SRC_SEARCH_PATHS "${TMP_path}/${TMP_subpath}")
    ENDFOREACH(TMP_subpath)
ENDFOREACH(TMP_path)

#Add os-related source paths files for qlibcext
FOREACH(TMP_path IN LISTS SRC_PATHS)
    FOREACH(TMP_subpath IN LISTS SRC_SUBPATHS_EXT)
		LIST(APPEND SRC_SEARCH_PATHS_EXT "${TMP_path}/${TMP_subpath}")
    ENDFOREACH(TMP_subpath)
ENDFOREACH(TMP_path)

FILE(GLOB_RECURSE SRC_LIB 
        ${SRC_SEARCH_PATHS})

FILE(GLOB_RECURSE SRC_LIB_EXT 
        ${SRC_SEARCH_PATHS_EXT})

ADD_LIBRARY(qlibc-static STATIC ${SRC_LIB})
ADD_LIBRARY(qlibc SHARED ${SRC_LIB})

ADD_LIBRARY(qlibcext-static STATIC ${SRC_LIB_EXT})
ADD_LIBRARY(qlibcext SHARED ${SRC_LIB_EXT})

TARGET_INCLUDE_DIRECTORIES(qlibc-static PUBLIC include include/qlibc)
TARGET_INCLUDE_DIRECTORIES(qlibc-static PUBLIC ${CMAKE_BINARY_DIR})
TARGET_INCLUDE_DIRECTORIES(qlibc PUBLIC include include/qlibc)
TARGET_INCLUDE_DIRECTORIES(qlibc PUBLIC ${CMAKE_BINARY_DIR})
TARGET_INCLUDE_DIRECTORIES(qlibcext-static PUBLIC include include/qlibc)
TARGET_INCLUDE_DIRECTORIES(qlibcext-static PUBLIC ${CMAKE_BINARY_DIR})
TARGET_INCLUDE_DIRECTORIES(qlibcext PUBLIC include include/qlibc)
TARGET_INCLUDE_DIRECTORIES(qlibcext PUBLIC ${CMAKE_BINARY_DIR})

TARGET_LINK_LIBRARIES(qlibc ${QLIBC_LINK_LIBS})
TARGET_LINK_LIBRARIES(qlibcext ${QLIBC_EXT_LINK_LIBS})

INSTALL(TARGETS qlibc qlibc-static qlibcext qlibcext-static
        EXPORT qlibc-bundle
        RUNTIME DESTINATION lib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        INCLUDES DESTINATION include)

INSTALL(FILES
        ${QLIBC_INCLUDE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/qlibc-config.h
        DESTINATION include/qlibc)
INSTALL(FILES
        ${QLIBC_INCLUDE_CONTAINERS_FILES} 
        DESTINATION include/qlibc/containers)
INSTALL(FILES
        ${QLIBC_INCLUDE_EXTENSIONS_FILES} 
        DESTINATION include/qlibc/extensions)
INSTALL(FILES
        ${QLIBC_INCLUDE_IPC_FILES} 
        DESTINATION include/qlibc/ipc)
INSTALL(FILES
        ${QLIBC_INCLUDE_UTILITIES_FILES} 
        DESTINATION include/qlibc/utilities)


EXPORT(TARGETS qlibc qlibc-static qlibcext qlibcext-static FILE ${CMAKE_BINARY_DIR}/qlibc-config.cmake)
EXPORT(PACKAGE qlibc)

