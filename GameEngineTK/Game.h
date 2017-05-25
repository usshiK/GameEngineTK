//
// Game.h
//

#pragma once

/* -- ヘッダーのインクルード ---- */
#include "StepTimer.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "DebugCamera.h"
#include <Keyboard.h>

//#include"Camera.h"
#include "FollowCamera.h"
#include "Obj3d.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
	// プレイヤーのパーツに番号を付ける
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_BASE,	// キャタピラ
		PLAYER_PARTS_BODY,	// 体
		PLAYER_PARTS_BREAST,// 胸

		PLAYER_PARTS_NUM
	};

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal/*VertexPositionColor*/>>	m_batch;		// プリミティブバッチ
	std::unique_ptr<DirectX::BasicEffect>									m_effect;		// ベーシックエフェクト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>								m_inputLayout;	// インプットレイアウト
	std::unique_ptr<DirectX::CommonStates>									m_states;		// コモンステイト

	// 行列
	DirectX::SimpleMath::Matrix		m_world;
	DirectX::SimpleMath::Matrix		m_view;
	DirectX::SimpleMath::Matrix		m_proj;

	// デバッグカメラ
	std::unique_ptr<DebugCamera>	m_debugCamera;

	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	// モデル
	std::unique_ptr<DirectX::Model> m_ground;
	Obj3d m_objSkyModel;
	std::unique_ptr<DirectX::Model> m_kyuu;
	std::unique_ptr<DirectX::Model> m_teaPod;

	// 球のワールド行列
#define KYUU_NUM 21
	DirectX::SimpleMath::Matrix m_worldKyuu[KYUU_NUM];

#define TEAPOD_NUM 20
	DirectX::SimpleMath::Matrix m_worldTeaPod[TEAPOD_NUM];
	int m_teaPodPos[TEAPOD_NUM];
	int m_teaPodRot[TEAPOD_NUM];

	// ロボットの座標
	DirectX::SimpleMath::Vector3 m_tankPos;
	// ロボット
	std::vector<Obj3d>m_player;

	// フレームを数える
	int m_frame;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyBoard;

	// カメラ
	std::unique_ptr<FollowCamera> m_camera;

	// カメラを設定する関数
	void setCamera();
};