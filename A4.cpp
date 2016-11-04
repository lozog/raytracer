#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>
#include <cmath>							// pow

#include "A4.hpp"
#include "GeometryNode.hpp"
#include "Ray.hpp"
#include "IntersectInfo.hpp"
#include "IntersectInfo.hpp"

using namespace std;

#define DEFAULT 0
const float EPSILON = 0.001f;

/*
	Returns a Ray from the eye to the middle of the x,y pixel on screen
*/
Ray makePrimaryRay(int x, int y, glm::vec3 a, glm::vec3 b, glm::vec3 eye, glm::vec3 view) {
	/*double aspectRatio = w / h;

	double Px = (2 * ((x + 0.5) / w) - 1) * tan(fovy) * aspectRatio;
	double Py =  (1 - 2 * ((y + 0.5) / h)) * tan(fovy);
// float Py = (1 - 2 * ((y + 0.5) / imageHeight) * tan(fov / 2 * M_PI / 180);
	// cout << Px << " " << Py << endl;
	glm::vec3 rayDir = view + glm::vec3(Px, Py, -1);
	rayDir = glm::normalize(rayDir);*/
	// cout << x << " " << y << " " << x*a << " " << y*b << endl;


	glm::vec3 rayDir = glm::normalize(glm::normalize(view) + x*a + y*b);

	Ray ray( eye, rayDir );
	return ray;
}

/*
	Solves a*t^2 + b*t + c, stores minimum positive root in t0
	if such a root exists
*/
bool findPosRoot(float a, float b, float c, float & t0) {
		// cout << a << " " << b << " " << c << endl;
    float determinant = b*b - 4*a*c;
    float t1;
		// cout << determinant << endl;
    
    if (determinant > 0) {
        t0 = (-b + sqrt(determinant)) / (2*a);
        t1 = (-b - sqrt(determinant)) / (2*a);
        // cout << "t0: " << t0 << ", t1: " << t1 << endl; 
        if ( t0 > t1 ) swap(t0, t1);

        if ( t0 < 0 ) {
        	t0 = t1;							// t0 negative, so try t1
        	if ( t0 < 0 )
        		return false;					// t0 and t1 both negative
        } // if

    } else if ( abs(determinant - 0.0f) < EPSILON) { // TODO: actually check properly
        cout << "det == 0" << endl;
        t0 = (-b + sqrt(determinant)) / (2*a);	// just one root
        if ( t0 < 0 ) {
        	return false;
        }
    } else {
        return false;							// no real roots
    } // if

	return true;
} // findPosRoot

IntersectInfo sceneIntersect(SceneNode * root, glm::vec3 eye, Ray ray) {
	// todo: find intersection with sphere, draw it white if hit, then we'll know if we're on the right track!
	// SceneNode * closestObject = NULL;
	GeometryNode* closestObject = NULL;
	float tmin = -1.0f; 						// distance of closestObject
	glm::vec4 closestObjCenter;
	// cout << "sceneIntersect" << endl;

	for ( SceneNode * child : root->children ) {
		// assuming all non-hierarchical spheres right now
		GeometryNode* childGeometryNode = dynamic_cast<GeometryNode*>(child);

		NonhierSphere* childPrim = dynamic_cast<NonhierSphere*>(childGeometryNode->m_primitive);

		// glm::vec4 center = childGeometryNode->trans * glm::vec4(0,0,0,1);
		glm::vec4 center = glm::vec4(childPrim->m_pos, 1);
		double radius = childPrim->m_radius;

		glm::vec3 L = eye - glm::vec3(center);

		// cout << L << endl;

		float a = pow(glm::length(ray.dir), 2);
		float b = 2 * glm::dot(ray.dir, L);
		float c = pow(glm::length(L), 2.0f) - pow(radius, 2.0f);

		float t0; 								// results of intersection, if any
		if ( !findPosRoot(a, b, c, t0) ) 
			continue; 							// didn't find any
		cout << "foundPosRoot: " << t0 << endl;

		if ( t0 > tmin ) {						// closest object found?
			closestObject = childGeometryNode;
			closestObjCenter = center;
			tmin = t0;
		} // if
	} // for

	if ( tmin > 0 ) {							// if we found an object
		IntersectInfo info;
		info.point 	= ray.pos + tmin * ray.dir;
		info.normal = 2*(info.point - glm::vec3(closestObjCenter));

		// GeometryNode* closestObjGeometryNode = dynamic_cast<GeometryNode*>(closestObject);
		info.material = closestObject->m_material;
		// cout << "hit object at " << info.point << endl;
		return info;
	} else {
		throw 0; // no object hit
	} // if
} // sceneIntersect

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
	// cout << right << endl;

	double aspectRatio = w / h;
	double fovx = fovy * aspectRatio;
	// cout << fovx << ", " << fovy << endl;
	double pixelSize = tan( glm::radians( fovx/2 ) ) / h;
	glm::vec3 a = pixelSize * right;
	glm::vec3 b = pixelSize * normalize(cross(right, cameraDir));
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

			float red = 0;
			try {
				IntersectInfo i = sceneIntersect(root, eye, ray);
				cout << x << " " << y << " hit obj!" << endl;
				red = 1;
			} catch (int noObj) {
				// no object intersection. draw background colour.
			} // try

			image(x, y, 0) = red;
			image(x, y, 1) = 0;
			image(x, y, 2) = 0;
			#endif // default
		} // for
	} // for

}

