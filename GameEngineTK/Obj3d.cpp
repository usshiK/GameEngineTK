#include "pch.h"
#include "Obj3d.h"

/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// 静的メンバ変数の実体
// カメラ
Camera* Obj3d::m_pCamera;

// コモンステイト(汎用描画設定)
std::unique_ptr<DirectX::CommonStates>		Obj3d::m_states;

// デバイス
Microsoft::WRL::ComPtr<ID3D11Device>		Obj3d::m_d3dDevice;

// コンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;

// エフェクトファクトリー
std::unique_ptr<DirectX::EffectFactory>		Obj3d::m_factory;

Obj3d::Obj3d()
	:m_parent(nullptr)
	,m_scale(Vector3(1,1,1))
{

}


//Obj3d::~Obj3d()
//{
//}

/// <summary>
/// ↓セッター↓
/// </summary>

// スケーリング
void Obj3d::setScale(const Vector3& scale)
{
	m_scale = scale;
}

// 回転角
void Obj3d::setRotate(const Vector3& rotate)
{
	m_rotate = rotate;
}

// 平行移動
void Obj3d::setTranse(const Vector3& transe)
{
	m_transe = transe;
}

// 親オブジェクト
void Obj3d::setParent(Obj3d * parent)
{
	m_parent = parent;
}

/// <summary>
/// ↑セッター↑
/// </summary>

// static変数を初期化
void Obj3d::initializeStatic(Camera * camera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = camera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");
}

/// <summary>
/// ↓ゲッター↓
/// </summary>

// ワールド行列
const Matrix& Obj3d::getWorld()
{
	return m_world;
}

// 回転角
const Vector3& Obj3d::getRotate()
{
	return m_rotate;
}

// 平行移動
const Vector3& Obj3d::getTranse()
{
	return m_transe;
}

// 親オブジェクト
Obj3d * Obj3d::getParent()
{
	return m_parent;
}

/// <summary>
/// ↑ゲッター↑
/// </summary>

/// <summary>
/// 更新処理
/// </summary>
void Obj3d::update()
{
	/// <summary>
	/// ワールド行列を計算
	/// </summary>
	// スケーリング行列
	Matrix scaleMat = Matrix::CreateScale(m_scale);
	// 回転行列
	Matrix rotateMatX = Matrix::CreateRotationX(m_rotate.x);
	Matrix rotateMatY = Matrix::CreateRotationY(m_rotate.y);
	Matrix rotateMatZ = Matrix::CreateRotationZ(m_rotate.z);
	Matrix rotateMat = rotateMatX * rotateMatY * rotateMatZ;
	// 平行移動行列
	Matrix transMat = Matrix::CreateTranslation(m_transe);

	// ワールド行列を設定
	m_world = scaleMat * rotateMat * transMat;
	// 親がいればそのworld行列もかける
	if (m_parent != nullptr)
	{
		m_world *= m_parent->getWorld();
	}
}

/// <summary>
/// 描画
/// </summary>
void Obj3d::draw()
{
	if (m_pModel != nullptr)
	{
		m_pModel->Draw(m_d3dContext.Get(),
			*m_states, 
			m_world, 
			m_pCamera->getViewMatrix(), 
			m_pCamera->getProjMatrix());
	}

}

/// <summary>
/// モデルを読み込み
/// </summary>
/// <param name="szFileName">ファイル名</param>
void Obj3d::LoadModel(const wchar_t * szFileName)
{
	m_pModel= Model::CreateFromCMO(
		m_d3dDevice.Get(),
		//L"Resources/robotBase.cmo",
		szFileName,
		*m_factory
	);
}
