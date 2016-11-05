#include <iostream>
#include <fstream>

#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

// based on https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
bool Mesh::intersect(const glm::vec3 & eye, const Ray & ray, double & t0, glm::vec3 & normal) {
	// std::cout << "Mesh intersect" << std::endl;

	float tmin = INT_MAX;

	for ( Triangle triangle : m_faces ) {
		double v0 = triangle.v1;
		double v1 = triangle.v2;
		double v2 = triangle.v3;
		glm::vec3 p0 = m_vertices.at(v0);
		glm::vec3 p1 = m_vertices.at(v1);
		glm::vec3 p2 = m_vertices.at(v2);

		glm::vec3 planeNorm = glm::normalize(glm::cross(p2 - p1, p1 - p0));

		triangle.normal = planeNorm;

		double D = -v0*p0.x - v1*p0.y - v2*p0.z;

		double t = -(glm::dot(eye, planeNorm) + D) / glm::dot(ray.dir, planeNorm);

		if ( output ) {
			// std::cout << p0 << " " << p1 << " " << p2 << std::endl;
			// std::cout << planeNorm << " " << t << std::endl;
			output = false;
		}

		if ( t < tmin ) {
			tmin = t;
		}
	}

	// make sure t > 0

	// Here, I was attempting to use Cramer's rule to see if the point lay
	// within the triangle, but ran out of time.
	// based on https://www.cs.utah.edu/~shirley/books/fcg2/rt.pdf (section 10.3.2)
	/*	if ( t < 0.0 || t == INT_MAX ) {
			return false;
		}

		// now check if point inside triangle, Cramer's method

		glm::vec3 P = eye + (t - 0.01f)*ray.dir;

		double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);

		double s = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;

		double t = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;

		if (s < 0 || t < 0 || s+t > 1.0) {
			return false;
		}*/

	return false;
}
