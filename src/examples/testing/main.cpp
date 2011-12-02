#include <oglml/funcs.hpp>
#include <glm/glm.hpp>

#include <ctime>
#include <cstdio>

// Include
using namespace oglml::glsl;

struct S { };

int main() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);
    ivec3 iv1(1, 2, 3);

    vec3 r;

    r = sign(vec3(1.0, -1.0, 0.0));

    print(r);

    return 0;
}
