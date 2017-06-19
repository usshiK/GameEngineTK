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
	virtual void initialize() = 0;// ������(��Ƀ��f���̓ǂݍ���)
	virtual void update() = 0;	  // �X�V
	virtual void draw() = 0;	  // �`��

	/*���Z�b�^�[��*/
	void setParent(Obj3d* parent);								// �e
	void setTrans(const DirectX::SimpleMath::Vector3& trans);	// �e����̃I�t�Z�b�g

protected:
	// �f�o�b�O�\���I�u�W�F�N�g
	Obj3d m_obj;

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
