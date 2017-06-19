#include "pch.h"
#include "CollisionNode.h"

/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;



/*CollisionNode*/

/// <summary>
/// 親を登録
/// </summary>
/// <param name="parent">親のポインタ</param>
void CollisionNode::setParent(Obj3d * parent)
{
	m_obj.setParent(parent);
}

void CollisionNode::setTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	m_trans = trans;
}




/*SphereNode*/

/// <summary>
/// コンストラクタ
/// </summary>
sphereNode::sphereNode()
{
	// デフォルトローカル半径
	m_localRadius = 1.0f;
}

/// <summary>
///  デストラクタ
/// </summary>
sphereNode::~sphereNode()
{

}

/// <summary>
/// 初期化
/// </summary>
void sphereNode::initialize()
{
	// デバッグ表示用モデルをロード
	m_obj.LoadModel(L"Resources/sphereNode.cmo");
}

/// <summary>
/// 更新
/// </summary>
void sphereNode::update()
{
	// デバッグモデルの設定
	m_obj.setTrans(m_trans);		// オフセット座標
	m_obj.setScale(Vector3(m_localRadius));// 球の大きさ

	// デバッグモデルの行列の更新
	m_obj.update();
}

/// <summary>
///  描画
/// </summary>
void sphereNode::draw()
{
	// デバッグモデル
	m_obj.draw();
}

void sphereNode::setLocalRadius(const float radius)
{
	m_localRadius = radius;
}


