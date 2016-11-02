#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

#include "A4.hpp"
#include "Ray.hpp"

using namespace std;

#define DEFAULT 0

Ray makePrimaryRay(uint x, uint y, glm::vec3 eye, glm::vec3 view, size_t w, size_t h, double fovx, double fovy) {
	double aspectRatio = w / h;

	double Px = (2 * ((x + 0.5) / w) - 1) * tan(fovy) * aspectRatio;
	double Py =  (1 - 2 * ((y + 0.5) / h)) * tan(fovy);
// float Py = (1 - 2 * ((y + 0.5) / imageHeight) * tan(fov / 2 * M_PI / 180);
	cout << Px << " " << Py << endl;
	glm::vec3 rayDir = view + glm::vec3(Px, Py, -1);
	rayDir = glm::normalize(rayDir);

	Ray ray( eye, rayDir );
	return ray;
}

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();


	const glm::vec3 cameraDir = glm::normalize(view);

	const glm::vec3 right = glm::cross(up, cameraDir);
	cout << right << endl;

	double aspectRatio = w / h;
	double fovx = fovy * aspectRatio;
	// cout << fovx << ", " << fovy << endl;

	/*glm::vec3 a = tan(glm::radians(fovx/2)) * right;
	glm::vec3 b = tan(glm::radians(fovy/2)) * cross(right, cameraDir);
	cout << a << endl;

	cout << b << endl;

	b = glm::normalize(b);
	cout << a << endl;
	cout << b << endl;*/


	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			#if DEFAULT
			// Red: increasing from top to bottom
			image(x, y, 0) = (double)y / h;
			// Green: increasing from left to right
			image(x, y, 1) = (double)x / w;
			// Blue: in lower-left and upper-right corners
			image(x, y, 2) = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
			#else
			Ray ray = makePrimaryRay(x, y, eye, view, w, h, glm::radians(fovx), glm::radians(fovy));
			cout << ray.pos << ", " << ray.dir << endl;
			image(x, y, 0) = 1;
			image(x, y, 1) = 0;
			image(x, y, 2) = 0;
			#endif // default
		}
	}

}

