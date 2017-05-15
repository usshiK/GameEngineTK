/****************************************************************
*|�@�T�v�@�@�@�@�J�����N���X
*|�@�쐬�ҁ@�@�@GS1 04 ���R�q��
*|�@�쐬���@�@�@2017/05/11
*****************************************************************/
#include "pch.h"
#include "Camera.h"

/* -- ���O��Ԃ���� ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;



/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="width">��ʂ̏c��</param>
/// <param name="height">��ʂ̉���</param>
Camera::Camera(int width, int height)
{
	m_eyePos = Vector3(0, 0, 5.0f);
	m_refPos = Vector3(0, 0, 0);
	m_upVec = Vector3(0, 1, 0);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = static_cast<float>(width) / height;
	m_nearClip = 0.01f;
	m_farClip = 1000.0f;
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// �X�V����
/// </summary>
void Camera::update()
{
	// �r���[�s������
	m_view = Matrix::CreateLookAt(m_eyePos,m_refPos,m_upVec);

	// �ˉe�s������
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}


/// <summary>
/// �r���[�s����擾
/// </summary>
/// <returns>�r���[�s��</returns>
const Matrix& Camera::getViewMatrix()
{
	return m_view;
}

/// <summary>
/// �ˉe�s����擾
/// </summary>
/// <returns>�ˉe�s��</returns>
const Matrix& Camera::getProjMatrix()
{
	return m_proj;
}

/// <summary>
/// ���_���W���Z�b�g
/// </summary>
/// <param name="eyePos">���_���W</param>
void Camera::setEyePos(const Vector3& eyePos)
{
	m_eyePos = eyePos;
}


/// <summary>
/// �����_���Z�b�g
/// </summary>
/// <param name="refPos">�����_</param>
void Camera::setRefPos(const Vector3& refPos)
{
	m_refPos = refPos;
}

/// <summary>
/// ������x�N�g�����Z�b�g
/// </summary>
/// <param name="UpVec">������x�N�g��</param>
void Camera::setUpVec(const Vector3& UpVec)
{
	m_upVec = UpVec;
}

/// <summary>
/// ������������p���Z�b�g
/// </summary>
/// <param name="fovY">�Z�b�g������������������p</param>
void Camera::setFovY(float fovY)
{
	m_fovY = fovY;
}

/// <summary>
/// ��ʂ̏c������Z�b�g
/// </summary>
/// <param name="aspect">�Z�b�g��������ʂ̏c����</param>
void Camera::setAspect(float aspect)
{
	m_aspect = aspect;
}

/// <summary>
/// �߂��̕\�����E���Z�b�g
/// </summary>
/// <param name="nearClip">�Z�b�g�������߂��̕\�����E</param>
void Camera::setNearClip(float nearClip)
{
	m_nearClip = nearClip;
}

/// <summary>
/// �����̕\�����E���Z�b�g
/// </summary>
/// <param name="farClip">�Z�b�g�����������̕\�����E</param>
void Camera::setFarClip(float farClip)
{
	m_farClip = farClip;
}
