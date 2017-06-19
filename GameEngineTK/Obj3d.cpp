#include "pch.h"
#include "Obj3d.h"

/* -- ���O��Ԃ���� ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// �ÓI�����o�ϐ��̎���
// �J����
Camera* Obj3d::m_pCamera;

// �R�����X�e�C�g(�ėp�`��ݒ�)
std::unique_ptr<DirectX::CommonStates>		Obj3d::m_states;

// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>		Obj3d::m_d3dDevice;

// �R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;

// �G�t�F�N�g�t�@�N�g���[
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
/// ���Z�b�^�[��
/// </summary>

// �X�P�[�����O
void Obj3d::setScale(const Vector3& scale)
{
	m_scale = scale;
}

// ��]�p
void Obj3d::setRotate(const Vector3& rotate)
{
	m_rotate = rotate;
}

// ��]�pX
void Obj3d::setRotateX(const float rotate)
{
	m_rotate.x = rotate;
}

// ��]�pY
void Obj3d::setRotateY(const float rotate)
{
	m_rotate.y = rotate;
}

// ��]�pZ
void Obj3d::setRotateZ(const float rotate)
{
	m_rotate.z = rotate;
}

// ���s�ړ�
void Obj3d::setTrans(const Vector3& transe)
{
	m_transe = transe;
}

// ���s�ړ�X
void Obj3d::setTransX(const float transe)
{
	m_transe.x = transe;
}

// ���s�ړ�Y
void Obj3d::setTransY(const float transe)
{
	m_transe.y = transe;
}

// ���s�ړ�Z
void Obj3d::setTransZ(const float transe)
{
	m_transe.z = transe;
}

// �e�I�u�W�F�N�g
void Obj3d::setParent(Obj3d * parent)
{
	m_parent = parent;
}

/// <summary>
/// ���Z�b�^�[��
/// </summary>

// static�ϐ���������
void Obj3d::initializeStatic(Camera * camera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = camera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");

	//// ���Z�`��p�̃u�����h�X�e�[�g���쐬
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
/// ���Q�b�^�[��
/// </summary>

// ���[���h�s��
const Matrix& Obj3d::getWorld()
{
	return m_world;
}

// ��]�p
const Vector3& Obj3d::getRotate()
{
	return m_rotate;
}

// ���s�ړ�
const Vector3& Obj3d::getTranse()
{
	return m_transe;
}

// �e�I�u�W�F�N�g
Obj3d * Obj3d::getParent()
{
	return m_parent;
}

/// <summary>
/// ���Q�b�^�[��
/// </summary>

/// <summary>
/// �X�V����
/// </summary>
void Obj3d::update()
{
	/// <summary>
	/// ���[���h�s����v�Z
	/// </summary>
	// �X�P�[�����O�s��
	Matrix scaleMat = Matrix::CreateScale(m_scale);
	// ��]�s��
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
	// ���s�ړ��s��
	Matrix transMat = Matrix::CreateTranslation(m_transe);

	// ���[���h�s���ݒ�
	m_world = scaleMat * rotateMat * transMat;
	// �e������΂���world�s���������
	if (m_parent != nullptr)
	{
		m_world *= m_parent->getWorld();
	}
}

/// <summary>
/// �`��
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
//		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
//		m_pModel->Draw(s_pDeviceContext, *s_pStates, m_LocalWorld, view, projection, false, Obj3D::SetSubtractive);
//	}
//}

/// <summary>
/// ���f����ǂݍ���
/// </summary>
/// <param name="szFileName">�t�@�C����</param>
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
*	@brief �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
*/
void Obj3d::DisableLighting()
{
	if (m_pModel)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// ���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}