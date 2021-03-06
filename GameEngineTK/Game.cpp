//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <ctime>

#include "Colision.h"

extern void ExitGame();

/* -- 名前空間を解放 ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;


using Microsoft::WRL::ComPtr;



/* -- プロトタイプ宣言 ---- */
float sinWave(float t);

Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	srand(static_cast<unsigned int>(time(nullptr)));

	// プリミティブバッチの初期化
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal/*VertexPositionColor*/>>(m_d3dContext.Get());

	// エフェクトの初期化
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	// 写し方を設定
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	// 
	void const* shaderByteCode;
	size_t byteCodeLength;

	// 
	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// 
	m_d3dDevice.Get()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// デバッグカメラの生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// キーボードの初期化
	m_keyBoard = std::make_unique<Keyboard>();

	// カメラの初期化
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	Vector3 eyepos = Vector3(m_tankPos);
	eyepos += Vector3(0.0f, 5.0f, 5.0f);
	m_camera->setEyePos(eyepos);
	Vector3 refpos = Vector3(m_tankPos.x, m_tankPos.y, m_tankPos.z);
	refpos += Vector3(0.0f, 0.0f, -5.0f);
	m_camera->setRefPos(refpos);
	m_camera->setKeyBoard(m_keyBoard.get());

	// オブジェクトクラスの静的メンバを初期化
	Obj3d::initializeStatic(m_camera.get(), m_d3dDevice, m_d3dContext);

	// ランドシェイプ(地形)の初期化に必要な設定
	LandShapeCommonDef lscDef;
	lscDef.pDevice = m_d3dDevice.Get();
	lscDef.pDeviceContext = m_d3dContext.Get();
	lscDef.pCamera = m_camera.get();
	// 地形の共通初期化
	LandShape::InitializeCommon(lscDef);


	// エフェクトファクトリー生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	// テクスチャの読み込みフォルダを指定
	m_factory->SetDirectory(L"Resources");

	// モデルを読み込む
	//m_ground.LoadModel(L"Resources/ground20m.cmo");
	// 地形
	m_landShape.Initialize(L"ground20m", L"ground20m");

	// 空
	m_objSkyModel.LoadModel(L"Resources/sky.cmo");

	m_kyuu = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources/kyuu.cmo",
		*m_factory
	);

	m_teaPod = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources/teaPod.cmo",
		*m_factory
	);


	// フレーム数を初期化	
	m_frame = 0;
	m_roboAngleX = 0.0f;
	m_roboAngleY = 0.0f;

	// ティーポッドの位置をランダムで決める
	for (int i = 0; i < TEAPOD_NUM; i++)
	{
		m_teaPodPos[i] = rand() % 100;
		m_teaPodRot[i] = rand() % 360;
	}

	// プレイヤーの初期化
	m_player = std::make_unique<Player>();
	m_player->initialize(Vector3(0.0f,0.0f,0.0f),m_keyBoard.get());
	m_camera->setTarget(m_player.get());

	// 敵の初期化
	m_enemy.resize(enemeyNum);
	for (int i = 0; i < enemeyNum; i++)
	{
		m_enemy[i] = std::make_unique<Enemy>();
		m_enemy[i]->initialize(Vector3(rand() % 60 -30, 0.0f, rand() % 60 - 30));
	}
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	// デバッグカメラの更新
	m_debugCamera->Update();


	// ゲームの毎フレーム処理
	// キーボードの状態を取得
	Keyboard::State keyState = m_keyBoard->GetState();
	m_KeyboardTracker.Update(keyState);

	// spaceを押したら
 	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
		{
			it->get()->getCollisionNode().setDebugVisible();
		}
	}

	// プレイヤー更新
	m_player->update();

	// 敵更新
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	{
		it->get()->update();
	}

	// カメラの更新
	m_camera->update();
	m_view = m_camera->getViewMatrix();
	m_proj = m_camera->getProjMatrix();

	// 弾丸と敵のあたり
	{
		// 弾丸のコリジョンノードを取得
		const Sphere& bulletsphere = m_player->getCollisionNodeBullet();

		// 敵
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end();)
		{
			// 敵のコリジョンノードを取得
			const Sphere& enemySphere = it->get()->getCollisionNode();

			// 球と旧のあたりをチェック
			if (checkSphereToSphere(bulletsphere, enemySphere))
			{// 当たった場合

				// 弾丸をもとに戻す
				m_player->resetBullet();

				// ヒットエフェクトを出す
				ModelEffectManager::getInstance()->Entry(
					L"Resources/HitEffect.cmo",	// モデルファイル
					15,	// 寿命フレーム数
					(*it)->getTrance(),	// ワールド座標
					Vector3(0, 0, 0),	// 速度
					Vector3(0, 0, 0),	// 加速度
					Vector3(0, 0, 0),	// 回転角（初期）
					Vector3(0, 0, 0),	// 回転角（最終）
					Vector3(0, 0, 0),	// スケール（初期）
					Vector3(10, 10, 10)	// スケール（最終）
				);
				// 敵を殺す
				it = m_enemy.erase(it);

				// 敵の数が０なら
				if (m_enemy.size() <= 0)
				{
					// クリア
					//m_ground.DisableLighting();
					m_objSkyModel.DisableLighting();
				}
			}
			else
			{// あたっていない場合
				it++;
			}
			
		}
	}

	// 空の更新
	m_objSkyModel.update();

	// 球のワールド行列の計算
	for (int i = 0; i < KYUU_NUM; i++)
	{
		// スケーリング行列
		Matrix scaleMat = Matrix::Identity;

		// 平行移動行列
		Matrix transMat = Matrix::Identity;

		// 回転行列
		Matrix rotMatZ = Matrix::Identity;		// ロール軸
		Matrix rotMatX = Matrix::Identity;		// ピッチ軸(仰角)
		Matrix rotMatY = Matrix::Identity;		// 　ヨー軸(方位角)
		Matrix rotMat = Matrix::Identity;		// 上三つを合成

		// 内側の設定
		if (i < 10)
		{
			scaleMat = Matrix::CreateScale((m_frame % 90) / 30 + 1);

			transMat = Matrix::CreateTranslation(20.0f, 0.0f, 0.0f);

			rotMatY = Matrix::CreateRotationY(XMConvertToRadians((i * 36.0f) + m_frame % 360));		// 　ヨー軸(方位角)
		}
		// 外側の設定
		else if (i < 20)
		{
			transMat = Matrix::CreateTranslation(40.0f, 0.0f, 0.0f);

			rotMatY = Matrix::CreateRotationY(XMConvertToRadians(((i * 36.0f) - m_frame%360)));		// 　ヨー軸(方位角)
		}
		else
		{
			scaleMat = Matrix::CreateScale(3.0f);

			rotMatY = Matrix::CreateRotationY(XMConvertToRadians(((i * 36.0f) - m_frame % 360)));		// 　ヨー軸(方位角)
		}

		// 上三つを合成
		rotMat = rotMatZ * rotMatX * rotMatY;								

		// ワールド行列の合成
		m_worldKyuu[i] = scaleMat * transMat * rotMat;
	}

	// ティーポッドの行列の計算
	for (int i = 0; i < TEAPOD_NUM; i++)
	{
		// スケーリング行列
		Matrix scaleMat = Matrix::Identity;

		// 平行移動行列
		Matrix transMat = Matrix::Identity;

		// 回転行列
		Matrix rotMatZ = Matrix::Identity;		// ロール軸
		Matrix rotMatX = Matrix::Identity;		// ピッチ軸(仰角)
		Matrix rotMatY = Matrix::Identity;		// 　ヨー軸(方位角)
		Matrix rotMat = Matrix::Identity;		// 上三つを合成

		// スケーリング行列
#define SCALE_SPD 100.0f
		scaleMat = Matrix::CreateScale(sinWave(m_frame / SCALE_SPD)+0.1f);

		// 位置をランダムに設定
		float a = 10;
		int a2 = a*10;
		float b = (m_frame % a2) /10;
		transMat = Matrix::CreateTranslation(m_teaPodPos[i] * (1.0f-((b+1)/a)), 0.0f, 0.0f);

		// 回転行列
		//rotMatY = Matrix::CreateRotationY(m_teaPodRot[i]);

		// 回転行列を合体
		rotMat = rotMatZ * rotMatX * rotMatY;
#define ROT_SPD 10
		Matrix rotRev = Matrix::CreateRotationY(static_cast<float>(m_frame % (360 * ROT_SPD))/ ROT_SPD);

		// 三種の行列を合体
		m_worldTeaPod[i] =  scaleMat * rotRev * transMat * rotMat;
	}

	// ヒットエフェクトの更新
	ModelEffectManager::getInstance()->Update();

	// フレームを数える
	m_frame++;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

	// 頂点インデックス
	uint16_t indices[]
	{
		0,1,2,
		2,1,3
	};

	// 頂点データ
	VertexPositionNormal vertices[]
	{
		{ Vector3(-1.f,+1.f,0.f),Vector3(0.f,0.f,1.f) },
		{ Vector3(-1.f,-1.f,0.f),Vector3(0.f,0.f,1.f) },
		{ Vector3(+1.f,+1.f,0.f),Vector3(0.f,0.f,1.f) },
		{ Vector3(+1.f,-1.f,0.f),Vector3(0.f,0.f,1.f) },
	};

    // TODO: Add your rendering code here.
	// レンダー処理はここに書く

	// いろいろ設定
	m_states = std::make_unique<CommonStates>(m_d3dDevice1.Get());
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);	// 透明度
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);			// 描画する順番
	m_d3dContext->RSSetState(m_states->Wireframe());							// 裏面から見えるか

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);


	// エフェクトとレイアウトを設定
	m_effect->Apply(m_d3dContext.Get());				
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());	

	
	// 空を描画
	m_objSkyModel.draw();

	// 地面を描画
	//m_ground.draw();
	m_landShape.Draw();

	// プレイヤーの描画
	m_player->render();

	// 敵
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end();it++)
	{
		it->get()->render();
	}

	// エフェクトの描画
	ModelEffectManager::getInstance()->Draw();

	// 描画する
	m_batch->Begin();

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	m_batch->End();



    Present();
}

/// <summary>
/// カメラをセットする
/// </summary>
void Game::setCamera()
{
	//Vector3 eyepos = Vector3(m_tankPos - m_player[PLAYER_PARTS_BASE].getTrans() * 50);
	//eyepos += Vector3(0.0f, 5.0f, 0.0f);
	//m_camera->setEyePos(eyepos);

	//Vector3 refpos = Vector3(m_tankPos.x +  m_player[PLAYER_PARTS_BASE].getTrans().x * 100, m_tankPos.y, m_tankPos.z + m_player[PLAYER_PARTS_BASE].getTrans().z * 100);
	//m_camera->setRefPos(refpos);
}


// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

float sinWave(float t)
{
	return (sinf(t) + 1.0f) / 2.0f;
};