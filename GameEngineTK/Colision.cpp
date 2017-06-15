/// <summary>
/// �Փ˔��胉�C�u����
/// </summary>
/* -- �w�b�_�[�̃C���N���[�h ---- */
#include "Colision.h"

/* -- ���O��Ԃ���� ---- */
using namespace DirectX::SimpleMath;

/// <summary>
/// ���Ƌ��̏Փ˔���
/// </summary>
/// <param name="sphereA">��A</param>
/// <param name="sphereB">��B</param>
/// <returns>�Փ˂��Ă�����true</returns>
bool checkSphereToSphere(const Sphere &sphereA, const Sphere& sphereB)
{
	// ��̋��̒��S���W�̍����o��
	Vector3 sub = sphereA.center - sphereB.center;

	// sub�̒��������߂�(�s�^�S��)	
	//float distanse = sqrtf(sub.x*sub.x + sub.y*sub.y + sub.z*sub.z);

	// sqrtf�͎��͏d���A�Ȃ�ׂ��g�������Ȃ� 
	// ����Isqutf���g���Ƃ�����
	float distanseSquare = sub.x*sub.x + sub.y*sub.y + sub.z*sub.z;
	// radius����悷��΁A���ʕς�����
	float radiusSquare = sphereA.radius + sphereB.radius;
	radiusSquare *= radiusSquare;

	// ��̔��a�̘a���o��
	//float radius = sphereA.radius + sphereB.radius;

	// ��̔��a�̘a����sub�̒������傫���Ȃ�
	if (radiusSquare < distanseSquare)
	{
		// �������Ă��Ȃ�
		return false;
	}

	return true;
}