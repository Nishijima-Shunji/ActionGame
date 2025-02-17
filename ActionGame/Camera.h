#pragma once
#include <SimpleMath.h>
class Camera
{
private:
	DirectX::XMFLOAT3	Camera_Speed = { 300.0f, 300.0f, 0.0f };
	int count = 0, move_count = 0;
public:
	DirectX::SimpleMath::Vector3 Camera_Pos = { 0.0f, 0.0f, 1.0f };

	static Camera& GetInstance() {
		static Camera instance;
		return instance;
	}

	void Update();
	void MoveCamera(float new_x, float new_y, float new_z, int count);
};

#define g_Camera Camera::GetInstance()

