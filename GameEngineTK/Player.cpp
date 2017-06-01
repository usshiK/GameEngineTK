#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


Player::Player()
{

}


Player::~Player()
{

}

void Player::initialize(DirectX::SimpleMath::Vector3 detoultPos)
{
	m_object.resize(5);	// 配列の個数をパーツの個数に増やす
	m_object[PLAYER_PARTS_BASE].LoadModel(L"Resources/robotBase.cmo");
	m_object[PLAYER_PARTS_BODY].LoadModel(L"Resources/robotBody.cmo");
	m_object[PLAYER_PARTS_BREAST].LoadModel(L"Resources/robotBreast.cmo");
	m_object[PLAYER_PARTS_HEAD].LoadModel(L"Resources/robotHead.cmo");
	m_object[PLAYER_PARTS_WING].LoadModel(L"Resources/robotWing.cmo");

	// 親子関係を築く
	m_object[PLAYER_PARTS_BODY].setParent(&m_object[PLAYER_PARTS_BASE]);
	m_object[PLAYER_PARTS_BREAST].setParent(&m_object[PLAYER_PARTS_BODY]);
	m_object[PLAYER_PARTS_HEAD].setParent(&m_object[PLAYER_PARTS_BREAST]);
	m_object[PLAYER_PARTS_WING].setParent(&m_object[PLAYER_PARTS_BREAST]);

	// 親からのオフセット(親から位置をずらす)
	m_object[PLAYER_PARTS_BODY].setTranse(Vector3(0.0f, 0.35f, 0.0f));
	m_object[PLAYER_PARTS_BREAST].setTranse(Vector3(0.0f, 0.4f, 0.0f));
	m_object[PLAYER_PARTS_HEAD].setTranse(Vector3(0.0f, 0.6f, 0.0f));
	m_object[PLAYER_PARTS_WING].setTranse(Vector3(0.0f, 0.3f, 0.5f));
	m_object[PLAYER_PARTS_WING].setRotate(Vector3(XMConvertToRadians(270), XMConvertToRadians(180), 0.0f));
}

void Player::update()
{
	// 振り向きを直す
	if (m_Rotation.y > 0.0f)
	{
		m_Rotation.y -= 0.5f;
	}
	if (m_Rotation.y < 0.0f)
	{
		m_Rotation.y += 0.5f;
	}

	// 振り向きを更新
	m_object[PLAYER_PARTS_BODY].setRotateY(XMConvertToRadians(m_Rotation.y));
	m_object[PLAYER_PARTS_HEAD].setRotateY(XMConvertToRadians(m_Rotation.y * 1.2f));

	// 前かがみを直す
	if (m_Rotation.x <= 0.0f)
	{
		m_Rotation.x += 0.5f;
	}

	// 前かがみを更新
	m_object[PLAYER_PARTS_BODY].setRotateX(XMConvertToRadians(m_Rotation.x));

	// 翼を上下させる
	if (m_object[PLAYER_PARTS_BASE].getTranse().y <= 0.0f)
	{
		// 翼をもとに戻す
		m_object[PLAYER_PARTS_WING].setRotateY(0);
		m_object[PLAYER_PARTS_WING].setRotateX(90);
		m_object[PLAYER_PARTS_WING].setTranseZ(0.5f);

		m_object[PLAYER_PARTS_WING].setTranseY(sinWave(m_frame / 20.0f) / 10 + 0.2f);
	}
	else
	{
		// 翼を回転
		m_object[PLAYER_PARTS_WING].setRotateY(m_object[PLAYER_PARTS_WING].getRotate().y + 0.2f);
		m_object[PLAYER_PARTS_WING].setRotateX(0);
		m_object[PLAYER_PARTS_WING].setTranseY(1.0f);
		m_object[PLAYER_PARTS_WING].setTranseZ(0.0f);
	}

	// 重力を掛ける(仮)
	if (m_object[PLAYER_PARTS_BASE].getTranse().y > 0)
	{
		m_object[PLAYER_PARTS_BASE].setTranseY(m_object[PLAYER_PARTS_BASE].getTranse().y - 0.2f);
	}
	else
	{
		m_object[PLAYER_PARTS_BASE].setTranseY(0);
	}

	// モデル達の更新
	m_object[PLAYER_PARTS_BASE].update();
	m_object[PLAYER_PARTS_BODY].update();
	m_object[PLAYER_PARTS_BREAST].update();
	m_object[PLAYER_PARTS_HEAD].update();
	m_object[PLAYER_PARTS_WING].update();

	m_frame++;
}

void Player::render()
{
	for (std::vector<Obj3d>::iterator it = m_object.begin(); it != m_object.end(); it++)
	{
		it->draw();
	}
}

void Player::inputkey(KEY_CODE code)
{
	if (code == Player::W)
	{
		// 前方に移動
		Vector3 moveV = Vector3(0.0f, 0.0f, 0.1f);
		Matrix rotmat = Matrix::CreateRotationY(m_object[PLAYER_PARTS_BASE].getRotate().y);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		m_object[PLAYER_PARTS_BASE].setTranse(m_object[PLAYER_PARTS_BASE].getTranse() - moveV);

		// 前かがみにする
		if (m_Rotation.x >= -15.0f)
		{
			m_Rotation.x -= 2.0f;
		}
	}

	if (code == Player::A)
	{
		Vector3 v = Vector3(m_object[PLAYER_PARTS_BASE].getRotate().x, m_object[PLAYER_PARTS_BASE].getRotate().y + 0.05f, m_object[PLAYER_PARTS_BASE].getRotate().z);
		m_object[PLAYER_PARTS_BASE].setRotate(v);

		// 左に振り向く
		if (m_Rotation.y <= 15.0f)
		{
			m_Rotation.y += 1.0f;
		}
	}

	if (code == Player::S)
	{
		// 後方に移動
		Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);
		Matrix rotmat = Matrix::CreateRotationY(m_object[PLAYER_PARTS_BASE].getRotate().y);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		m_object[PLAYER_PARTS_BASE].setTranse(m_object[PLAYER_PARTS_BASE].getTranse() - moveV);

		// 後ろを見る
		if (m_Rotation.y <= 45.0f)
		{
			m_Rotation.y += 1.5f;
		}
		if (m_Rotation.x <= 15.0f)
		{
			m_Rotation.x += 1.5f;
		}
	}

	if (code == Player::D)
	{
		Vector3 v = Vector3(m_object[PLAYER_PARTS_BASE].getRotate().x, m_object[PLAYER_PARTS_BASE].getRotate().y - 0.05f, m_object[PLAYER_PARTS_BASE].getRotate().z);
		m_object[PLAYER_PARTS_BASE].setRotate(v);

		// 右に振り向く
		if (m_Rotation.y >= -15.0f)
		{
			m_Rotation.y -= 1.0f;
		}
	}

	if (code == Player::ENTER)
	{
		// 上に飛ばす
		m_object[PLAYER_PARTS_BASE].setTranseY(m_object[PLAYER_PARTS_BASE].getTranse().y + 0.25f);

		// 翼を回転
		m_object[PLAYER_PARTS_WING].setRotateY(m_object[PLAYER_PARTS_WING].getRotate().y + 0.3f);
		m_object[PLAYER_PARTS_WING].setRotateX(0);
		m_object[PLAYER_PARTS_WING].setTranseY(1.0f);
		m_object[PLAYER_PARTS_WING].setTranseZ(0.0f);
	}
}

Vector3 Player::getTrance()
{
	return m_object[PLAYER_PARTS_BASE].getTranse();
}

DirectX::SimpleMath::Vector3 Player::getRotation()
{
	return m_object[PLAYER_PARTS_BASE].getRotate();
}

float Player::sinWave(float t)
{
	return (sinf(t) + 1.0f) / 2.0f;
};