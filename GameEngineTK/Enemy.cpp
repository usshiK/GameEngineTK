#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


Enemy::Enemy()
{

}


Enemy::~Enemy()
{
}

void Enemy::initialize(DirectX::SimpleMath::Vector3 detoultPos)
{
	m_object.resize(ENEMY_PARTS_NUM);	// 配列の個数をパーツの個数に増やす
	m_object[ENEMY_PARTS_BASE].LoadModel(L"Resources/robotBase.cmo");
	m_object[ENEMY_PARTS_BODY].LoadModel(L"Resources/robotBody.cmo");
	m_object[ENEMY_PARTS_BREAST].LoadModel(L"Resources/robotBreast.cmo");
	m_object[ENEMY_PARTS_HEAD].LoadModel(L"Resources/robotHead.cmo");
	m_object[ENEMY_PARTS_WING].LoadModel(L"Resources/robotWing.cmo");

	//親子関係を築く
	m_object[ENEMY_PARTS_BODY].setParent(&m_object[ENEMY_PARTS_BASE]);
	m_object[ENEMY_PARTS_BREAST].setParent(&m_object[ENEMY_PARTS_BODY]);
	m_object[ENEMY_PARTS_HEAD].setParent(&m_object[ENEMY_PARTS_BREAST]);
	m_object[ENEMY_PARTS_WING].setParent(&m_object[ENEMY_PARTS_BREAST]);

	// 親からのオフセット(親から位置をずらす)
	m_object[ENEMY_PARTS_BODY].setTrans(Vector3(0.0f, 0.35f, 0.0f));
	m_object[ENEMY_PARTS_BREAST].setTrans(Vector3(0.0f, 0.4f, 0.0f));
	m_object[ENEMY_PARTS_HEAD].setTrans(Vector3(0.0f, 0.6f, 0.0f));
	m_object[ENEMY_PARTS_WING].setTrans(Vector3(0.0f, 0.3f, 0.5f));
	m_object[ENEMY_PARTS_WING].setRotate(Vector3(XMConvertToRadians(270), XMConvertToRadians(180), 0.0f));

	m_object[ENEMY_PARTS_BASE].setTrans(detoultPos);

	// 影の初期化
	m_shadow.LoadModel(L"Resources/shadow.cmo");
	m_shadow.setTrans(m_object[ENEMY_PARTS_BASE].getTrans());
	m_shadow.setTransY(0.01f);

	m_ai = GO;

	m_frame = 0;

	// 武器のあたり判定ノードの初期化
	{
		m_collisionNode.initialize();				// 初期化
		m_collisionNode.setParent(&m_object[0]);	// 基盤パーツにセット
		m_collisionNode.setTrans(Vector3(0, 0.7f, 0));	// オフセットをセット
		m_collisionNode.setLocalRadius(0.7f);		// 半径
	}
}

void Enemy::update()
{
	// 2秒に一度
	if (m_frame % (60*2) == 0)
	{
		// AIを更新
		m_ai = static_cast<AI_CODE>(rand() % AI_CODE_NUM);
	}
	
	// 現在のAIから行動を起こす
	action();

	// 振り向きを直す
	if (m_ai != ROT_LEFT && m_Rotation.y > 0.0f)
	{
		m_Rotation.y -= 0.5f;
	}
	if (m_ai != ROT_RIGHT && m_Rotation.y < 0.0f)
	{
		m_Rotation.y += 0.5f;
	}

	// 振り向きを更新
	m_object[ENEMY_PARTS_BODY].setRotateY(XMConvertToRadians(m_Rotation.y));
	m_object[ENEMY_PARTS_HEAD].setRotateY(XMConvertToRadians(m_Rotation.y * 1.2f));

	// 前かがみを更新
	m_object[ENEMY_PARTS_BODY].setRotateX(XMConvertToRadians(m_Rotation.x));

	// 翼を上下させる
	if (m_object[ENEMY_PARTS_BASE].getTrans().y <= 0.0f)
	{
		// 翼をもとに戻す
		m_object[ENEMY_PARTS_WING].setRotateY(0);
		m_object[ENEMY_PARTS_WING].setRotateX(90);
		m_object[ENEMY_PARTS_WING].setTransZ(0.5f);
		m_object[ENEMY_PARTS_WING].setTransY(sinWave(m_frame / 20.0f) / 10 + 0.2f);
	}
	else
	{
		// 翼を回転
		m_object[ENEMY_PARTS_WING].setRotateY(m_object[ENEMY_PARTS_WING].getRotate().y + 0.2f);
		m_object[ENEMY_PARTS_WING].setRotateX(0);
		m_object[ENEMY_PARTS_WING].setTransY(1.0f);
		m_object[ENEMY_PARTS_WING].setTransZ(0.0f);
	}
	// 重力を掛ける(仮)
	if (m_object[ENEMY_PARTS_BASE].getTrans().y > 0)
	{
		m_object[ENEMY_PARTS_BASE].setTransY(m_object[ENEMY_PARTS_BASE].getTrans().y - 0.2f);
	}
	else
	{
		m_object[ENEMY_PARTS_BASE].setTransY(0);
	}

	// 影にプレイヤーを追わせる
	m_shadow.setTrans(m_object[ENEMY_PARTS_BASE].getTrans());
	m_shadow.setTransY(0.01f);

	// モデル達の更新
	m_object[ENEMY_PARTS_BASE].update();
	m_object[ENEMY_PARTS_BODY].update();
	m_object[ENEMY_PARTS_BREAST].update();
	m_object[ENEMY_PARTS_HEAD].update();
	m_object[ENEMY_PARTS_WING].update();

	// 影の更新
	m_shadow.update();

	// 前方に移動
	Vector3 moveV = Vector3(0.0f, 0.0f, 0.05f);
	Matrix rotmat = Matrix::CreateRotationY(m_object[ENEMY_PARTS_BASE].getRotate().y);
	moveV = Vector3::TransformNormal(moveV, rotmat);
	m_object[ENEMY_PARTS_BASE].setTrans(m_object[ENEMY_PARTS_BASE].getTrans() - moveV);

	// 前かがみにする
	if (m_Rotation.x >= -15.0f)
	{
		m_Rotation.x -= 2.0f;
	}

	// あたり判定ノードの更新
	m_collisionNode.update();

	m_frame++;

}

void Enemy::render()
{
	for (std::vector<Obj3d>::iterator it = m_object.begin(); it != m_object.end(); it++)
	{
		it->draw();
	}

	// 影
	m_shadow.DrawSubtractive();

	// あたり判定ノードの描画
	m_collisionNode.draw();
}

/// <summary>
/// AIごとに動きを設定
/// </summary>
void Enemy::action()
{
	if (m_ai == Enemy::ROT_LEFT)
	{
		Vector3 v = Vector3(m_object[ENEMY_PARTS_BASE].getRotate().x, m_object[ENEMY_PARTS_BASE].getRotate().y + 0.01f, m_object[ENEMY_PARTS_BASE].getRotate().z);
		m_object[ENEMY_PARTS_BASE].setRotate(v);

		// 左に振り向く
		if (m_Rotation.y <= 15.0f)
		{
			m_Rotation.y += 1.0f;
		}
	}

	if (m_ai == Enemy::ROT_RIGHT)
	{
		Vector3 v = Vector3(m_object[ENEMY_PARTS_BASE].getRotate().x, m_object[ENEMY_PARTS_BASE].getRotate().y - 0.01f, m_object[ENEMY_PARTS_BASE].getRotate().z);
		m_object[ENEMY_PARTS_BASE].setRotate(v);

		// 右に振り向く
		if (m_Rotation.y >= -15.0f)
		{
			m_Rotation.y -= 1.0f;
		}
	}

	if (m_ai == Enemy::FLY)
	{
		// 上に飛ばす
		m_object[ENEMY_PARTS_BASE].setTransY(m_object[ENEMY_PARTS_BASE].getTrans().y + 0.25f);

		// 翼を回転
		m_object[ENEMY_PARTS_WING].setRotateY(m_object[ENEMY_PARTS_WING].getRotate().y + 0.3f);
		m_object[ENEMY_PARTS_WING].setRotateX(0);
		m_object[ENEMY_PARTS_WING].setTransY(1.0f);
		m_object[ENEMY_PARTS_WING].setTransZ(0.0f);
	}
}


const Vector3& Enemy::getTrance()
{
	return m_object[ENEMY_PARTS_BASE].getTrans();
}

const Vector3& Enemy::getRotation()
{
	return m_object[ENEMY_PARTS_BASE].getRotate();
}

sphereNode & Enemy::getCollisionNode()
{
	return m_collisionNode;
}

float Enemy::sinWave(float t)
{
	return (sinf(t) + 1.0f) / 2.0f;
};