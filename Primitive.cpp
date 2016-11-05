#include "Primitive.hpp"
#include "findPosRoot.hpp"

#include <iostream>
#include <cmath>                            // pow
#include <algorithm>                        // swap
using namespace std;
#include <glm/gtx/io.hpp>

Primitive::~Primitive()
{
}

bool Primitive::intersect(const glm::vec3 & eye, const Ray & ray, double & t0) {
	// cout << "Primitive intersect" << endl;
	return false;
}

glm::vec3 Primitive::normalAt(glm::vec3 point) {
	return glm::vec3();
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const glm::vec3 & eye, const Ray & ray, double & t0) {
	return false;
}

glm::vec3 Sphere::normalAt(glm::vec3 point) {
	return glm::vec3();
}

Cube::~Cube()
{
}

bool Cube::intersect(const glm::vec3 & eye, const Ray & ray, double & t0) {
	return false;
}

glm::vec3 Cube::normalAt(glm::vec3 point) {
	return glm::vec3();
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::intersect(const glm::vec3 & eye, const Ray & ray, double & t0) {
	// glm::vec4 center = childGeometryNode->trans * glm::vec4(0,0,0,1);
	// cout << "nonhiersphere intersect" << endl;
	glm::vec4 center = glm::vec4(m_pos, 1);
	double radius = m_radius;

	glm::vec3 L = eye - glm::vec3(center);

	// cout << L << endl;
	// cout << "2: ray.dir " << ray.dir << endl;

	double a = pow(glm::length(ray.dir), 2);
	double b = 2 * glm::dot(ray.dir, L);
	double c = pow(glm::length(L), 2.0f) - pow(radius, 2.0f);

		// cout << "2: a b c " << a << " " << b << " " << c << endl;

	return findPosRoot(a, b, c, t0);
}

glm::vec3 NonhierSphere::normalAt(glm::vec3 point) {
	return (point - m_pos) / (float)m_radius; // uh-oh
}


NonhierBox::~NonhierBox()
{
}

bool NonhierBox::intersect(const glm::vec3 & eye, const Ray & ray, double & t0) {
	// cout << "nonhierbox intersect" << endl;

	// based on  https://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
	double tmin = INT_MIN, tmax = INT_MAX;

	double boxMin[3], boxMax[3];
	for (int i = 0; i < 3; i += 1) {
		boxMin[i] = m_pos[i] - (m_size/2);
		boxMax[i] = m_pos[i] + (m_size/2);

	}

	/*for (int i = 0; i < 3; i += 1) {
		tmin = (boxMin[i] - ray.pos[i]) / r.dir[i];
		tmax = (boxMin[i] - ray.pos[i]) / r.dir[i];
		if (tmin > tmax) swap(tmin, tmax);
	}*/


	#if 1
	for (int i = 0; i < 3; i += 1) {

		// cout << i << ": " << boxMin << " " << boxMax << endl;

		double t1 = (boxMin[i] - ray.pos[i]) / (ray.dir)[i];
		double t2 = (boxMax[i] - ray.pos[i]) / (ray.dir)[i];

		// cout << i << ": " << t1 << " " << t2 << endl;

		tmin = max(tmin, min(t1, t2));
		tmax = min(tmax, max(t1, t2));
	}

	// cout << "min & max: " << tmin << " " << tmax << endl;

	if ( tmax > max(tmin, 0.0) ) {
		t0 = tmin;
		cout << t0 << endl;
		return true;
	}
	// cout << "nope" << endl;
	return false;
	#endif
	#if 0
	for (int i = 0; i < 3; i += 1) {
		double boxMin = m_pos[i] - (m_size/2);
		double boxMax = m_pos[i] + (m_size/2);

		if (ray.dir[i] != 0.0) {
			

			double t1 = (boxMin - ray.pos[i]) * (-1.0f * ray.dir)[i];
			double t2 = (boxMax - ray.pos[i]) * (-1.0f * ray.dir)[i];

			cout << i << ": " << t1 << " " << t2 << endl;

			tmin = max(tmin, min(t1, t2));
			tmax = min(tmax, max(t1, t2));
		} else if (ray.pos[i] <= boxMin || ray.pos[i] >= boxMax) {
			cout << "nope" << endl;
			return false;
		}
		
	}

	cout << " min & max: " << tmin << " " << tmax << endl;

	if ( tmax > tmin && tmin > 0.0 ) {
		t0 = tmin;
		cout << t0 << endl;
		return true;
	}
	cout << "nope" << endl;
	return false;
	#endif
}

glm::vec3 NonhierBox::normalAt(const glm::vec3 point) {
	// based on http://blog.johnnovak.net/2016/10/22/the-nim-raytracer-project-part-4-calculating-box-normals/
	glm::vec3 normPos = point - m_pos;
	glm::vec3 normal = glm::normalize(glm::vec3(
		(int)normPos.x / m_size,
		(int)normPos.y / m_size,
		(int)normPos.z / m_size
		));
}