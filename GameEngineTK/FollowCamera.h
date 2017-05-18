/// <summary>
/// 自機に追従するカメラ
/// </summary>
/// 
/* -- ヘッダーのインクルード ---- */
#include "Camera.h"
#include <Keyboard.h>

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

	// キーボードのセット
	void setKeyBoard(DirectX::Keyboard* keyBoard);

	// TPSカメラの初期化
	void initiarizeTPS();
protected:
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	// 追従対象の角度
	float m_targetAngle;

	// キーボード
	DirectX::Keyboard* m_keyBoard;
	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyBoarTracker;

private:
	// 座標の線形補完用
	DirectX::SimpleMath::Vector3 lerp(DirectX::SimpleMath::Vector3 startPos,DirectX::SimpleMath::Vector3 targetPos,float Proportion);

	// FPSとTPSの切り替えようフラグ
	bool m_isTPS;
};

