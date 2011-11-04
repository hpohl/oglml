#include <oglml/vec.hpp>

// Acceptable for little test programs
using namespace oglml;

struct St;

class C {
    struct S { };

    friend struct St;
};

struct St {
    C::S s;
};

int main() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(3.0, 2.0, 1.0);

    vec3 r(v1 + v2.xyz);

    std::cout << r.z << std::endl;

    print(r);

    return 0;
}
