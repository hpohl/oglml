add_library(oglml SHARED dummy.cpp vec.hpp vecfwd.hpp
            mat.hpp matfwd.hpp funcs.hpp operators.hpp glsl.hpp)

add_subdirectory(helpers)
add_subdirectory(vec)
add_subdirectory(mat)


target_link_libraries(oglml oglml_helpers)
