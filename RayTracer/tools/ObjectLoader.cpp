#pragma once

#include "ObjectLoader.h"

#include <sstream>

#include "Vertex.h"

namespace util {
std::vector<shapes::Triangle> loadObj(
    std::istream& in, const std::shared_ptr<material::Material>& material) {
	std::vector<Vertex> vertices;
	std::vector<Vec3> v;
	// std::vector<Vec3> vt;
	// std::vector<Vec3> vn;

	std::string lineStr;
	while (std::getline(in, lineStr)) {
		std::istringstream lineSS(lineStr);
		std::string lineType;
		lineSS >> lineType;

		// vertex
		if (lineType == "v") {
			float x = 0, y = 0, z = 0;
			lineSS >> x >> y >> z;
			v.push_back(Vec3(x, y, z));
		}

		// texture
		/*if (lineType == "vt") {
		    float u = 0, v = 0;
		    lineSS >> u >> v;
		    vt.push_back(Vec3(u, v, 0));
		}*/

		// normal
		/*if (lineType == "vn") {
		    float i = 0, j = 0, k = 0;
		    lineSS >> i >> j >> k;
		    vn.push_back(Vec3(i, j, k).normalize());
		}*/

		// polygon
		if (lineType == "f") {
			std::string refStr;
			while (lineSS >> refStr) {
				std::istringstream ref(refStr);
				std::string vStr
				    //, vtStr, vnStr
				    ;
				std::getline(ref, vStr, '/');
				// std::getline(ref, vtStr, '/');
				// std::getline(ref, vnStr, '/');
				int currentv = atoi(vStr.c_str());
				// int currentvt = atoi(vtStr.c_str());
				// int currentvn = atoi(vnStr.c_str());
				currentv = (currentv >= 0 ? currentv : v.size() + currentv);
				/*currentvt =
				    (currentvt >= 0 ? currentvt : vt.size() + currentvt);
				currentvn =
				    (currentvn >= 0 ? currentvn : vn.size() + currentvn);*/
				Vertex vert;
				vert.position = v[currentv - 1];
				/*vert.texcoord = v[currentvt - 1];
				vert.normal = v[currentvn - 1];*/
				vertices.push_back(vert);
			}
		}
	}
	std::vector<shapes::Triangle> triangles;
	for (int i = 0; i < vertices.size(); i += 3) {
		triangles.push_back(
		    {vertices[i + 0], vertices[i + 1], vertices[i + 2], material});
	}
	return triangles;
}
}  // namespace util