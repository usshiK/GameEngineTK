/// <summary>
/// 衝突判定ライブラリ
/// </summary>
/* -- ヘッダーのインクルード ---- */
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>

// 線分クラス
class Segment
{
public:
	DirectX::SimpleMath::Vector3 start;	// 始点
	DirectX::SimpleMath::Vector3 end;	// 終点
};


// 球体クラス
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 center;	// 中心座標

	float radius;							// 半径

	Sphere() :radius(0) {};
	~Sphere() {};
};

// カプセルクラス
class Capsure
{
public:
	Segment segment;	// 線分
	float radius;		// 円の半径

	Capsure() :radius(0) {};
};

// 球と球の衝突判定
bool checkSphereToSphere(const Sphere & sphereA, const Sphere & sphereB);
