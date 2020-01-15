#include "classes.hpp"
#include <functional>
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"

std::ostream& operator<<(std::ostream& out, glm::mat4 const& matrix) {
    return out << glm::to_string(matrix);
}

int main()
{
    rg::Hexahedron body(glm::vec3(0,0,0), glm::vec2(0,0));
    std::cout << body.transformation_matrix() << std::endl;

    // use glm functions on transformation_matrix matrix
    body.set_transformation_matrix(
            glm::translate(body.transformation_matrix(), glm::vec3(0.33, 0.33, 0.33)));

    std::cout << body.transformation_matrix() << std::endl;

    /* or even easier */
    body.transform(glm::scale(glm::vec3(1.2f, 1.2f, 1.2f)));
    std::cout << body.transformation_matrix() << std::endl;

    // apply manualy defined transformation (old_matrix) -> new_matrix
    body.transform(
        [](auto const &old_transformation_matrix) { 
            auto transformed = glm::translate(old_transformation_matrix, glm::vec3(-0.25, -0.25, 0.5)); 
            transformed = glm::scale(transformed, {2.0f, 2.0f, 2.0f});
            return transformed;
        });

    std::cout << body.transformation_matrix() << std::endl;

    // you can use some of pre-defined transformations
    body.transform(rg::transformation::SetPosition(glm::vec3(0.5,0.5,-1.0)));
    std::cout << body.transformation_matrix() << std::endl;

    std::vector<std::unique_ptr<rg::Transformation>> ts {};
    ts.emplace_back(std::make_unique<rg::transformation::SetPosition>(glm::vec3(20, 20, 20)));
    ts.emplace_back(std::make_unique<rg::Transformator>([](auto const &m) { return glm::translate(m, glm::vec3(1,2,3));}));
    ts.emplace_back(std::make_unique<rg::TransformatorMatrix>(glm::translate(glm::vec3(1.2,1.2,1.2))));

    for (const auto& t : ts)
        body.transform(*t);

    std::cout << body.transformation_matrix() << std::endl;

    return 0;
}
