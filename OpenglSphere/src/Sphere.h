#pragma once

#include <vector>

#pragma pack(push, 1)
struct Point
{
	Point() : Point(0., 0., 0.) {}

	Point(float x, float y, float z) :
		x(x), y(y), z(z)
	{
	}

	float x, y, z;
};

struct Triangle
{
	Point vertices[3];
};
#pragma pack(pop)

class Sphere
{
public:
	Sphere() = default;
	~Sphere() = default;

	std::vector<Triangle> getCoordinates(float radius, float deltaDegrees = 1.);
};
