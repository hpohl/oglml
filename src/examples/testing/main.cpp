#include <oglml/vec.hpp>

// Include
using namespace oglml::glsl;

int main() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);
    vec3 r;

    r = (((v1 + v2).xy + v2.x).rr + v1.gg).x;
    r = 1.0;
    r = v1 + v2;

    print(r);

    return 0;
}
