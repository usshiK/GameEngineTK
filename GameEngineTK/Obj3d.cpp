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
	,m_useQuternion(false)
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

// 回転角X
void Obj3d::setRotateX(const float rotate)
{
	m_rotate.x = rotate;
}

// 回転角Y
void Obj3d::setRotateY(const float rotate)
{
	m_rotate.y = rotate;
}

// 回転角Z
void Obj3d::setRotateZ(const float rotate)
{
	m_rotate.z = rotate;
}

// 平行移動
void Obj3d::setTrans(const Vector3& transe)
{
	m_transe = transe;
}

// 平行移動X
void Obj3d::setTransX(const float transe)
{
	m_transe.x = transe;
}

// 平行移動Y
void Obj3d::setTransY(const float transe)
{
	m_transe.y = transe;
}

// 平行移動Z
void Obj3d::setTransZ(const float transe)
{
	m_transe.z = transe;
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

	//// 減算描画用のブレンドステートを作成
	//D3D11_BLEND_DESC desc;
	//desc.AlphaToCoverageEnable = false;
	//desc.IndependentBlendEnable = false;
	//desc.RenderTarget[0].BlendEnable = true;
	//desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	//desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	//desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	//desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	//desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//HRESULT ret = s_pDevice->CreateBlendState(&desc, &s_pBlendStateSubtract);
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
	Matrix rotateMat;
	if (m_useQuternion)
	{
		rotateMat = Matrix::CreateFromQuaternion(m_rotateQ);
	}
	else
	{// 
		Matrix rotateMatX = Matrix::CreateRotationX(m_rotate.x);
		Matrix rotateMatY = Matrix::CreateRotationY(m_rotate.y);
		Matrix rotateMatZ = Matrix::CreateRotationZ(m_rotate.z);
		rotateMat = rotateMatX * rotateMatY * rotateMatZ;
	}
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

//void Obj3D::DrawSubtractive()
//{
//	if (m_pModel)
//	{
//		assert(s_pCamera);
//		const Matrix& view = s_pCamera->GetViewmat();
//		const Matrix& projection = s_pCamera->GetProjmat();
//
//		assert(s_pDeviceContext);
//		assert(s_pStates);
//
//		// 減算描画用の設定関数を渡して描画
//		m_pModel->Draw(s_pDeviceContext, *s_pStates, m_LocalWorld, view, projection, false, Obj3D::SetSubtractive);
//	}
//}

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

/**
*	@brief オブジェクトのライティングを無効にする
*/
void Obj3d::DisableLighting()
{
	if (m_pModel)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}