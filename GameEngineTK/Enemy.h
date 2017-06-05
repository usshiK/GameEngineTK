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

	// プレイヤーのパーツに番号を付ける
	enum ENEMY_PARTS
	{
		ENEMY_PARTS_BASE,	// キャタピラ
		ENEMY_PARTS_BODY,	// 体
		ENEMY_PARTS_BREAST, // 胸
		ENEMY_PARTS_HEAD,	// 頭
		ENEMY_PARTS_WING,	// 翼

		ENEMY_PARTS_NUM
	};

	int m_frame;								// フレーム数を数える

	DirectX::SimpleMath::Vector3 m_Rotation;	// 回転角

	std::vector<Obj3d> m_object;				// 自身のモデル達

	Enemy::AI_CODE m_ai;

	// サインウェーブ
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
