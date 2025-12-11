cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PATCH_VERSION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PATCH_VERSION})

enable_testing()

#find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(boost_unit_test_framework REQUIRED)
if(NOT boost_unit_test_framework_FOUND)
    message(FATAL_ERROR "Boost.UnitTestFramework not found!")
endif()

add_subdirectory(vendor/google/googletest/googletest)
if(NOT TARGET gtest OR NOT TARGET gtest_main)
    message(FATAL_ERROR "Google Test targets (gtest, gtest_main) not found!")
endif()

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)
add_executable(test_version test_version.cpp)
add_executable(gtest_version gtest_version.cpp)

set_target_properties(
    helloworld_cli
    helloworld
    test_version
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)
# Настройки для Boost.Test
target_link_libraries(test_version
    PRIVATE
        boost_unit_test_framework::boost_unit_test_framework
        helloworld
)
target_compile_definitions(test_version
    PRIVATE BOOST_TEST_DYN_LINK
)

# Настройки для Google Test
target_link_libraries(gtest_version
    PRIVATE
        gtest
        gtest_main
        helloworld
)

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE/W4)
    target_compile_options(helloworld PRIVATE/W4)
    target_compile_options(test_version PRIVATE/W4)
    target_compile_options(gtest_version PRIVATE /W4)
else ()
    target_compile_options(helloworld_cli PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(helloworld PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(gtest_version PRIVATE -Wall -Wextra -pedantic -Werror)
endif()



install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

add_test(NAME BoostTest COMMAND test_version)
add_test(NAME MyGTest COMMAND gtest_version)