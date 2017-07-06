#pragma once
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"

class Enemy
{
private:
	// AIのID
	enum AI_CODE
	{
		GO,
		ROT_LEFT,
		ROT_RIGHT,
		FLY,

		AI_CODE_NUM
	};

	// パーツID
	enum ENEMY_PARTS
	{
		ENEMY_PARTS_BASE,	// キャタピラ
		ENEMY_PARTS_BODY,	// 体
		ENEMY_PARTS_BREAST, // 胸
		ENEMY_PARTS_HEAD,	// 頭
		ENEMY_PARTS_WING,	// 翼

		ENEMY_PARTS_NUM
	};

	// 回転角
	DirectX::SimpleMath::Vector3 m_Rotation;	

	// 自身のモデル達
	std::vector<Obj3d> m_object;
	// 影モデル
	Obj3d m_shadow;

	// 現在のAI
	AI_CODE m_ai;

	// フレーム数を数える
	int m_frame;				

	//　球状あたり判定ノード
	sphereNode m_collisionNode;

public:
	Enemy();
	~Enemy();
	void initialize(DirectX::SimpleMath::Vector3 detoultPos);
	void update();
	void render();

	// AIごとに動きを設定
	void action();

	/*↓ゲッター↓*/
	const DirectX::SimpleMath::Vector3& getTrance();	// トランス
	const DirectX::SimpleMath::Vector3& getRotation();	// 回転角
	sphereNode& getCollisionNode();	// 球状あたり判定ノード

private:
	// サインウェーブ
	float sinWave(float t);
};

#pragma once
