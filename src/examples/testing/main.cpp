#include <oglml/vec.hpp>

int main() {

    const glml::vec3 v1(1.0, 2.0, 3.0);
    const glml::vec3 v2(v1.swizzle(1, 2), 10.0);

    v1.length();

    v1.print();

    return 0;
}
