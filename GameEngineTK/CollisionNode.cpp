#include "pch.h"
#include "CollisionNode.h"

/* -- ���O��Ԃ���� ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;

bool CollisionNode::m_debugVisible = false;


/*CollisionNode*/

void CollisionNode::initialize()
{
}

/// <summary>
/// �e��o�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
void CollisionNode::setParent(Obj3d * parent)
{
	m_obj.setParent(parent);
}

/// <summary>
/// �e����̃I�t�Z�b�g��o�^
/// </summary>
/// <param name="trans">���炷�x�N�g��</param>
void CollisionNode::setTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	m_trans = trans;
}

/// <summary>
/// �f�o�b�O�\���I�u�W�F�N�g�̕\��,��\���؂�ւ�
/// </summary>
void CollisionNode::setDebugVisible(bool flag)
{
	m_debugVisible = flag;
}

bool CollisionNode::GetDebugVisible()
{
	return m_debugVisible;
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
	// �p�����̃C�j�V�����C�Y
	CollisionNode::initialize();

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

	// ���苅(���S���W�Ɣ��a)���X�V
	{
		// ���[���h�s����擾
		const Matrix& worldm = m_obj.getWorld();

		// ���̒��S�_�ƕ\�ʂ̓_���`
		Vector3 center(0, 0, 0);					// ����p��
		Vector3 right(1, 0, 0);
		center = Vector3::Transform(center, worldm);// ���[���h���W�ɕϊ�
		right = Vector3::Transform(right, worldm);

		// �����o�ɃZ�b�g
		Sphere::center = center;							// ���S���W
		Sphere::radius = Vector3::Distance(center, right);	// ���a
	}
}

/// <summary>
///  �`��
/// </summary>
void sphereNode::draw()
{
	// �\���t���O��true�Ȃ�
	if (m_debugVisible)
	{
		// �f�o�b�O���f��
		m_obj.draw();
	}
}

void sphereNode::setLocalRadius(const float radius)
{
	m_localRadius = radius;
}


