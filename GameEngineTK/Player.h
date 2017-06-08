#pragma once
#include <d3d11_1.h>
#include <Keyboard.h>
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

	// キーボード
	DirectX::Keyboard* m_keyBoard;

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

