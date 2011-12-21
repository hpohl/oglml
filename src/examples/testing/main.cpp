#include <oglml/glsl.hpp>
#include <glm/glm.hpp>

#include <ctime>
#include <cstdio>

struct S { };

/*mat4 func(const mat4& m1, const mat4& m2) {
    return mat4(m1[0] * m2[1], m1[1], m1[2], m1[3]);
}*/

int main() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);
    vec3 vr;

    print(max(v1, v2.xxx));

    // -------------
    /*mat2x4 m1(1, 2, 3, 4,
              5, 6, 7, 8);

    mat4x2 m2(1, 2,
              3, 4,
              5, 6,
              7, 8);

    auto mr(v1.x * m1);

    oglml::print(mr);*/

    return 0;
}
