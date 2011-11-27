#include <oglml/vec.hpp>

#include <ctime>
#include <cstdio>

// Include
//using namespace oglml::glsl;

struct S { };

int main() {
    oglml::vec3 v1(1.0, 2.0, 3.0);
    oglml::vec3 v2(1.0, 2.0, 3.0);
    oglml::vec2 v3(1.0, 2.0);

    oglml::assign(v1, v1.yx, 3.0);

    oglml::print(v1);

    return 0;
}
