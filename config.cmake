INCLUDE(CheckIncludeFiles)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)
INCLUDE("CMakePushCheckStateEx.cmake")


CHECK_INCLUDE_FILES("sys/stat.h" HAVE_SYS_STAT_H)

#The size of `pthread_t', as computed by sizeof.
CMAKE_RESET_CHECK_STATE_EX()
SET(CMAKE_EXTRA_INCLUDE_FILES "pthread.h")
CHECK_TYPE_SIZE("pthread_t" SIZEOF_PTHREAD_T)
SET(HAVE_PTHREADS ${SIZEOF_PTHREAD_T})

CMAKE_RESET_CHECK_STATE_EX()
CHECK_SYMBOL_EXISTS(S_IRGRP "sys/stat.h" HAVE_S_IRGRP)
CHECK_SYMBOL_EXISTS(S_IROTH "sys/stat.h" HAVE_S_IROTH)

CHECK_FUNCTION_EXISTS(strerror_r HAVE_STRERROR_R)
CHECK_FUNCTION_EXISTS(strerror_s HAVE_STRERROR_S)
CHECK_FUNCTION_EXISTS(strptime HAVE_STRPTIME)
CHECK_FUNCTION_EXISTS(timegm HAVE_TIMEGM)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/qlibc-config.h)
