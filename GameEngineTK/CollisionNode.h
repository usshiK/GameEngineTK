/// <summary>
/// あたり判定ノード
/// </summary>
/* -- ヘッダーのインクルード ---- */
#pragma once
#include "Colision.h"
#include "Obj3d.h"

/* --- クラスの定義 ---*/
// あたり判定ノード
class CollisionNode
{
public:
	/*↓いつもの↓*/
	virtual void initialize();	// 初期化(主にモデルの読み込み)
	virtual void update() = 0;	// 更新
	virtual void draw() = 0;	// 描画

	/*↓セッター↓*/
	void setParent(Obj3d* parent);								// 親
	void setTrans(const DirectX::SimpleMath::Vector3& trans);	// 親からのオフセット
	void setDebugVisible(bool flag = !m_debugVisible);										// デバッグ表示オブジェクトの表示,非表示切り替え

	/*↓ゲッター↓*/
	// デバッグモデルを表示しているかを取得
	static bool GetDebugVisible();

protected:
	// デバッグ表示オブジェクト
	Obj3d m_obj;
	// デバッグ表示オブジェクトを表示するかのフラグ
	static bool m_debugVisible;

	// 親からのオフセット
	DirectX::SimpleMath::Vector3 m_trans;
};

// 球状あたり判定ノード
class sphereNode:public CollisionNode,public Sphere
{
public:
	sphereNode();
	virtual ~sphereNode();

	void initialize();	// 初期化(主にモデルの読み込み)
	void update();		// 更新
	void draw();		// 描画

	/*↓セッター↓*/
	void setLocalRadius(const float radius);// ローカル半径

private:
	float m_localRadius;// ローカル半径
};
