#pragma once
#include <vector>
#include "Obj3d.h"

class Player
{
private:
	// �v���C���[�̃p�[�c�ɔԍ���t����
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_BASE,	// �L���^�s��
		PLAYER_PARTS_BODY,	// ��
		PLAYER_PARTS_BREAST,// ��
		PLAYER_PARTS_HEAD,	// ��
		PLAYER_PARTS_WING,	// ��

		PLAYER_PARTS_NUM
	};
	int m_frame;

	DirectX::SimpleMath::Vector3 m_Rotation;

	std::vector<Obj3d> m_object;

	// �T�C���E�F�[�u
	float sinWave(float t);
public:
	enum KEY_CODE
	{
		W,
		S,
		A,
		D,
		ENTER
	};

	Player();
	~Player();
	void initialize(DirectX::SimpleMath::Vector3 detoultPos);
	void update();
	void render();

	void inputkey(KEY_CODE code);
	DirectX::SimpleMath::Vector3 getTrance();
	DirectX::SimpleMath::Vector3 getRotation();
};

