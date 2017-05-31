/// <summary>
/// 3Dオブジェクトのクラス
/// </summary>
#pragma once
#include <direct.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <CommonStates.h>

#include <memory>
#include <windows.h>
#include <wrl/client.h>

#include "Camera.h"

class Obj3d
{
public:
	Obj3d();
	//virtual ~Obj3d();

	// セッター
	// スケーリング
	void setScale(const DirectX::SimpleMath::Vector3& scale);

	// 回転角
	void setRotate(const DirectX::SimpleMath::Vector3& rotate);
	void setRotateX(const float rotate);
	void setRotateY(const float rotate);
	void setRotateZ(const float rotate);

	// 平行移動
	void setTranse(const DirectX::SimpleMath::Vector3& transe);
	void setTranseX(const float transe);
	void setTranseY(const float transe);
	void setTranseZ(const float transe);

	// 親オブジェクト
	void setParent(Obj3d* parent);



	// 制定メンバ変数を初期化する 
	static void initializeStatic(Camera* camera,
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);



	// ゲッター
	// ワールド行列
	const DirectX::SimpleMath::Matrix& getWorld();

	// 回転角
	const DirectX::SimpleMath::Vector3& getRotate();

	// 平行移動
	const DirectX::SimpleMath::Vector3& getTranse();

	// 親オブジェクト
	Obj3d* getParent();



	// 更新
	void update();

	// 描画
	void draw();

	// モデルの読み込み
	void LoadModel(const wchar_t* szFileName);
private:
	// モデル
	std::unique_ptr<DirectX::Model>m_pModel;

	// スケーリング
	DirectX::SimpleMath::Vector3 m_scale;

	// 回転角
	DirectX::SimpleMath::Vector3 m_rotate;

	// 平行移動
	DirectX::SimpleMath::Vector3 m_transe;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

	// 親オブジェクトのポインタ
	Obj3d* m_parent;
private:
	// カメラ
	static Camera* m_pCamera;

	// デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>  m_d3dDevice;

	// コンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;

	// コモンステイト(汎用描画設定)
	static std::unique_ptr<DirectX::CommonStates>	m_states;

	// エフェクトファクトリー
	static std::unique_ptr<DirectX::EffectFactory> m_factory;
};

