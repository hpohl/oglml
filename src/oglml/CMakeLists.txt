add_library(oglml SHARED dummy.cpp vec.hpp)

add_subdirectory(helpers)


target_link_libraries(oglml oglml_helpers)
