#include "glm/glm.hpp"
#include <cmath>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::



class Objekat {
public:
	glm::mat4 position;
};


int main()
{

	Objekat o;
	glm::rotate(o.position, static_cast<float>(M_PI), glm::vec3(0,1,0));


	return 0;
}
