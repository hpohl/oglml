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

    vec3 r(v1);

    r = negate(-r);

    print(r);

    return 0;
}
