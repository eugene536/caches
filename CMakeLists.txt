PROJECT(FixedCache)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -I${CMAKE_CURRENT_SOURCE_DIR} -pedantic -Wall -Wextra -Wno-unused-result -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -g")

SET(SOURCE_FILES fixedcache.hpp
                 lrucache.hpp)

IF (1)
    SET(SOURCE_FILES ${SOURCE_FILES}
                     tests/fixedcache.cpp
                     tests/lrucache.cpp)

    SET_PROPERTY(SOURCE tests/lrucache.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lrucache.hpp)

    FIND_PACKAGE(Threads)
    ENABLE_TESTING()
    FIND_PACKAGE(GTest REQUIRED)
    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

    ADD_EXECUTABLE(FixedCache ${SOURCE_FILES})

    TARGET_LINK_LIBRARIES(FixedCache ${GTEST_BOTH_LIBRARIES})
    TARGET_LINK_LIBRARIES(FixedCache ${CMAKE_THREAD_LIBS_INIT})
ELSE()
    SET(SOURCE_FILES ${SOURCE_FILES} main.cpp)

    ADD_EXECUTABLE(FixedCache ${SOURCE_FILES})
ENDIF()
