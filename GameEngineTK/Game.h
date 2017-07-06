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

#include "SpriteBatch.h"
#include "WICTextureLoader.h"

//#include"Camera.h"
#include "FollowCamera.h"
#include "Obj3d.h"
#include "Player.h"
#include "Enemy.h"
#include "ModelEffect.h"
#include "LandShape.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
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

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>>	m_batch;		// プリミティブバッチ
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
	//Obj3d m_ground;
	Obj3d m_objSkyModel;
	std::unique_ptr<DirectX::Model> m_kyuu;
	std::unique_ptr<DirectX::Model> m_teaPod;
	// 地形
	LandShape m_landShape;


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
	std::unique_ptr<Player> m_player;

	// 敵ロボット
	const int enemeyNum = 15;
	std::vector<std::unique_ptr<Enemy>>m_enemy;

	// フレームを数える
	int m_frame;
	float m_roboAngleX;
	float m_roboAngleY;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyBoard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// カメラ
	std::unique_ptr<FollowCamera> m_camera;
	//std::unique_ptr<FollowCamera> m_camera;

	// カメラを設定する関数
	void setCamera();
};