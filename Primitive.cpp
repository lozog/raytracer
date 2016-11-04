#include "Primitive.hpp"
#include "findPosRoot.hpp"

#include <iostream>
#include <cmath>                            // pow
using namespace std;
#include <glm/gtx/io.hpp>

Primitive::~Primitive()
{
}

bool Primitive::intersect(const glm::vec3 & eye, const Ray & ray, float& t0) {
	cout << "Primitive intersect" << endl;
	return false;
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const glm::vec3 & eye, const Ray & ray, float& t0) {
	return false;
}

Cube::~Cube()
{
}

bool Cube::intersect(const glm::vec3 & eye, const Ray & ray, float& t0) {
	return false;
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::intersect(const glm::vec3 & eye, const Ray & ray, float& t0) {
	// glm::vec4 center = childGeometryNode->trans * glm::vec4(0,0,0,1);
	// cout << "nonhiersphere intersect" << endl;
	glm::vec4 center = glm::vec4(m_pos, 1);
	double radius = m_radius;

	glm::vec3 L = eye - glm::vec3(center);

	// cout << L << endl;
	// cout << "2: ray.dir " << ray.dir << endl;

	float a = pow(glm::length(ray.dir), 2);
	float b = 2 * glm::dot(ray.dir, L);
	float c = pow(glm::length(L), 2.0f) - pow(radius, 2.0f);

		// cout << "2: a b c " << a << " " << b << " " << c << endl;

	return findPosRoot(a, b, c, t0);
}


NonhierBox::~NonhierBox()
{
}

bool NonhierBox::intersect(const glm::vec3 & eye, const Ray & ray, float& t0) {
	cout << "nonhierbox intersect" << endl;
	return false;
}