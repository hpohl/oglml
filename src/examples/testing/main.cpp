#include <oglml/vec.hpp>

int main() {
    oglml::vec3 v1(1.0, 2.0, 3.0);
    oglml::vec3 v2(10.0, 20.0, 30.0);

    oglml::vec3 r((v1 * 10.0).zyx);

    oglml::print(r);

    return 0;
}
