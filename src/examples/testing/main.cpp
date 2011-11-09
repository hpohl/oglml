#include <oglml/vec.hpp>

// Include t
using namespace oglml::glsl;

vec3 process(const vec3& v) {
    vec3 w(v);
    w = v + v.yyy;
    return w;
}

int main() {
    // Def
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(v1);

    // Calc
    vec3 r = 10.0 + v1;

    // Print
    print(r);

    return 0;
}
