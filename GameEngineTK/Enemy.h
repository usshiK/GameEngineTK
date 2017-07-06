#pragma once
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"

class Enemy
{
private:
	// AI��ID
	enum AI_CODE
	{
		GO,
		ROT_LEFT,
		ROT_RIGHT,
		FLY,

		AI_CODE_NUM
	};

	// �p�[�cID
	enum ENEMY_PARTS
	{
		ENEMY_PARTS_BASE,	// �L���^�s��
		ENEMY_PARTS_BODY,	// ��
		ENEMY_PARTS_BREAST, // ��
		ENEMY_PARTS_HEAD,	// ��
		ENEMY_PARTS_WING,	// ��

		ENEMY_PARTS_NUM
	};

	// ��]�p
	DirectX::SimpleMath::Vector3 m_Rotation;	

	// ���g�̃��f���B
	std::vector<Obj3d> m_object;
	// �e���f��
	Obj3d m_shadow;

	// ���݂�AI
	AI_CODE m_ai;

	// �t���[�����𐔂���
	int m_frame;				

	//�@���󂠂��蔻��m�[�h
	sphereNode m_collisionNode;

public:
	Enemy();
	~Enemy();
	void initialize(DirectX::SimpleMath::Vector3 detoultPos);
	void update();
	void render();

	// AI���Ƃɓ�����ݒ�
	void action();

	/*���Q�b�^�[��*/
	const DirectX::SimpleMath::Vector3& getTrance();	// �g�����X
	const DirectX::SimpleMath::Vector3& getRotation();	// ��]�p
	sphereNode& getCollisionNode();	// ���󂠂��蔻��m�[�h

private:
	// �T�C���E�F�[�u
	float sinWave(float t);
};

#pragma once
