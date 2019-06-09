include(GenerateExportHeader)

find_package(Vulkan REQUIRED)
find_package(glfw3 3 REQUIRED)

find_package(PkgConfig)
pkg_check_modules(glm REQUIRED glm)

set(SRCS
    src/dummy.cpp
)

add_library(vlk SHARED ${SRCS})

generate_export_header(vlk
    BASE_NAME VLK
    EXPORT_FILE_NAME gen/vlk/export.h
)

target_compile_features(vlk
    # C++ features --> results in C++ 17
    PUBLIC cxx_attribute_deprecated cxx_auto_type cxx_constexpr cxx_decltype cxx_delegating_constructors
        cxx_deleted_functions cxx_digit_separators cxx_enum_forward_declarations cxx_final cxx_inheriting_constructors
        cxx_inline_namespaces cxx_lambdas cxx_lambda_init_captures cxx_noexcept cxx_nullptr cxx_override cxx_range_for
        cxx_right_angle_brackets cxx_rvalue_references cxx_sizeof_member cxx_static_assert cxx_strong_enums
        cxx_variable_templates cxx_variadic_templates cxx_template_template_parameters
)

target_compile_options(vlk
    PRIVATE -Werror -Wall -Wextra -Wpedantic -Wformat -Wimplicit-fallthrough=5 -Wmissing-include-dirs
        -Wsuggest-override -Wduplicated-branches -Wduplicated-cond -Wshadow -Wlogical-op
        PRIVATE -fvisibility=hidden
)

target_compile_definitions(vlk
    PUBLIC -DGLFW_INCLUDE_VULKAN -DGLM_FORCE_RADIAN -DGLM_FORCE_DEPTH_ZERO_TO_ONE -DLOG_LEVEL_DEBUG -DVLK_VULKAN_VALIDATAION_LAYER
)

target_link_libraries(vlk
    PUBLIC Vulkan::Vulkan glfw
)