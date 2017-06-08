/// <summary>
/// 自機に追従するカメラ
/// </summary>
#include "pch.h"
#include "FollowCamera.h"
/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;

/* -- 定数の定義 ---- */
const float FollowCamera::CAMER_DISTANSE = 6.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
	,m_targetPos(Vector3::Zero)
	,m_targetAngle(0.0f)
	, m_keyBoard(nullptr)
	, m_isTPS(true)
{
	initiarizeTPS();
}


FollowCamera::~FollowCamera()
{
}

/// <summary>
/// 更新
/// </summary>
void FollowCamera::update()
{
	// 次にカメラが目指す座標
	Vector3 nextEyePos, nextRefPos;

	// キーボードの状態を取得
	Keyboard::State keyState = m_keyBoard->GetState();
	m_keyBoarTracker.Update(keyState);
	// Cを押したらフラグを切り替える
	if (m_keyBoarTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		if (m_isTPS)
		{
			m_isTPS = false;
		}
		else
		{
			m_isTPS = true;
			initiarizeTPS();
		}
	}

	setTargetPos(m_targetPointa->getTrance());
	setTargetAngle(m_targetPointa->getRotation().y);


	// FPSカメラ用
	Vector3 fpsCameraPos;


	// 参照点から、視点への差分ベクトル
	Vector3 cameraV = Vector3(0.0f, 0.0f, CAMER_DISTANSE);

	// 自機の後ろに回り込むための回転行列
	Matrix rotMat = Matrix::CreateRotationY(m_targetAngle);

	cameraV = Vector3::TransformNormal(cameraV, rotMat);

	// フラグにより処理を変更
	if (m_isTPS)
	{
		nextRefPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
		nextEyePos = nextRefPos + cameraV;
		 // 視点を現在位置から補完
		nextEyePos = lerp(m_eyePos, nextEyePos, 0.1f);
		// 参照店を現在位置から補完
		nextRefPos = lerp(m_refPos, nextRefPos, 0.2f);
	}
	else
	{
		nextEyePos = m_targetPos + Vector3(0.0f, 0.4f, 0.0f);
		nextRefPos = nextEyePos - cameraV;
	}

	setEyePos(nextEyePos);
	setRefPos(nextRefPos);

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

/// <summary>
/// キーボードのセット
/// </summary>
/// <param name="keyBoard">セットするキーボード</param>
void FollowCamera::setKeyBoard(DirectX::Keyboard * keyBoard)
{
	m_keyBoard = keyBoard;
}

/// <summary>
/// TPSカメラの初期化
/// </summary>
void FollowCamera::initiarizeTPS()
{
	// 次にカメラが目指す座標
	Vector3 nextEyePos, nextRefPos;

	// 参照点から、視点への差分ベクトル
	Vector3 cameraV = Vector3(0.0f, 0.0f, CAMER_DISTANSE);

	// 自機の後ろに回り込むための回転行列
	Matrix rotMat = Matrix::CreateRotationY(m_targetAngle);

	cameraV = Vector3::TransformNormal(cameraV, rotMat);
	nextRefPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
	nextEyePos = nextRefPos + cameraV;

	setEyePos(nextEyePos);
	setRefPos(nextRefPos);

}

/// <summary>
/// 座標の線形補完用
/// </summary>
/// <param name="startPos">現在の座標</param>
/// <param name="targetPos">目的の座標</param>
/// <returns>補完後の座標</returns>
DirectX::SimpleMath::Vector3 FollowCamera::lerp(DirectX::SimpleMath::Vector3 startPos,DirectX::SimpleMath::Vector3 targetPos, float Proportion)
{
	return startPos + (targetPos - startPos)*Proportion;
}
