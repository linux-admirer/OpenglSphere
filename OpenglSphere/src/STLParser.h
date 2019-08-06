#pragma once

#include <string>
#include <vector>

struct Triangle;

class STLParser
{
public:
	STLParser();
	~STLParser();

	void writeSTL(std::string const& filePath, std::vector<Triangle> const& triangles);
};
