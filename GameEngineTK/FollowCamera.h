/// <summary>
/// ���@�ɒǏ]����J����
/// </summary>
/// 
/* -- �w�b�_�[�̃C���N���[�h ---- */
#include "Camera.h"

#pragma once
class FollowCamera : public Camera
{
public:
	FollowCamera(int width, int height);
	~FollowCamera();

	// �X�V
	void update()override;

	// �Ǐ]�Ώۂ��Z�b�g
	void setTargetPos(const DirectX::SimpleMath::Vector3& targetPos);
	void setTargetAngle(const float& targetAngle);

	static const float CAMER_DISTANSE;

protected:
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̊p�x
	float m_targetAngle;
};

