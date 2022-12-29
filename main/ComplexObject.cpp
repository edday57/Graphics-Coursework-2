#include "ComplexObject.hpp"

#include "../vmlib/mat33.hpp"

// Creates a cylinder object.
SimpleMeshData make_cylinder(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform)
{
	// The positions and normals
	std::vector<Vec3f> pos;
	std::vector<Vec3f> norm;

	// The previous y and z coordinates
	float prevY = std::cos(0.f);
	float prevZ = std::sin(0.f);

	// Loops through all the sub divides of the cylinder
	for (std::size_t i = 0; i < aSubdivs; ++i) {

		// The angle and the y and z coordinates
		float const angle = (i + 1) / float(aSubdivs) * 2.f * 3.1415926f;
		float y = std::cos(angle);
		float z = std::sin(angle);

		// Two triangles (= 3 * 2 positions) create one segment of the cylinder’s shell.
		pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
		pos.emplace_back(Vec3f{ 0.f, y, z });
		pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });

		pos.emplace_back(Vec3f{ 0.f, y, z });
		pos.emplace_back(Vec3f{ 1.f, y, z });
		pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });

		// The normals of these triangles.
		norm.emplace_back(Vec3f{ 0.f, prevY, prevZ });
		norm.emplace_back(Vec3f{ 0.f, y, z });
		norm.emplace_back(Vec3f{ 0.f, prevY, prevZ });

		norm.emplace_back(Vec3f{ 0.f, y, z });
		norm.emplace_back(Vec3f{ 0.f, y, z });
		norm.emplace_back(Vec3f{ 0.f, prevY, prevZ });

		// if the cylinder is capped then more triangles will be added.
		if (aCapped == true) {
			// The triangles for the two caps
			pos.emplace_back(Vec3f{ 0.f, y, z });
			pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
			pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });

			pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });
			pos.emplace_back(Vec3f{ 1.f, y, z });
			pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });

			// The normals for the cap triangles
			norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });

			norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
		}

		// Updating the previous y and z to be the current ones
		prevY = y;
		prevZ = z;
	}

	// The colour
	std::vector col(pos.size(), aColor);

	// Adding the preTransform to the positions
	for (auto& p : pos) {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}

	// Adding the preTransform to the normals
	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
	for (auto& n : norm) {
		n = normalize(N * n);
	}

	// Return the data
	return SimpleMeshData{ std::move(pos), std::move(col), std::move(norm) };
}

// Creates a cube object.
SimpleMeshData make_cube(Vec3f aColor, Mat44f aPreTransform)
{
	// The positions and normals
	std::vector<Vec3f> pos;
	std::vector<Vec3f> norm;

	// The top of the cube
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });

	// The front of the cube
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });

	// The left of the cube
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });

	// The bottom of the cube
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });

	// The right of the cube
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });

	// The back of the cube
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });

	// The normals
	norm.emplace_back(Vec3f{ 0.f, 0.f, -1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, -1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, -1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, -1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, -1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, -1.f });

	norm.emplace_back(Vec3f{ 0.f, 1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, 1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, 1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, 1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, 1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, 1.f, 0.f });

	norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });

	norm.emplace_back(Vec3f{ 0.f, 0.f, 1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, 1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, 1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, 1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, 1.f });
	norm.emplace_back(Vec3f{ 0.f, 0.f, 1.f });

	norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
	norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });

	norm.emplace_back(Vec3f{ 0.f, -1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, -1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, -1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, -1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, -1.f, 0.f });
	norm.emplace_back(Vec3f{ 0.f, -1.f, 0.f });

	// The colour
	std::vector col(pos.size(), aColor);

	// Adding the preTransform to the positions
	for (auto& p : pos) {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}

	// Adding the preTransform to the normals
	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
	for (auto& n : norm) {
		n = normalize(N * n);
	}

	// Return the data
	return SimpleMeshData{ std::move(pos), std::move(col), std::move(norm) };
}

// Creates a fence object.
// Takes the number of fences and the transformation to be appplied on them all.
SimpleMeshData make_fence(std::size_t aSubdivs, Mat44f aPreTransform)
{
	// Values to be itterated for when there are several fences.
	float b = 0, w1 = 0, w2 = 0;

	// The fence
	SimpleMeshData fence;

	// Loops through the number of fences wanted (aSubdivs) and creates them next to each other (to the right).
	// The preTransform is also applied to each part of the fence.
	for (std::size_t i = 0; i < aSubdivs; ++i) {
		// The cylinder parts of the fence (bars)
		auto bar1 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 0.f, 0.f }));
		auto bar2 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 5.f, 0.f }));
		auto bar3 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 10.f, 0.f }));
		auto bar4 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 15.f, 0.f }));
		auto bar5 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 20.f, 0.f }));
		auto bar6 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 25.f, 0.f }));

		// The cube parts of the fence (walls)
		auto wall1 = make_cube({ 1.f, 1.f, 1.f }, aPreTransform * make_scaling(1.f, 3.f, 1.f) * make_translation({ w1 + -1.f, 0.f, 0.f }));
		auto wall2 = make_cube({ 1.f, 1.f, 1.f }, aPreTransform * make_scaling(1.f, 3.f, 1.f) * make_translation({ w1 + 5.f, 0.f, 0.f }));
		auto wall3 = make_cube({ 1.f, 1.f, 1.f }, aPreTransform * make_scaling(2.f, 1.f, 0.5f) * make_translation({ w2 + 1.f, -1.5f, 0.f }));

		// Combining these shapes to create the fence
		auto bars12 = concatenate(std::move(bar1), bar2);
		auto bars123 = concatenate(std::move(bars12), bar3);
		auto bars1234 = concatenate(std::move(bars123), bar4);
		auto bars12345 = concatenate(std::move(bars1234), bar5);
		auto bars123456 = concatenate(std::move(bars12345), bar6);

		auto walls12 = concatenate(std::move(wall1), wall2);
		auto walls123 = concatenate(std::move(walls12), wall3);

		auto fencePart = concatenate(std::move(bars123456), walls123);

		// Combining the current fence with the previous one
		fence = concatenate(std::move(fence), fencePart);

		// Updating the counter variables
		b = b + 1.f;
		w1 = w1 + 6.f;
		w2 = w2 + 3.f;
	}

	// Return the fence object
	return fence;
}