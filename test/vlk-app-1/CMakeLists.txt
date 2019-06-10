
set(SRCS
    src/main.cpp
)

add_executable(vlk-app-1 "${SRCS}")

target_link_libraries(vlk-app-1
    PRIVATE vlk
)
