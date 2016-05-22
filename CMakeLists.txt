PROJECT(FixedCache)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

OPTION(test "Build all tests." OFF)
OPTION(debug "Build with debug info." OFF)

IF (debug)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pedantic -Wall -Wextra -Wno-unused-result -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -g")
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O2")
ENDIF()

SET(SOURCE_FILES main/fixedcache.hpp
                 main/lrucache.hpp
                 main/fifocache.hpp
                 main/lfucache.hpp)

IF (test)
    SET(SOURCE_FILES ${SOURCE_FILES}
                     tests/base_tests.hpp

                     tests/lrucache.cpp
                     tests/fifocache.cpp
                     tests/lfucache.cpp

                     tests/main.cpp
                     )

     ADD_SUBDIRECTORY(gtest)
     ENABLE_TESTING()
     ADD_EXECUTABLE(FixedCache  ${SOURCE_FILES})
     TARGET_LINK_LIBRARIES(FixedCache gtest gtest_main)
ELSE()
    SET(SOURCE_FILES ${SOURCE_FILES} main.cpp)

    ADD_EXECUTABLE(FixedCache ${SOURCE_FILES})
ENDIF()

