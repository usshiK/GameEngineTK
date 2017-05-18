/// <summary>
/// ���@�ɒǏ]����J����
/// </summary>
/// 
/* -- �w�b�_�[�̃C���N���[�h ---- */
#include "Camera.h"
#include <Keyboard.h>

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

	// �L�[�{�[�h�̃Z�b�g
	void setKeyBoard(DirectX::Keyboard* keyBoard);

	// TPS�J�����̏�����
	void initiarizeTPS();
protected:
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̊p�x
	float m_targetAngle;

	// �L�[�{�[�h
	DirectX::Keyboard* m_keyBoard;
	// �L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyBoarTracker;

private:
	// ���W�̐��`�⊮�p
	DirectX::SimpleMath::Vector3 lerp(DirectX::SimpleMath::Vector3 startPos,DirectX::SimpleMath::Vector3 targetPos,float Proportion);

	// FPS��TPS�̐؂�ւ��悤�t���O
	bool m_isTPS;
};

