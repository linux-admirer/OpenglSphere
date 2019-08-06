#include "STLParser.h"

#include <fstream>

#include "Sphere.h"

STLParser::STLParser()
{
}

STLParser::~STLParser()
{
}

void STLParser::writeSTL(std::string const& filePath, std::vector<Triangle> const& triangles)
{
	std::ofstream stream(filePath, std::ios_base::out);

	stream << "solid CustomObject\n";

	for (auto& triangle : triangles)
	{
		stream << "  facet normal 0.0 0.0 0.0 \n";
		stream << "    outer loop\n";
		stream << "      vertex " << triangle.vertices[0].x << " " << triangle.vertices[0].y << " " << triangle.vertices[0].z << "\n";
		stream << "      vertex " << triangle.vertices[1].x << " " << triangle.vertices[1].y << " " << triangle.vertices[1].z << "\n";
		stream << "      vertex " << triangle.vertices[2].x << " " << triangle.vertices[2].y << " " << triangle.vertices[2].z << "\n";
		stream << "    endloop\n";
		stream << "  endfacet\n";
	}

	stream << "endsolid\n";
	stream.close();
}
