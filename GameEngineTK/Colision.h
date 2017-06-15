/// <summary>
/// �Փ˔��胉�C�u����
/// </summary>
/* -- �w�b�_�[�̃C���N���[�h ---- */
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>

// �����N���X
class Segment
{
public:
	DirectX::SimpleMath::Vector3 start;	// �n�_
	DirectX::SimpleMath::Vector3 end;	// �I�_
};


// ���̃N���X
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 center;	// ���S���W

	float radius;							// ���a

	Sphere() :radius(0) {};
	~Sphere() {};
};

// �J�v�Z���N���X
class Capsure
{
public:
	Segment segment;	// ����
	float radius;		// �~�̔��a

	Capsure() :radius(0) {};
};

// ���Ƌ��̏Փ˔���
bool checkSphereToSphere(const Sphere & sphereA, const Sphere & sphereB);
