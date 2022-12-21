#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

/** Mat44f: 4x4 matrix with floats
 *
 * See vec2f.hpp for discussion. Similar to the implementation, the Mat44f is
 * intentionally kept simple and somewhat bare bones.
 *
 * The matrix is stored in row-major order (careful when passing it to OpenGL).
 *
 * The overloaded operator () allows access to individual elements. Example:
 *    Mat44f m = ...;
 *    float m12 = m(1,2);
 *    m(0,3) = 3.f;
 *
 * The matrix is arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
struct Mat44f
{
	float v[16];

	constexpr
		float& operator() (std::size_t aI, std::size_t aJ) noexcept
	{
		assert(aI < 4 && aJ < 4);
		return v[aI * 4 + aJ];
	}
	constexpr
		float const& operator() (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert(aI < 4 && aJ < 4);
		return v[aI * 4 + aJ];
	}
};

// Identity matrix
constexpr Mat44f kIdentity44f = { {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
} };

// Common operators for Mat44f.
// Note that you will need to implement these yourself.

constexpr
Mat44f operator*(Mat44f const& aLeft, Mat44f const& aRight) noexcept
{
	Mat44f output = kIdentity44f;
	output(0, 0) = aLeft(0, 0) * aRight(0, 0) + aLeft(0, 1) * aRight(1, 0) + aLeft(0, 2) * aRight(2, 0) + aLeft(0, 3) * aRight(3, 0);
	output(0, 1) = aLeft(0, 0) * aRight(0, 1) + aLeft(0, 1) * aRight(1, 1) + aLeft(0, 2) * aRight(2, 1) + aLeft(0, 3) * aRight(3, 1);
	output(0, 2) = aLeft(0, 0) * aRight(0, 2) + aLeft(0, 1) * aRight(1, 2) + aLeft(0, 2) * aRight(2, 2) + aLeft(0, 3) * aRight(3, 2);
	output(0, 3) = aLeft(0, 0) * aRight(0, 3) + aLeft(0, 1) * aRight(1, 3) + aLeft(0, 2) * aRight(2, 3) + aLeft(0, 3) * aRight(3, 3);

	output(1, 0) = aLeft(1, 0) * aRight(0, 0) + aLeft(1, 1) * aRight(1, 0) + aLeft(1, 2) * aRight(2, 0) + aLeft(1, 3) * aRight(3, 0);
	output(1, 1) = aLeft(1, 0) * aRight(0, 1) + aLeft(1, 1) * aRight(1, 1) + aLeft(1, 2) * aRight(2, 1) + aLeft(1, 3) * aRight(3, 1);
	output(1, 2) = aLeft(1, 0) * aRight(0, 2) + aLeft(1, 1) * aRight(1, 2) + aLeft(1, 2) * aRight(2, 2) + aLeft(1, 3) * aRight(3, 2);
	output(1, 3) = aLeft(1, 0) * aRight(0, 3) + aLeft(1, 1) * aRight(1, 3) + aLeft(1, 2) * aRight(2, 3) + aLeft(1, 3) * aRight(3, 3);

	output(2, 0) = aLeft(2, 0) * aRight(0, 0) + aLeft(2, 1) * aRight(1, 0) + aLeft(2, 2) * aRight(2, 0) + aLeft(2, 3) * aRight(3, 0);
	output(2, 1) = aLeft(2, 0) * aRight(0, 1) + aLeft(2, 1) * aRight(1, 1) + aLeft(2, 2) * aRight(2, 1) + aLeft(2, 3) * aRight(3, 1);
	output(2, 2) = aLeft(2, 0) * aRight(0, 2) + aLeft(2, 1) * aRight(1, 2) + aLeft(2, 2) * aRight(2, 2) + aLeft(2, 3) * aRight(3, 2);
	output(2, 3) = aLeft(2, 0) * aRight(0, 3) + aLeft(2, 1) * aRight(1, 3) + aLeft(2, 2) * aRight(2, 3) + aLeft(2, 3) * aRight(3, 3);

	output(3, 0) = aLeft(3, 0) * aRight(0, 0) + aLeft(3, 1) * aRight(1, 0) + aLeft(3, 2) * aRight(2, 0) + aLeft(3, 3) * aRight(3, 0);
	output(3, 1) = aLeft(3, 0) * aRight(0, 1) + aLeft(3, 1) * aRight(1, 1) + aLeft(3, 2) * aRight(2, 1) + aLeft(3, 3) * aRight(3, 1);
	output(3, 2) = aLeft(3, 0) * aRight(0, 2) + aLeft(3, 1) * aRight(1, 2) + aLeft(3, 2) * aRight(2, 2) + aLeft(3, 3) * aRight(3, 2);
	output(3, 3) = aLeft(3, 0) * aRight(0, 3) + aLeft(3, 1) * aRight(1, 3) + aLeft(3, 2) * aRight(2, 3) + aLeft(3, 3) * aRight(3, 3);

	return output;

}

constexpr
Vec4f operator*(Mat44f const& aLeft, Vec4f const& aRight) noexcept
{
	Vec4f output = { aLeft(0,0) * aRight.x + aLeft(0,1) * aRight.y + aLeft(0,2) * aRight.z + aLeft(0,3) * aRight.w,
		aLeft(1,0) * aRight.x + aLeft(1,1) * aRight.y + aLeft(1,2) * aRight.z + aLeft(1,3) * aRight.w,
		aLeft(2,0) * aRight.x + aLeft(2,1) * aRight.y + aLeft(2,2) * aRight.z + aLeft(2,3) * aRight.w,
		aLeft(3,0) * aRight.x + aLeft(3,1) * aRight.y + aLeft(3,2) * aRight.z + aLeft(3,3) * aRight.w
	};
	return output;
}

// Functions:

inline
Mat44f make_rotation_x(float aAngle) noexcept
{
	Mat44f i = kIdentity44f;
	i(1, 1) = cos(aAngle);
	i(1, 2) = -sin(aAngle);
	i(2, 1) = sin(aAngle);
	i(2, 2) = cos(aAngle);

	return i;
}


inline
Mat44f make_rotation_y(float aAngle) noexcept
{
	Mat44f i = kIdentity44f;
	i(0, 0) = cos(aAngle);
	i(2, 0) = -sin(aAngle);
	i(0, 2) = sin(aAngle);
	i(2, 2) = cos(aAngle);

	return i;
}

inline
Mat44f make_rotation_z(float aAngle) noexcept
{
	Mat44f i = kIdentity44f;
	i(0, 0) = cos(aAngle);
	i(1, 0) = -sin(aAngle);
	i(0, 1) = -sin(aAngle);
	i(1, 1) = cos(aAngle);

	return i;
}

inline
Mat44f make_translation(Vec3f aTranslation) noexcept
{
	Mat44f i = kIdentity44f;
	i(0, 3) = aTranslation.x;
	i(1, 3) = aTranslation.y;
	i(2, 3) = aTranslation.z;


	return i;
}

inline
Mat44f make_perspective_projection(float aFovInRadians, float aAspect, float aNear, float aFar) noexcept
{
	Mat44f i = kIdentity44f;
	float s = 1 / tan((aFovInRadians / 2));
	i(0, 0) = s / aAspect;
	i(1, 1) = s;
	i(2, 2) = -(aFar + aNear) / (aFar - aNear);
	i(2, 3) = -2 * (aFar * aNear) / (aFar - aNear);
	i(3, 2) = -1;
	i(3, 3) = 0;
	return i;
}

inline
Mat44f make_scaling(float x, float y, float z) noexcept
{
	Mat44f i = kIdentity44f;
	i(0, 0) = x;
	i(1, 1) = y;
	i(2, 2) = z;


	return i;
}

inline
Mat44f lookAt(Vec3f position, Vec3f target, Vec3f worldUp) noexcept
{
	Vec3f zAxis = normalize(position - target);
	Vec3f xAxis = normalize(cross(normalize(worldUp), zAxis));
	Vec3f yAxis = cross(zAxis, xAxis);
	Mat44f translation = kIdentity44f;
	translation(0, 3) = -position.x;
	translation(1, 3) = -position.y;
	translation(2, 3) = -position.z;
	Mat44f rotation = kIdentity44f;
	rotation(0, 0) = xAxis.x;
	rotation(0, 1) = xAxis.y;
	rotation(0, 2) = xAxis.z;
	rotation(1, 0) = yAxis.x;
	rotation(1, 1) = yAxis.y;
	rotation(1, 2) = yAxis.z;
	rotation(2, 0) = zAxis.x;
	rotation(2, 1) = zAxis.y;
	rotation(2, 2) = zAxis.z;

	return rotation * translation;

}

inline 
float makeRadians(float angle) noexcept
{
	float pi = 3.1415926f;
	float rads = angle * (pi / 180);
	return rads;

}

inline 
float clamp(float value, float min, float max) noexcept
{
	if (value < min) {
		value = min;
	}
	if (value > max) {
		value = max;
	}
	return value;
}

#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
