/// <summary>
/// �����蔻��m�[�h
/// </summary>
/* -- �w�b�_�[�̃C���N���[�h ---- */
#pragma once
#include "Colision.h"
#include "Obj3d.h"

/* --- �N���X�̒�` ---*/
// �����蔻��m�[�h
class CollisionNode
{
public:
	/*�������́�*/
	virtual void initialize();	// ������(��Ƀ��f���̓ǂݍ���)
	virtual void update() = 0;	// �X�V
	virtual void draw() = 0;	// �`��

	/*���Z�b�^�[��*/
	void setParent(Obj3d* parent);								// �e
	void setTrans(const DirectX::SimpleMath::Vector3& trans);	// �e����̃I�t�Z�b�g
	void setDebugVisible(bool flag = !m_debugVisible);										// �f�o�b�O�\���I�u�W�F�N�g�̕\��,��\���؂�ւ�

	/*���Q�b�^�[��*/
	// �f�o�b�O���f����\�����Ă��邩���擾
	static bool GetDebugVisible();

protected:
	// �f�o�b�O�\���I�u�W�F�N�g
	Obj3d m_obj;
	// �f�o�b�O�\���I�u�W�F�N�g��\�����邩�̃t���O
	static bool m_debugVisible;

	// �e����̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_trans;
};

// ���󂠂��蔻��m�[�h
class sphereNode:public CollisionNode,public Sphere
{
public:
	sphereNode();
	virtual ~sphereNode();

	void initialize();	// ������(��Ƀ��f���̓ǂݍ���)
	void update();		// �X�V
	void draw();		// �`��

	/*���Z�b�^�[��*/
	void setLocalRadius(const float radius);// ���[�J�����a

private:
	float m_localRadius;// ���[�J�����a
};
