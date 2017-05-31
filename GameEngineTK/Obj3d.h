/// <summary>
/// 3D�I�u�W�F�N�g�̃N���X
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

	// �Z�b�^�[
	// �X�P�[�����O
	void setScale(const DirectX::SimpleMath::Vector3& scale);

	// ��]�p
	void setRotate(const DirectX::SimpleMath::Vector3& rotate);
	void setRotateX(const float rotate);
	void setRotateY(const float rotate);
	void setRotateZ(const float rotate);

	// ���s�ړ�
	void setTranse(const DirectX::SimpleMath::Vector3& transe);
	void setTranseX(const float transe);
	void setTranseY(const float transe);
	void setTranseZ(const float transe);

	// �e�I�u�W�F�N�g
	void setParent(Obj3d* parent);



	// ���胁���o�ϐ������������� 
	static void initializeStatic(Camera* camera,
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);



	// �Q�b�^�[
	// ���[���h�s��
	const DirectX::SimpleMath::Matrix& getWorld();

	// ��]�p
	const DirectX::SimpleMath::Vector3& getRotate();

	// ���s�ړ�
	const DirectX::SimpleMath::Vector3& getTranse();

	// �e�I�u�W�F�N�g
	Obj3d* getParent();



	// �X�V
	void update();

	// �`��
	void draw();

	// ���f���̓ǂݍ���
	void LoadModel(const wchar_t* szFileName);
private:
	// ���f��
	std::unique_ptr<DirectX::Model>m_pModel;

	// �X�P�[�����O
	DirectX::SimpleMath::Vector3 m_scale;

	// ��]�p
	DirectX::SimpleMath::Vector3 m_rotate;

	// ���s�ړ�
	DirectX::SimpleMath::Vector3 m_transe;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	// �e�I�u�W�F�N�g�̃|�C���^
	Obj3d* m_parent;
private:
	// �J����
	static Camera* m_pCamera;

	// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>  m_d3dDevice;

	// �R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;

	// �R�����X�e�C�g(�ėp�`��ݒ�)
	static std::unique_ptr<DirectX::CommonStates>	m_states;

	// �G�t�F�N�g�t�@�N�g���[
	static std::unique_ptr<DirectX::EffectFactory> m_factory;
};

