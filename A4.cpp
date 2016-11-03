#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

#include "A4.hpp"
#include "Ray.hpp"

using namespace std;

#define DEFAULT 0

Ray makePrimaryRay(int x, int y, glm::vec3 a, glm::vec3 b, glm::vec3 eye, glm::vec3 view) {
	/*double aspectRatio = w / h;

	double Px = (2 * ((x + 0.5) / w) - 1) * tan(fovy) * aspectRatio;
	double Py =  (1 - 2 * ((y + 0.5) / h)) * tan(fovy);
// float Py = (1 - 2 * ((y + 0.5) / imageHeight) * tan(fov / 2 * M_PI / 180);
	// cout << Px << " " << Py << endl;
	glm::vec3 rayDir = view + glm::vec3(Px, Py, -1);
	rayDir = glm::normalize(rayDir);*/
	cout << x << " " << y << " " << x*a << " " << y*b << endl;
	glm::vec3 rayDir = glm::normalize(view) + x*a + y*b;


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
	double pixelSize = tan( glm::radians( fovx/2 ) ) / h;
	glm::vec3 a = pixelSize * right;
	glm::vec3 b = pixelSize * normalize(cross(right, cameraDir));
	cout << "a: " << a << endl;

	cout << "b: " << b << endl;
	// cout << a.x << endl;

	// b = glm::normalize(b);
	// cout << "a: " << a << endl;
	// cout << "b: " << b << endl;


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
			// convert 
			Ray ray = makePrimaryRay(x - w/2, y - h/2, a, b, eye, view);
			// cout << ray.pos << ", " << ray.dir << endl;
			image(x, y, 0) = 1;
			image(x, y, 1) = 0;
			image(x, y, 2) = 0;
			#endif // default
		}
	}

}

