/// <summary>
/// ���@�ɒǏ]����J����
/// </summary>
#include "pch.h"
#include "FollowCamera.h"
/* -- ���O��Ԃ���� ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;

/* -- �萔�̒�` ---- */
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
/// �X�V
/// </summary>
void FollowCamera::update()
{
	// ���ɃJ�������ڎw�����W
	Vector3 nextEyePos, nextRefPos;

	// �L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keyState = m_keyBoard->GetState();
	m_keyBoarTracker.Update(keyState);
	// C����������t���O��؂�ւ���
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


	// FPS�J�����p
	Vector3 fpsCameraPos;


	// �Q�Ɠ_����A���_�ւ̍����x�N�g��
	Vector3 cameraV = Vector3(0.0f, 0.0f, CAMER_DISTANSE);

	// ���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotMat = Matrix::CreateRotationY(m_targetAngle);

	cameraV = Vector3::TransformNormal(cameraV, rotMat);

	// �t���O�ɂ�菈����ύX
	if (m_isTPS)
	{
		nextRefPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
		nextEyePos = nextRefPos + cameraV;
		 // ���_�����݈ʒu����⊮
		nextEyePos = lerp(m_eyePos, nextEyePos, 0.1f);
		// �Q�ƓX�����݈ʒu����⊮
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
/// �Ǐ]�Ώۂ̍��W���Z�b�g����
/// </summary>
/// <param name="tagetPos"></param>
void FollowCamera::setTargetPos(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_targetPos = targetPos;
}

/// <summary>
/// �Ǐ]�Ώۂ̊p�x���Z�b�g����
/// </summary>
/// <param name="targetAngle"></param>
void FollowCamera::setTargetAngle(const float & targetAngle)
{
	m_targetAngle = targetAngle;
}

/// <summary>
/// �L�[�{�[�h�̃Z�b�g
/// </summary>
/// <param name="keyBoard">�Z�b�g����L�[�{�[�h</param>
void FollowCamera::setKeyBoard(DirectX::Keyboard * keyBoard)
{
	m_keyBoard = keyBoard;
}

/// <summary>
/// TPS�J�����̏�����
/// </summary>
void FollowCamera::initiarizeTPS()
{
	// ���ɃJ�������ڎw�����W
	Vector3 nextEyePos, nextRefPos;

	// �Q�Ɠ_����A���_�ւ̍����x�N�g��
	Vector3 cameraV = Vector3(0.0f, 0.0f, CAMER_DISTANSE);

	// ���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotMat = Matrix::CreateRotationY(m_targetAngle);

	cameraV = Vector3::TransformNormal(cameraV, rotMat);
	nextRefPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
	nextEyePos = nextRefPos + cameraV;

	setEyePos(nextEyePos);
	setRefPos(nextRefPos);

}

/// <summary>
/// ���W�̐��`�⊮�p
/// </summary>
/// <param name="startPos">���݂̍��W</param>
/// <param name="targetPos">�ړI�̍��W</param>
/// <returns>�⊮��̍��W</returns>
DirectX::SimpleMath::Vector3 FollowCamera::lerp(DirectX::SimpleMath::Vector3 startPos,DirectX::SimpleMath::Vector3 targetPos, float Proportion)
{
	return startPos + (targetPos - startPos)*Proportion;
}
