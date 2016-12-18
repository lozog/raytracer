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

IntersectInfo sceneIntersect(const SceneNode * root, const glm::vec3 & eye, const Ray & ray, uint x, uint y) {
	GeometryNode* closestObjectNode = NULL;
	Primitive* closestObjectPrim = NULL;
	double tmin; 												// distance of closestObject
	glm::vec3 normalMin;										// normal of closestObject
	// cout << "sceneIntersect" << endl;

	for ( SceneNode * child : root->children ) {
		// assuming non-hierarchical right now
		GeometryNode* childGeometryNode = dynamic_cast<GeometryNode*>(child);

		// NonhierSphere* childPrim = dynamic_cast<NonhierSphere*>(childGeometryNode->m_primitive);
		Primitive* childPrim = childGeometryNode->m_primitive;

		double t0; 												// results of intersection, if any
		glm::vec3 normal;
		if ( !childGeometryNode->m_primitive->intersect(eye, ray, t0, normal) )
			continue; 											// didn't find any
		// cout << "foundPosRoot: " << t0 << endl;

		if ( closestObjectNode == NULL || t0 < tmin ) {			// closest object found?
			closestObjectNode = childGeometryNode;
			closestObjectPrim = childPrim;
			normalMin = normal;
			tmin = t0;
		} // if
	} // for

	if ( closestObjectNode != NULL ) {									// if we found an object
		IntersectInfo info;										// build IntersectInfo
		info.point 	= ray.pos + (tmin - 0.01f) * ray.dir;		// offset a little bit to account for error
		// info.normal = (info.point - closestObjectPrim->m_pos) / closestObjectPrim->m_radius;
		// info.normal = closestObjectPrim->normalAt(info.point);
		info.normal = normalMin;

		// GeometryNode* closestObjGeometryNode = dynamic_cast<GeometryNode*>(closestObject);
		info.material = closestObjectNode->m_material;
/*		if (x == 223 && y == 255) {
			cout << eye << endl;
		}*/
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
					 SceneNode * root, uint x, uint y) {

	PhongMaterial* material = dynamic_cast<PhongMaterial*>(info.material);

	glm::vec3 result = glm::vec3();
	result += material->m_kd * ambient;
	double s = material->m_shininess;									// sharpness of highlight

	glm::vec3 v = eye - info.point;

	for (Light* light : lights) {
		const glm::vec3 lightDir = light->position - info.point;
		double r = glm::length(lightDir);								// distance from light
		// cout << "distance to light 1: " << r << endl;

		// cast shadow ray
		Ray shadow( info.point, glm::normalize( lightDir ));
		// cout << info.point << " " << glm::normalize( lightDir ) << endl;
		// cout << shadow.pos << " " << shadow.dir << endl;
		try {
			IntersectInfo shadowInfo = sceneIntersect(root, info.point, shadow, x, y);
			double shadowDist = glm::length(light->position - shadowInfo.point);
			if ( shadowDist - r < EPSILON ) {
				// result += glm::vec3(1.0f, 1.0f, 1.0f);
				continue;
			}
		} catch (int noShadow) {
			// shadow ray didn't hit light
		}

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

	return result;
}

void generateBG(uint x, uint y, size_t w, size_t h, glm::vec3 & bgColour) {
	if ( y < 3*h/4 ) {
		bgColour.x = 0.1f;
		bgColour.y = (float)y / (float)(6*h);
		bgColour.z = 0.0f;

	} else {
		bgColour.x = 0.1f * (float)y / (float)(2*h);
		bgColour.y = 0.2f;
		bgColour.z = 0.0f;

	}
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

			generateBG(x, y, w, h, bgColour);

			Ray ray = makePrimaryRay(x - w/2, y - h/2, a, b, eye, view);
			// cout << ray.pos << ", " << ray.dir << endl;

			glm::vec3 colour = glm::vec3();
			try {
				IntersectInfo info = sceneIntersect(root, eye, ray, x, y);
				// cout << x << " " << y << " hit obj!" << endl;

				colour = illuminate(info, lights, ambient, ray, eye, root, x, y);
			} catch (int noObj) {
				// no object intersection. draw background colour.
				colour = bgColour;
			} // try

			image(x, y, 0) = colour.x;
			image(x, y, 1) = colour.y;
			image(x, y, 2) = colour.z;
		} // for
	} // for

}