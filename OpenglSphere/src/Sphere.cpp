#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

static const double pi180 = M_PI / 180;

double degreeToRadian(float degrees)
{
	return (degrees * pi180);
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
		for (; azimuth_d < 180; azimuth_d += delta_d)
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

			/*---------------------------------*/
			// 360 - azimuth
			triangle.negativeY();
			triangles.push_back(triangle);

			triangle.vertices[0] = pointCur;
			triangle.negativeY();
			triangles.push_back(triangle);
			/*---------------------------------*/

			pointCur = pointNext;
			pointTop = pointTopNext;
			azimuthCurRad = azimuthNextRad;
		}
		polarCur = polarNext;
		zcoordCur = zcoordNext;
	}

	return triangles;
}
