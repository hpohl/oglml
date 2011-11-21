#include <oglml/vec.hpp>
#include <oglml/mat.hpp>

#include <glm/glm.hpp>

// Include
using namespace oglml::glsl;

struct S { } s;

int main() {
    mat2x2 m1;
    mat2x2 m2;
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);
    vec3 v3(3.0, 2.0, 1.0);

    m1[1] = v1.xy;

    print(ceil(v1));

    v1 += v2;

    glm::vec3 glmv1;
    glm::vec3 glmv2;

    glm::bvec3 r = glm::greaterThan(glmv1, glmv2);

    return 0;
}
