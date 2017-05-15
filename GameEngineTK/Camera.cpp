/****************************************************************
*|　概要　　　　カメラクラス
*|　作成者　　　GS1 04 牛山航平
*|　作成日　　　2017/05/11
*****************************************************************/
#include "pch.h"
#include "Camera.h"

/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;



/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">画面の縦幅</param>
/// <param name="height">画面の横幅</param>
Camera::Camera(int width, int height)
{
	m_eyePos = Vector3(0, 0, 5.0f);
	m_refPos = Vector3(0, 0, 0);
	m_upVec = Vector3(0, 1, 0);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = static_cast<float>(width) / height;
	m_nearClip = 0.0001f;
	m_farClip = 100000000.0f;
}


/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// 更新処理
/// </summary>
void Camera::update()
{
	// ビュー行列を作る
	m_view = Matrix::CreateLookAt(m_eyePos,m_refPos,m_upVec);

	// 射影行列を作る
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}


/// <summary>
/// ビュー行列を取得
/// </summary>
/// <returns>ビュー行列</returns>
DirectX::SimpleMath::Matrix Camera::getViewMatrix()
{
	return m_view;
}

/// <summary>
/// 射影行列を取得
/// </summary>
/// <returns>射影行列</returns>
DirectX::SimpleMath::Matrix Camera::getProjMatrix()
{
	return m_proj;
}

/// <summary>
/// 視点座標をセット
/// </summary>
/// <param name="eyePos">視点座標</param>
void Camera::setEyePos(DirectX::SimpleMath::Vector3 eyePos)
{
	m_eyePos = eyePos;
}


/// <summary>
/// 注視点をセット
/// </summary>
/// <param name="refPos">注視点</param>
void Camera::setRefPos(DirectX::SimpleMath::Vector3 refPos)
{
	m_refPos = refPos;
}

/// <summary>
/// 上方向ベクトルをセット
/// </summary>
/// <param name="UpVec">上方向ベクトル</param>
void Camera::setUpVec(DirectX::SimpleMath::Vector3 UpVec)
{
	m_upVec = UpVec;
}
