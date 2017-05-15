/// <summary>
/// 自機に追従するカメラ
/// </summary>
#include "pch.h"
#include "FollowCamera.h"
/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;

/* -- 定数の定義 ---- */
const float FollowCamera::CAMER_DISTANSE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
	,m_targetPos(Vector3::Zero)
	,m_targetAngle(0.0f)
{

}


FollowCamera::~FollowCamera()
{
}

/// <summary>
/// 更新
/// </summary>
void FollowCamera::update()
{
	Vector3 eyePos, refPos;

	refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);

	Vector3 cameraV = Vector3(0.0f, 0.0f, CAMER_DISTANSE);

	Matrix rotMat = Matrix::CreateRotationY(m_targetAngle);

	cameraV = Vector3::TransformNormal(cameraV, rotMat);

	eyePos = refPos + cameraV;

	setEyePos(eyePos);
	setRefPos(refPos);

	Camera::update();
}

/// <summary>
/// 追従対象の座標をセットする
/// </summary>
/// <param name="tagetPos"></param>
void FollowCamera::setTargetPos(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_targetPos = targetPos;
}

/// <summary>
/// 追従対象の角度をセットする
/// </summary>
/// <param name="targetAngle"></param>
void FollowCamera::setTargetAngle(const float & targetAngle)
{
	m_targetAngle = targetAngle;
}
