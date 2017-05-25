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

// ���s�ړ�
void Obj3d::setTranse(const Vector3& transe)
{
	m_transe = transe;
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
	Matrix rotateMatX = Matrix::CreateRotationX(m_rotate.x);
	Matrix rotateMatY = Matrix::CreateRotationY(m_rotate.y);
	Matrix rotateMatZ = Matrix::CreateRotationZ(m_rotate.z);
	Matrix rotateMat = rotateMatX * rotateMatY * rotateMatZ;
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
