#include <oglml/vec.hpp>

// Acceptable for little test programs
using namespace oglml;

int main() {
    //float data[] = { 10.0, 20.0, 30.0 };
    //float* dataPtr = data;

    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(3.0, 2.0, 1.0);

    vec3 r = 100.0 / v1;

    print(r);

    return 0;
}
