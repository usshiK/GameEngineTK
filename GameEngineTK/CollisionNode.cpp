#include "pch.h"
#include "CollisionNode.h"

/* -- ���O��Ԃ���� ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;



/*CollisionNode*/

/// <summary>
/// �e��o�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
void CollisionNode::setParent(Obj3d * parent)
{
	m_obj.setParent(parent);
}

void CollisionNode::setTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	m_trans = trans;
}




/*SphereNode*/

/// <summary>
/// �R���X�g���N�^
/// </summary>
sphereNode::sphereNode()
{
	// �f�t�H���g���[�J�����a
	m_localRadius = 1.0f;
}

/// <summary>
///  �f�X�g���N�^
/// </summary>
sphereNode::~sphereNode()
{

}

/// <summary>
/// ������
/// </summary>
void sphereNode::initialize()
{
	// �f�o�b�O�\���p���f�������[�h
	m_obj.LoadModel(L"Resources/sphereNode.cmo");
}

/// <summary>
/// �X�V
/// </summary>
void sphereNode::update()
{
	// �f�o�b�O���f���̐ݒ�
	m_obj.setTrans(m_trans);		// �I�t�Z�b�g���W
	m_obj.setScale(Vector3(m_localRadius));// ���̑傫��

	// �f�o�b�O���f���̍s��̍X�V
	m_obj.update();
}

/// <summary>
///  �`��
/// </summary>
void sphereNode::draw()
{
	// �f�o�b�O���f��
	m_obj.draw();
}

void sphereNode::setLocalRadius(const float radius)
{
	m_localRadius = radius;
}


