#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

#include "A4.hpp"
#include "GeometryNode.hpp"
#include "Ray.hpp"
#include "IntersectInfo.hpp"
#include "PhongMaterial.hpp"

using namespace std;

#define DEFAULT 0

/*
	Returns a Ray from the eye to the middle of the x,y pixel on screen
*/
Ray makePrimaryRay(int x, int y,
	const glm::vec3 a,
	const glm::vec3 b,
	const glm::vec3 eye,
	const glm::vec3 view) {

	glm::vec3 rayDir = glm::normalize(glm::normalize(view) + x*a + y*b);

	Ray ray( eye, rayDir );
	return ray;
}

IntersectInfo sceneIntersect(const SceneNode * root, const glm::vec3 & eye, const Ray & ray) {
	// SceneNode * closestObject = NULL;
	GeometryNode* closestObjectNode = NULL;
	Primitive* closestObjectPrim = NULL;
	double tmin; 												// distance of closestObject
	// cout << "sceneIntersect" << endl;

	for ( SceneNode * child : root->children ) {
		// assuming all non-hierarchical spheres right now
		GeometryNode* childGeometryNode = dynamic_cast<GeometryNode*>(child);

		// NonhierSphere* childPrim = dynamic_cast<NonhierSphere*>(childGeometryNode->m_primitive);
		Primitive* childPrim = childGeometryNode->m_primitive;

		// glm::vec4 center = childGeometryNode->trans * glm::vec4(0,0,0,1);
		/*glm::vec4 center = glm::vec4(childPrim->m_pos, 1);
		double radius = childPrim->m_radius;

		glm::vec3 L = eye - glm::vec3(center);

		// cout << L << endl;

		float a = pow(glm::length(ray.dir), 2);
		float b = 2 * glm::dot(ray.dir, L);
		float c = pow(glm::length(L), 2.0f) - pow(radius, 2.0f);
		// cout << "1: a b c " << a << " " << b << " " << c << endl;
	// cout << "1: ray.dir " << ray.dir << endl;*/

		double t0; 												// results of intersection, if any
		if ( !childGeometryNode->m_primitive->intersect(eye, ray, t0) )
			continue; 											// didn't find any
		// cout << "foundPosRoot: " << t0 << endl;

		if ( closestObjectNode == NULL || t0 < tmin ) {			// closest object found?
			closestObjectNode = childGeometryNode;
			closestObjectPrim = childPrim;
			tmin = t0;
		} // if
	} // for

	if ( closestObjectNode != NULL ) {									// if we found an object
		IntersectInfo info;										// build IntersectInfo
		info.point 	= ray.pos + tmin * ray.dir;
		// info.normal = (info.point - closestObjectPrim->m_pos) / closestObjectPrim->m_radius;
		info.normal = closestObjectPrim->normalAt(info.point);

		// GeometryNode* closestObjGeometryNode = dynamic_cast<GeometryNode*>(closestObject);
		info.material = closestObjectNode->m_material;
		// cout << "hit object at " << info.point << endl;
		return info;
	} else {
		throw 0; // no object hit
	} // if
} // sceneIntersect

glm::vec3 illuminate(const IntersectInfo& info,
					 const std::list<Light *>  & lights,
					 const glm::vec3 & ambient,
					 const Ray ray,
					 const glm::vec3 eye,
					 SceneNode * root) {

	PhongMaterial* material = dynamic_cast<PhongMaterial*>(info.material);

	glm::vec3 result = glm::vec3();
	result += material->m_kd * ambient;
	double s = material->m_shininess;									// sharpness of highlight

	glm::vec3 v = eye - info.point;

	#if 1
	for (Light* light : lights) {
		const glm::vec3 lightDir = light->position - info.point;
		float r = glm::length(lightDir);								// distance from light
		// cout << "distance to light 1: " << r << endl;

		// cast shadow ray
		Ray shadow( info.point, glm::normalize( lightDir ));
		// cout << info.point << " " << glm::normalize( lightDir ) << endl;
		// cout << shadow.pos << " " << shadow.dir << endl;
		try {
			IntersectInfo shadowInfo = sceneIntersect(root, info.point, shadow);
			float shadowDist = glm::length(light->position - shadowInfo.point);
			if ( shadowDist - r > EPSILON ) {
				continue;
			}
		} catch (int noShadow) {}

		// positive dot product of normal and light vectors, or 0
		float dotNL = max(glm::dot(info.normal, glm::normalize(lightDir)), 0.0f);
		// if (dotNL < 0) dotNL = 0.0f;
		// cout << "distance to light 2: " << r << endl;

		float c0 = light->falloff[0];									// calculate attenuation
		float c1 = light->falloff[1];
		float c2 = light->falloff[2];
		float fatt = c0 + c1*r + c2*pow(r, 2);

		glm::vec3 lightFAtt = light->colour/fatt;						// light with attenuation

		glm::vec3 h = (v + lightDir ) / (glm::length(v + lightDir));

		float dotHN = pow(glm::dot(h, info.normal), s);
		if (dotHN < 0) dotHN = 0.0f;
		// cout << dotHN << endl;

		result += material->m_kd * lightFAtt * dotNL;					// apply diffuse shading
		// result += material->m_kd * lightFAtt;					// apply diffuse shading
		result += material->m_ks * lightFAtt * dotHN;					// apply specular shading
		// cout << material->m_kd << "*" << lightFAtt << "*" << dotNL << endl;
		// cout << material->m_ks * lightFAtt * dotHN << endl;
	} // for
	#endif

	return result;
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
	// cout << right << endl;

	double aspectRatio = w / h;
	double fovx = fovy * aspectRatio;
	// cout << fovx << ", " << fovy << endl;
	double pixelSize = tan( glm::radians( fovy ) ) / h;
	const glm::vec3 a = -1.0f * pixelSize * right;
	const glm::vec3 b = pixelSize * normalize(cross(right, cameraDir));
	// cout << "a: " << a << endl;
	// cout << "b: " << b << endl;

	glm::vec3 bgColour = glm::vec3(0.1f, 0.1f, 0.1f);

	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			#if DEFAULT
			// Red: increasing from top to bottom
			// image(x, y, 0) = (double)y / h;
			image(x, y, 0) = (double)y / h;
			// Green: increasing from left to right
			image(x, y, 1) = (double)x / w;
			// Blue: in lower-left and upper-right corners
			image(x, y, 2) = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
			#else
			// Red: increasing from top to bottom
			// image(x, y, 0) = (double)y / h;
			bgColour.x = (double)y / h;
			// Green: increasing from left to right
			bgColour.y = (double)x / w;
			// Blue: in lower-left and upper-right corners
			bgColour.z = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;

			Ray ray = makePrimaryRay(x - w/2, y - h/2, a, b, eye, view);
			// cout << ray.pos << ", " << ray.dir << endl;

			glm::vec3 colour = glm::vec3();
			try {
				IntersectInfo info = sceneIntersect(root, eye, ray);
				// cout << x << " " << y << " hit obj!" << endl;

				colour = illuminate(info, lights, ambient, ray, eye, root);
			} catch (int noObj) {
				// no object intersection. draw background colour.
				colour = bgColour;
			} // try

			image(x, y, 0) = colour.x;
			image(x, y, 1) = colour.y;
			image(x, y, 2) = colour.z;
			#endif // default
		} // for
	} // for

}
