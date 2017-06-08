#pragma once
#include <d3d11_1.h>
#include <Keyboard.h>
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

	// �L�[�{�[�h
	DirectX::Keyboard* m_keyBoard;

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
	void initialize(DirectX::SimpleMath::Vector3 detoultPos, DirectX::Keyboard* keyBoard);
	void update();
	void render();

	DirectX::SimpleMath::Vector3 getTrance();
	DirectX::SimpleMath::Vector3 getRotation();

private:
	void go();
	void back();
	void turnRight();
	void turnLeft();
	void fly();
	void fall();
};

