#include "Sphere.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

static const double pi180 = M_PI / 180;

double degreeToRadian(float degrees)
{
	return (degrees * pi180);
}

void negateY(Triangle& triangle)
{
	auto& vertices = triangle.vertices;
	vertices[0].y = -vertices[0].y;
	vertices[1].y = -vertices[1].y;
	vertices[2].y = -vertices[2].y;
}

void negateX(Triangle& triangle)
{
	auto& vertices = triangle.vertices;
	vertices[0].x = -vertices[0].x;
	vertices[1].x = -vertices[1].x;
	vertices[2].x = -vertices[2].x;
}

void swapXY(Triangle& triangle)
{
	auto& vertices = triangle.vertices;
	std::swap(vertices[0].x, vertices[0].y);
	std::swap(vertices[0].x, vertices[0].y);
	std::swap(vertices[0].x, vertices[0].y);
}

std::vector<Triangle> Sphere::getCoordinates(float radius, float delta_d)
{
	std::vector<Triangle> triangles;
	triangles.reserve(static_cast<size_t>(180 * 360 * 2/ (delta_d * delta_d)));

	 //x = r cos(azimuth_d) sin(polar_d)
	 //y = r sin(azimuth_d) sin(polar_d)
	 //z = r cos(polar_d)

	float polar_d = 0.;

	double polarCurRad = degreeToRadian(polar_d);
	float polarCur = radius * std::sin(polarCurRad);
	float zcoordCur = radius * std::cos(polarCurRad);
	for (; polar_d < 180; polar_d += delta_d)
	{
		double polarNextRad = degreeToRadian(polar_d + delta_d);
		float polarNext = radius * std::sin(polarNextRad);
		float zcoordNext = radius * std::cos(polarNextRad);

		float azimuth_d = 0.;
		double azimuthCurRad = degreeToRadian(azimuth_d);
		Point pointCur;
		pointCur.x = polarCur * std::cos(azimuthCurRad);
		pointCur.y = polarCur * std::sin(azimuthCurRad);
		pointCur.z = zcoordCur;

		Point pointTop;
		pointTop.x = polarNext * std::cos(azimuthCurRad);
		pointTop.y = polarNext * std::sin(azimuthCurRad);
		pointTop.z = zcoordNext;

		Triangle triangle;
		for (; azimuth_d < 90; azimuth_d += delta_d)
		{
			double azimuthNextRad = degreeToRadian(azimuth_d + delta_d);
			Point pointNext;
			pointNext.x = polarCur * std::cos(azimuthNextRad);
			pointNext.y = polarCur * std::sin(azimuthNextRad);
			pointNext.z = zcoordCur;

			triangle.vertices[0] = pointCur;
			triangle.vertices[1] = pointNext;
			triangle.vertices[1] = pointTop;

			triangles.push_back(triangle);

			Point pointTopNext;
			pointTopNext.x = polarNext * std::cos(azimuthNextRad);
			pointTopNext.y = polarNext * std::sin(azimuthNextRad);
			pointTopNext.z = zcoordNext;

			triangle.vertices[0] = pointTopNext;
			triangles.push_back(triangle);

			pointCur = pointNext;
			pointTop = pointTopNext;
			azimuthCurRad = azimuthNextRad;
		}
		polarCur = polarNext;
		zcoordCur = zcoordNext;
	}

	auto size = triangles.size();
	Triangle triangle;
	for (size_t i = 0; i < size; ++i)
	{
		// 180 + theta
		triangle = triangles[i];
		negateX(triangle);
		negateY(triangle);
		triangles.push_back(triangle);

		// 90 + theta
		triangle = triangles[i];
		swapXY(triangle);
		negateY(triangle);
		triangles.push_back(triangle);

		// 270 + theta
		negateX(triangle);
		negateY(triangle);
		triangles.push_back(triangle);
	}

	return triangles;
}
