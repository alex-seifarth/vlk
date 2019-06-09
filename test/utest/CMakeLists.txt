
find_package(GTest REQUIRED)

set(SRCS
    log/test-log.cpp)

add_executable(utest "${SRCS}")

target_link_libraries(utest
    PRIVATE GTest::GTest GTest::Main
    PRIVATE vlk
)

target_compile_options(utest
    PRIVATE -Wno-attributes
)

add_test(vlk-tests utest)
