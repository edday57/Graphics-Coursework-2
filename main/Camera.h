#ifndef CAMERA_H
#define CAMERA_H
#include "../vmlib/mat44.hpp"

class Camera
{
public:
	Mat44f getViewMatrix() const;
	virtual void setPosition(const Vec3f position) {}
	virtual void rotate(float yaw, float pitch) {} //in degrees
	virtual void move (const Vec3f offsetPos) {}

	const Vec3f getLook() const;
	const Vec3f getRight() const;
	const Vec3f getUp() const;

	float getFOV() const { return mFOV; }
	void setFOV(float fov) { mFOV = fov; }

protected:
	Camera();

	virtual void updateCameraVetors() {}

	Vec3f mPosition;
	Vec3f mTargetPos;
	Vec3f mUp;
	Vec3f mLook;
	Vec3f mRight;
	const Vec3f WORLD_UP;

	//In radians
	float mYaw;
	float mPitch;

	//Camera parameter
	float mFOV; //degrees

};

class FPSCamera : public Camera
{
public:
	FPSCamera(Vec3f position = { 0.0f, 0.0f, 0.0f }, float yaw = 3.1415926f, float pitch = 0.0f);

	virtual void setPosition(const Vec3f position);
	virtual void rotate(float yaw, float pitch); //degrees
	virtual void move(const Vec3f offsetPos);

private:
	void updateCameraVectors();
};

class OrbitCamera : public Camera
{
public:
	OrbitCamera();

	virtual void rotate(float yaw, float pitch); //in degrees
	void setLookAt(const Vec3f target);
	void setRadius(float radius);

private:
	float mRadius;
	void updateCameraVectors();
};

#endif