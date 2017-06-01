#pragma once
#include <vector>
#include "Obj3d.h"

class Player
{
private:
	// プレイヤーのパーツに番号を付ける
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_BASE,	// キャタピラ
		PLAYER_PARTS_BODY,	// 体
		PLAYER_PARTS_BREAST,// 胸
		PLAYER_PARTS_HEAD,	// 頭
		PLAYER_PARTS_WING,	// 翼

		PLAYER_PARTS_NUM
	};
	int m_frame;

	DirectX::SimpleMath::Vector3 m_Rotation;

	std::vector<Obj3d> m_object;

	// サインウェーブ
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

