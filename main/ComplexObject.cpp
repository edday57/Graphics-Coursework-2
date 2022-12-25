#include "ComplexObject.hpp"

#include "../vmlib/mat33.hpp"

SimpleMeshData make_cylinder(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform)
{
	//TODO: implement me
	std::vector<Vec3f> pos;

	std::vector<Vec3f> norm;

	float prevY = std::cos(0.f);
	float prevZ = std::sin(0.f);

	for (std::size_t i = 0; i < aSubdivs; ++i) {
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

		// normals
		norm.emplace_back(Vec3f{ 0.f, prevY, prevZ });
		norm.emplace_back(Vec3f{ 0.f, y, z });
		norm.emplace_back(Vec3f{ 0.f, prevY, prevZ });

		norm.emplace_back(Vec3f{ 0.f, y, z });
		norm.emplace_back(Vec3f{ 0.f, y, z });
		norm.emplace_back(Vec3f{ 0.f, prevY, prevZ });


		if (aCapped == true) {
			pos.emplace_back(Vec3f{ 0.f, y, z });
			pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
			pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });

			pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });
			pos.emplace_back(Vec3f{ 1.f, y, z });
			pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });

			// normals
			norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ -1.f, 0.f, 0.f });

			norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
			norm.emplace_back(Vec3f{ 1.f, 0.f, 0.f });

		}

		prevY = y;
		prevZ = z;
	}

	std::vector col(pos.size(), aColor);

	for (auto& p : pos) {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}
	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform))); //

	for (auto& n : norm) {
		n = normalize(N * n);
	}

	return SimpleMeshData{ std::move(pos), std::move(col), std::move(norm) };
}

SimpleMeshData make_cube(Vec3f aColor, Mat44f aPreTransform)
{
	std::vector<Vec3f> pos;

	std::vector<Vec3f> norm;

	// top
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });

	// front
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });

	// left
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });

	// bottom
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, +1.f });

	// right
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, +1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, +1.f });

	// back
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ -1.f, -1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, +1.f, -1.f });
	pos.emplace_back(Vec3f{ +1.f, -1.f, -1.f });

	// Normals
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

	std::vector col(pos.size(), aColor);

	for (auto& p : pos) {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}

	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform))); //

	for (auto& n : norm) {
		n = normalize(N * n);
	}

	return SimpleMeshData{ std::move(pos), std::move(col), std::move(norm) };
}

SimpleMeshData make_fence(std::size_t aSubdivs, Mat44f aPreTransform)
{
	float b = 0, w1 = 0, w2 = 0;

	SimpleMeshData fence;

	for (std::size_t i = 0; i < aSubdivs; ++i) {
		auto bar1 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 0.f, 0.f }));
		auto bar2 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 5.f, 0.f }));
		auto bar3 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 10.f, 0.f }));
		auto bar4 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 15.f, 0.f }));
		auto bar5 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 20.f, 0.f }));
		auto bar6 = make_cylinder(true, 128, { 0.4f, 0.4f, 0.4f }, aPreTransform * make_scaling(5.f, 0.1f, 0.1f) * make_translation({ b, 25.f, 0.f }));

		auto wall1 = make_cube({ 1.f, 1.f, 1.f }, aPreTransform * make_scaling(1.f, 3.f, 1.f) * make_translation({ w1 + -1.f, 0.f, 0.f }));
		auto wall2 = make_cube({ 1.f, 1.f, 1.f }, aPreTransform * make_scaling(1.f, 3.f, 1.f) * make_translation({ w1 + 5.f, 0.f, 0.f }));
		auto wall3 = make_cube({ 1.f, 1.f, 1.f }, aPreTransform * make_scaling(2.f, 1.f, 0.5f) * make_translation({ w2 + 1.f, -1.5f, 0.f }));

		auto bars12 = concatenate(std::move(bar1), bar2);
		auto bars123 = concatenate(std::move(bars12), bar3);
		auto bars1234 = concatenate(std::move(bars123), bar4);
		auto bars12345 = concatenate(std::move(bars1234), bar5);
		auto bars123456 = concatenate(std::move(bars12345), bar6);

		auto walls12 = concatenate(std::move(wall1), wall2);
		auto walls123 = concatenate(std::move(walls12), wall3);

		auto fencePart = concatenate(std::move(bars123456), walls123);

		fence = concatenate(std::move(fence), fencePart);

		b = b + 1.f;
		w1 = w1 + 6.f;
		w2 = w2 + 3.f;
	}

	return fence;
}