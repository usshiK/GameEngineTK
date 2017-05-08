//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <ctime>

extern void ExitGame();

/* -- ���O��Ԃ���� ---- */
using namespace DirectX;
using namespace DirectX::SimpleMath;


using Microsoft::WRL::ComPtr;


/* -- �v���g�^�C�v�錾 ---- */
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

	// �v���~�e�B�u�o�b�`�̏�����
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal/*VertexPositionColor*/>>(m_d3dContext.Get());

	// �G�t�F�N�g�̏�����
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	// �ʂ�����ݒ�
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

	// �f�o�b�O�J�����̐���
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// �G�t�F�N�g�t�@�N�g���[����
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	// �e�N�X�`���̓ǂݍ��݃t�H���_���w��
	m_factory->SetDirectory(L"Resources");

	// ���f����ǂݍ���
	m_ground = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources/ground20m.cmo",
		*m_factory
	);

	m_skyModel = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources/sky.cmo",
		*m_factory
	);

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


	// �t���[������������	
	m_frame = 0;

	// �e�B�[�|�b�h�̈ʒu�������_���Ō��߂�
	for (int i = 0; i < TEAPOD_NUM; i++)
	{
		m_teaPodPos[i] = rand() % 100;
		m_teaPodRot[i] = rand() % 360;
	}

	// ���{�b�g
	m_robotBase = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources/robotBase.cmo",
		*m_factory
	);
	m_rotY = 0.0f;

	// �L�[�{�[�h�̏�����
	m_keyBoard = std::make_unique<Keyboard>();


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

	// �f�o�b�O�J�����̍X�V
	m_debugCamera->Update();


	// �Q�[���̖��t���[������

	// �L�[�{�[�h����
	// �L�[�{�[�h�̏�Ԃ��擾
	auto kb = m_keyBoard->GetState();

	// ���@�̕����]��
	if (kb.A)
	{
		m_rotY += 0.1f;
	}
	if (kb.D)
	{
		m_rotY -= 0.1f;
	}

	// ���@�ɔ��f
	m_rotMatRobot = Matrix::CreateRotationY(m_rotY);
	// ���@�̑O�i���
	if (kb.W)
	{
		// �O���Ɉړ�
		Vector3 moveV(0.0f,0.0f,-0.1f);

		moveV = Vector3::TransformNormal(moveV,m_woldRobot);

		// ���@�ɔ��f����
		m_tankPos += moveV;
	}
	if (kb.S)
	{
		// �O���Ɉړ�
		Vector3 moveV(0.0f, 0.0f, 0.1f);

		moveV = Vector3::TransformNormal(moveV, m_woldRobot);

		// ���@�ɔ��f����
		m_tankPos += moveV;
	}

	{// ���@�̃��[���h�s����v�Z
		m_trnsMatRobot = Matrix::CreateTranslation(m_tankPos);

		m_woldRobot = m_rotMatRobot * m_trnsMatRobot;
	}
	// ���̃��[���h�s��̌v�Z
	for (int i = 0; i < KYUU_NUM; i++)
	{
		// �X�P�[�����O�s��
		Matrix scaleMat = Matrix::Identity;

		// ���s�ړ��s��
		Matrix transMat = Matrix::Identity;

		// ��]�s��
		Matrix rotMatZ = Matrix::Identity;		// ���[����
		Matrix rotMatX = Matrix::Identity;		// �s�b�`��(�p)
		Matrix rotMatY = Matrix::Identity;		// �@���[��(���ʊp)
		Matrix rotMat = Matrix::Identity;		// ��O������

		// �����̐ݒ�
		if (i < 10)
		{
			scaleMat = Matrix::CreateScale((m_frame % 90) / 30 + 1);

			transMat = Matrix::CreateTranslation(20.0f, 0.0f, 0.0f);

			rotMatY = Matrix::CreateRotationY(XMConvertToRadians((i * 36.0f) + m_frame % 360));		// �@���[��(���ʊp)
		}
		// �O���̐ݒ�
		else if (i < 20)
		{
			transMat = Matrix::CreateTranslation(40.0f, 0.0f, 0.0f);

			rotMatY = Matrix::CreateRotationY(XMConvertToRadians(((i * 36.0f) - m_frame%360)));		// �@���[��(���ʊp)
		}
		else
		{
			scaleMat = Matrix::CreateScale(3.0f);

			rotMatY = Matrix::CreateRotationY(XMConvertToRadians(((i * 36.0f) - m_frame % 360)));		// �@���[��(���ʊp)
		}

		// ��O������
		rotMat = rotMatZ * rotMatX * rotMatY;								

		// ���[���h�s��̍���
		m_worldKyuu[i] = scaleMat * transMat * rotMat;
	}

	// �e�B�[�|�b�h�̍s��̌v�Z
	for (int i = 0; i < TEAPOD_NUM; i++)
	{
		// �X�P�[�����O�s��
		Matrix scaleMat = Matrix::Identity;

		// ���s�ړ��s��
		Matrix transMat = Matrix::Identity;

		// ��]�s��
		Matrix rotMatZ = Matrix::Identity;		// ���[����
		Matrix rotMatX = Matrix::Identity;		// �s�b�`��(�p)
		Matrix rotMatY = Matrix::Identity;		// �@���[��(���ʊp)
		Matrix rotMat = Matrix::Identity;		// ��O������

		// �X�P�[�����O�s��
#define SCALE_SPD 100.0f
		scaleMat = Matrix::CreateScale(/*sinWave(m_frame / SCALE_SPD)+0.1f*/0.5f);

		// �ʒu�������_���ɐݒ�
		float a = 10;
		int a2 = a*10;
		float b = (m_frame % a2) /10;
		transMat = Matrix::CreateTranslation(m_teaPodPos[i] * (1.0f-((b+1)/a)), 0.0f, 0.0f);

		// ��]�s��
		rotMatY = Matrix::CreateRotationY(m_teaPodRot[i]);

		// ��]�s�������
		rotMat = rotMatZ * rotMatX * rotMatY;
#define ROT_SPD 10
		Matrix rotRev = Matrix::CreateRotationY(static_cast<float>(m_frame % (360 * ROT_SPD))/ ROT_SPD);

		// �O��̍s�������
		m_worldTeaPod[i] =  scaleMat * rotRev * transMat * rotMat;
	}

	// �t���[���𐔂���
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

	// ���_�C���f�b�N�X
	uint16_t indices[]
	{
		0,1,2,
		2,1,3
	};

	// ���_�f�[�^
	VertexPositionNormal vertices[]
	{
		{ Vector3(-1.f,+1.f,0.f),Vector3(0.f,0.f,1.f) },
		{ Vector3(-1.f,-1.f,0.f),Vector3(0.f,0.f,1.f) },
		{ Vector3(+1.f,+1.f,0.f),Vector3(0.f,0.f,1.f) },
		{ Vector3(+1.f,-1.f,0.f),Vector3(0.f,0.f,1.f) },
	};

    // TODO: Add your rendering code here.
	// �����_�[�����͂����ɏ���

	// ���낢��ݒ�
	m_states = std::make_unique<CommonStates>(m_d3dDevice1.Get());
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);	// �����x
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);			// �`�悷�鏇��
	m_d3dContext->RSSetState(m_states->Wireframe());							// ���ʂ��猩���邩

	// �r���[�s������				���J�����̈ʒu			������			����͂ǂ�����
	//m_view = Matrix::CreateLookAt(Vector3(0.f, 3.f, 3.f),Vector3::Zero, Vector3::UnitY);
	// �f�o�b�O�J��������r���[�s����擾
	m_view = m_debugCamera->GetCameraMatrix();

	// �ˉe�s������								������p(XM_PI =��)	����ʂ̏c�Ɖ��̒����̔䗦			����ԋ߂����ƈ�ԉ�����
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,float(m_outputWidth) / float(m_outputHeight), 0.1f, 200.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);


	// �G�t�F�N�g�ƃ��C�A�E�g��ݒ�
	m_effect->Apply(m_d3dContext.Get());				
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());	

	
	// ���`��
	m_skyModel->Draw(m_d3dContext.Get(),
		*m_states,
		m_world, m_view, m_proj);

	// �n�ʂ�`��
	m_ground->Draw(m_d3dContext.Get(), 
		*m_states, 
		m_world, m_view, m_proj);

	// ����`��
	//for (int i = 0; i < KYUU_NUM; i++)
	//{
	//	m_kyuu->Draw(m_d3dContext.Get(),
	//		*m_states,
	//		m_worldKyuu[i], m_view, m_proj);
	//}

	//for (int i = 0; i < TEAPOD_NUM; i++)
	//{
	//	m_teaPod->Draw(m_d3dContext.Get(),
	//		*m_states,
	//		m_worldTeaPod[i], m_view, m_proj);
	//}

	// ���{�b�g�̃L���^�s��
	m_robotBase->Draw(m_d3dContext.Get(),
		*m_states,
		m_woldRobot, m_view, m_proj);


	// �`�悷��
	m_batch->Begin();
	//m_batch->DrawLine(
	//	VertexPositionColor(Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)),
	//	VertexPositionColor(Vector3(800.0f, 600.0f, 0.0f), Color(1.0f, 1.0f, 1.0f))
	//);

	//// �O�p�`�̎O�ӂ̐ݒ�	
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::White);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Aqua);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Aqua);
	//VertexPositionColor v4(Vector3(-0.5f, -0.5f, 0.5f), Colors::Aqua);

	//// �O�ӂ����ƂɎO�p�`��`��
	//m_batch->DrawTriangle(v1, v2, v3);
	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	m_batch->End();

    Present();
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