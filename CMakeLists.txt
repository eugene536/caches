PROJECT(FixedCache)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

OPTION(test "Build all tests." OFF)
OPTION(debug "Build with debug info." OFF)

if (debug)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pedantic -Wall -Wextra -Wno-unused-result -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -g")
else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O2")
endif()

SET(SOURCE_FILES main/fixedcache.hpp
                 main/lrucache.hpp
                 main/fifocache.hpp
                 main/lfucache.hpp)

################################
# Testing
################################
if (test)
    SET(SOURCE_FILES ${SOURCE_FILES}
                     tests/base_tests.hpp
                     tests/main.cpp

                     tests/lrucache.cpp
                     tests/fifocache.cpp
                     tests/lfucache.cpp
                     )

#    FIND_PACKAGE(Threads)
#    ENABLE_TESTING()
#    FIND_PACKAGE(GTest REQUIRED)
#    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

#    ADD_EXECUTABLE(FixedCache  ${SOURCE_FILES})

#    TARGET_LINK_LIBRARIES(FixedCache ${GTEST_BOTH_LIBRARIES})
#    TARGET_LINK_LIBRARIES(FixedCache ${CMAKE_THREAD_LIBS_INIT})

     add_subdirectory(gtest)
     ENABLE_TESTING()
     include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
     ADD_EXECUTABLE(FixedCache  ${SOURCE_FILES})
     TARGET_LINK_LIBRARIES(FixedCache gtest gtest_main)
ELSE()
    SET(SOURCE_FILES ${SOURCE_FILES} main.cpp)

    ADD_EXECUTABLE(FixedCache ${SOURCE_FILES})
ENDIF()

