/****************************************************************
*|�@�T�v�@�@�@�@�J�����N���X
*|�@�쐬�ҁ@�@�@GS1 04 ���R�q��
*|�@�쐬���@�@�@2017/05/11
*****************************************************************/
#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	Camera(int width,int height);
	virtual ~Camera();

	virtual void update();

	const DirectX::SimpleMath::Matrix& getViewMatrix();	// �r���[�s����擾����
	const DirectX::SimpleMath::Matrix& getProjMatrix();	// �r���[�s����擾����

	void setEyePos(const DirectX::SimpleMath::Vector3& eyePos);	// ���_���W���Z�b�g
	void setRefPos(const DirectX::SimpleMath::Vector3& refPos);	// �����_���Z�b�g
	void setUpVec(const DirectX::SimpleMath::Vector3& UpVec);	//	������x�N�g�����Z�b�g

	void setFovY(float fovY);			// ������������p���Z�b�g
	void setAspect(float aspect);		// ��ʂ̏c������Z�b�g
	void setNearClip(float nearClip);	// �߂��̕\�����E���Z�b�g
	void setFarClip(float farClip);		// �����̕\�����E���Z�b�g

protected:
	// �r���[�s��
	DirectX::SimpleMath::Matrix	m_view;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eyePos;
	// ���Ă����(�����_)
	DirectX::SimpleMath::Vector3 m_refPos;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 m_upVec;

	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix		m_proj;
	// ������������p(�c�����ɂǂ̂��炢�̊p�x�����邩)
	float m_fovY;
	// ��ʂ̏c����
	float m_aspect;
	// �߂��̕\�����E
	float m_nearClip;
	// �����̕\�����E
	float m_farClip;
};

