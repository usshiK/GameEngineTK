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


// 法線付き三角形（反時計回りが表面）
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// 法線ベクトル
};


// 球と球の衝突判定
bool checkSphereToSphere(const Sphere & sphereA, const Sphere & sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);

void ClosestPtPoint2Triangle(const DirectX::SimpleMath::Vector3 & _point, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _closest);

bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3 & _point, const Triangle & _triangle);

bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);
