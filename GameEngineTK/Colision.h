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


// �@���t���O�p�`�i�����v��肪�\�ʁj
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// �@���x�N�g��
};


// ���Ƌ��̏Փ˔���
bool checkSphereToSphere(const Sphere & sphereA, const Sphere & sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);

void ClosestPtPoint2Triangle(const DirectX::SimpleMath::Vector3 & _point, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _closest);

bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3 & _point, const Triangle & _triangle);

bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);
