#include <oglml/vec.hpp>

// Acceptable for little test programs
using namespace oglml;

int main() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(1.0, 2.0, 3.0);

    vec3 r;

    r.xy = vec2(v1.xy);

    print(r);

    return 0;
}
