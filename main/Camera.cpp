#include "Camera.h"

const float DEF_FOV = 45.0f;


Camera::Camera()
	:mPosition(Vec3f{ 0.0f, 0.0f, 0.0f }),
	mTargetPos(Vec3f{ 0.0f, 0.0f, 0.0f }),
	mUp(Vec3f{ 0.0f, 1.0f, 0.0f }),
	mRight(Vec3f{ 0.0f, 0.0f, 0.0f }),
	WORLD_UP(Vec3f{ 0.0f, 1.0f, 0.0f }),
	mYaw(0.0f),
	mPitch(0.f),
	mFOV(DEF_FOV)
{

}

Mat44f Camera::getViewMatrix() const
{
	return lookAt(mPosition, mTargetPos, mUp);
}

const Vec3f Camera::getLook() const
{
	return mLook;
}

const Vec3f Camera::getRight() const
{
	return mRight;
}

const Vec3f Camera::getUp() const
{
	return mUp;
}

//FPS Camera
FPSCamera::FPSCamera(Vec3f position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

void FPSCamera::setPosition(const Vec3f position)
{
	mPosition = position;
}

void FPSCamera::move(const Vec3f offsetPos)
{
	mPosition += offsetPos;
	updateCameraVetors();
}

void FPSCamera::rotate(float yaw, float pitch) {
	mYaw += makeRadians(yaw);
	mPitch += makeRadians(pitch);
	float pi = 3.1415926f;
	mPitch = clamp(mPitch, -pi / 2.0f + 0.1f, pi / 2.0f - 0.1f);
	updateCameraVectors();
}

void FPSCamera::updateCameraVectors() {
	//Convert spherical to cartesian coords
	//Calculate view direction vector from yaw and pitch
	Vec3f look;
	look.x = cosf(mPitch)* sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);
	mLook = normalize(look);

	//Recalculate right and up vec
	mRight = normalize(cross(mLook, WORLD_UP));
	mUp = normalize(cross(mRight, mLook));

	mTargetPos = mPosition + mLook;

}

//Orbit Camera
OrbitCamera::OrbitCamera()
	:mRadius(10.f)
{}

void OrbitCamera::setLookAt(const Vec3f target) {
	mTargetPos = target;
}
void OrbitCamera::setRadius(float radius) {
	mRadius = clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
	mYaw = makeRadians(yaw);
	mPitch = makeRadians(pitch);
	float pi = 3.1415926f;
	mPitch = clamp(mPitch, -pi / 2.0f + 0.1f, pi / 2.0f - 0.1f);

	updateCameraVectors();
}

void OrbitCamera::updateCameraVectors() {
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}