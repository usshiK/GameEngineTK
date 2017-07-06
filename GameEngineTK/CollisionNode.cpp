#include "pch.h"
#include "CollisionNode.h"

/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;

bool CollisionNode::m_debugVisible = false;


/*CollisionNode*/

void CollisionNode::initialize()
{
}

/// <summary>
/// 親を登録
/// </summary>
/// <param name="parent">親のポインタ</param>
void CollisionNode::setParent(Obj3d * parent)
{
	m_obj.setParent(parent);
}

/// <summary>
/// 親からのオフセットを登録
/// </summary>
/// <param name="trans">ずらすベクトル</param>
void CollisionNode::setTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	m_trans = trans;
}

/// <summary>
/// デバッグ表示オブジェクトの表示,非表示切り替え
/// </summary>
void CollisionNode::setDebugVisible(bool flag)
{
	m_debugVisible = flag;
}

bool CollisionNode::GetDebugVisible()
{
	return m_debugVisible;
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
	// 継承元のイニシャライズ
	CollisionNode::initialize();

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

	// 判定球(中心座標と半径)を更新
	{
		// ワールド行列を取得
		const Matrix& worldm = m_obj.getWorld();

		// 球の中心点と表面の点を定義
		Vector3 center(0, 0, 0);					// 箱を用意
		Vector3 right(1, 0, 0);
		center = Vector3::Transform(center, worldm);// ワールド座標に変換
		right = Vector3::Transform(right, worldm);

		// メンバにセット
		Sphere::center = center;							// 中心座標
		Sphere::radius = Vector3::Distance(center, right);	// 半径
	}
}

/// <summary>
///  描画
/// </summary>
void sphereNode::draw()
{
	// 表示フラグがtrueなら
	if (m_debugVisible)
	{
		// デバッグモデル
		m_obj.draw();
	}
}

void sphereNode::setLocalRadius(const float radius)
{
	m_localRadius = radius;
}


