add_library(oglml SHARED dummy.cpp vec.hpp vecfwd.hpp)

add_subdirectory(helpers)
add_subdirectory(vec)


target_link_libraries(oglml oglml_helpers)
