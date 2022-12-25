#ifndef COMPLEX_OBJECT
#define COMPLEX_OBJECT

#include <vector>

#include <cstdlib>

#include "SimpleMesh.h"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"


SimpleMeshData make_cylinder(
	bool aCapped = true,
	std::size_t aSubdivs = 16,
	Vec3f aColor = { 1.f, 1.f, 1.f },
	Mat44f aPreTransform = kIdentity44f
);

SimpleMeshData make_cube(
	Vec3f aColor = { 1.f, 1.f, 1.f },
	Mat44f aPreTransform = kIdentity44f
);

SimpleMeshData make_fence(
	std::size_t aSubdivs = 1,
	Mat44f aPreTransform = kIdentity44f
);

#endif // COMPLEX_OBJECT
