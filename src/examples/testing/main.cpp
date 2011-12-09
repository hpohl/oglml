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
    vec3 vr;

    //print(vr);

    // -------------
    mat3 m1;
    mat2 m2;
    mat4 mr(10.0);

    print(mr);

    return 0;
}
