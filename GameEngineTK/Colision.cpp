/// <summary>
/// 衝突判定ライブラリ
/// </summary>
/* -- ヘッダーのインクルード ---- */
#include "Colision.h"

/* -- 名前空間を解放 ---- */
using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球の衝突判定
/// </summary>
/// <param name="sphereA">球A</param>
/// <param name="sphereB">球B</param>
/// <returns>衝突していたらtrue</returns>
bool checkSphereToSphere(const Sphere &sphereA, const Sphere& sphereB)
{
	// 二つの球の中心座標の差を出す
	Vector3 sub = sphereA.center - sphereB.center;

	// subの長さを求める(ピタゴる)	
	//float distanse = sqrtf(sub.x*sub.x + sub.y*sub.y + sub.z*sub.z);

	// sqrtfは実は重い、なるべく使いたくない 
	// せや！squtfを使わんといたろ
	float distanseSquare = sub.x*sub.x + sub.y*sub.y + sub.z*sub.z;
	// radiusも二乗すれば、結果変わらんやん
	float radiusSquare = sphereA.radius + sphereB.radius;
	radiusSquare *= radiusSquare;

	// 二つの半径の和を出す
	//float radius = sphereA.radius + sphereB.radius;

	// 二つの半径の和をがsubの長さより大きいなら
	if (radiusSquare < distanseSquare)
	{
		// 当たっていない
		return false;
	}

	return true;
}