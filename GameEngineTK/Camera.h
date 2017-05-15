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

	void update();

	DirectX::SimpleMath::Matrix getViewMatrix();	// �r���[�s����擾����
	DirectX::SimpleMath::Matrix getProjMatrix();	// �r���[�s����擾����

	void setEyePos(DirectX::SimpleMath::Vector3 eyePos);	// ���_���W���Z�b�g
	void setRefPos(DirectX::SimpleMath::Vector3 refPos);	// �����_���Z�b�g
	void setUpVec(DirectX::SimpleMath::Vector3 UpVec);		//	������x�N�g�����Z�b�g
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

