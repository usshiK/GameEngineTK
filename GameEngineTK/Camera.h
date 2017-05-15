/****************************************************************
*|　概要　　　　カメラクラス
*|　作成者　　　GS1 04 牛山航平
*|　作成日　　　2017/05/11
*****************************************************************/
#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	Camera(int width,int height);
	virtual ~Camera();

	void update();

	DirectX::SimpleMath::Matrix getViewMatrix();	// ビュー行列を取得する
	DirectX::SimpleMath::Matrix getProjMatrix();	// ビュー行列を取得する

	void setEyePos(DirectX::SimpleMath::Vector3 eyePos);	// 視点座標をセット
	void setRefPos(DirectX::SimpleMath::Vector3 refPos);	// 注視点をセット
	void setUpVec(DirectX::SimpleMath::Vector3 UpVec);		//	上方向ベクトルをセット
protected:
	// ビュー行列
	DirectX::SimpleMath::Matrix	m_view;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_eyePos;
	// 見ている先(注視点)
	DirectX::SimpleMath::Vector3 m_refPos;
	// 上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upVec;

	// プロジェクション行列
	DirectX::SimpleMath::Matrix		m_proj;
	// 垂直方向視野角(縦方向にどのくらいの角度見えるか)
	float m_fovY;
	// 画面の縦横比
	float m_aspect;
	// 近くの表示限界
	float m_nearClip;
	// 遠くの表示限界
	float m_farClip;

};

