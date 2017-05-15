/// <summary>
/// 自機に追従するカメラ
/// </summary>
/// 
/* -- ヘッダーのインクルード ---- */
#include "Camera.h"

#pragma once
class FollowCamera : public Camera
{
public:
	FollowCamera(int width, int height);
	~FollowCamera();

	// 更新
	void update()override;

	// 追従対象をセット
	void setTargetPos(const DirectX::SimpleMath::Vector3& targetPos);
	void setTargetAngle(const float& targetAngle);

	static const float CAMER_DISTANSE;

protected:
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	// 追従対象の角度
	float m_targetAngle;
};

