#include <oglml/vec.hpp>
#include <oglml/mat.hpp>

// Include
using namespace oglml::glsl;

int main() {
    mat2x2 m1;
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);

    m1[0] = vec2(1.0, 2.0);

    print(m1[0]);

    std::cout << (1.0 != v2) << std::endl;

    return 0;
}
