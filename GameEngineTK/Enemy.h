#pragma once
#include <vector>
#include "Obj3d.h"

class Enemy
{
private:
	enum AI_CODE
	{
		GO,
		ROT_LEFT,
		ROT_RIGHT,
		FLY,

		AI_CODE_NUM
	};

	// �v���C���[�̃p�[�c�ɔԍ���t����
	enum ENEMY_PARTS
	{
		ENEMY_PARTS_BASE,	// �L���^�s��
		ENEMY_PARTS_BODY,	// ��
		ENEMY_PARTS_BREAST, // ��
		ENEMY_PARTS_HEAD,	// ��
		ENEMY_PARTS_WING,	// ��

		ENEMY_PARTS_NUM
	};

	int m_frame;								// �t���[�����𐔂���

	DirectX::SimpleMath::Vector3 m_Rotation;	// ��]�p

	std::vector<Obj3d> m_object;				// ���g�̃��f���B

	Enemy::AI_CODE m_ai;

	// �T�C���E�F�[�u
	float sinWave(float t);
public:
	Enemy();
	~Enemy();
	void initialize(DirectX::SimpleMath::Vector3 detoultPos);
	void update();
	void render();

	void action();
	DirectX::SimpleMath::Vector3 getTrance();
	DirectX::SimpleMath::Vector3 getRotation();
};

#pragma once
