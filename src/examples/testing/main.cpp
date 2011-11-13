#include <oglml/vec.hpp>

// Include
using namespace oglml::glsl;

int main() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);
    vec3 r;
    //vec3 r((v1.xy + v1.x).yx, (v1 * v1.zzz).a); // Bug

    //swizzle(r, 0, 1, 1) = v1;

    print(r);

    std::cout << sizeof(vec2) << std::endl;
    std::cout << sizeof(vec3) << std::endl;
    std::cout << sizeof(vec4) << std::endl;
    std::cout << sizeof(ivec2) << std::endl;

    return 0;
}
