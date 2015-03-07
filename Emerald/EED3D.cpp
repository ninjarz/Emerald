#include "EED3D.h"
#include "EECore.h"

//#define _PIX_DEBUG_

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EED3D
	//----------------------------------------------------------------------------------------------------
	EED3D::EED3D()
		:
		m_device(NULL),
		m_deviceContext(NULL),
		m_screenNear(),
		m_screenDeep(),
		m_isSSAA(),
		m_isVsync(),
		m_videoCardMemory(),
		m_x4MultiQuality(),
		m_swapChain(NULL),
		m_depthStencilBuffer(NULL),
		m_renderTargetView(NULL),
		m_depthStencilView(NULL)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EED3D::EED3D(const EED3D&)
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EED3D::Initialize(bool _isSSAA, bool _isVsync)
	{
		m_isSSAA = _isSSAA;
		m_isVsync = _isVsync;

		HRESULT hr;

		/******************************获取显示刷新率，用于垂直同步******************************/
		/******************************获取显示刷新率，用于垂直同步******************************/
		//保存垂直同步设定
		//获取显示刷新率（用于垂直同步）
		IDXGIFactory* factory;
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(hr))
		{
			return false;
		}

		IDXGIAdapter* adapter;
		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr))
		{
			return false;
		}

		IDXGIOutput* adapterOutput;
		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr))
		{
			return false;
		}

		unsigned int numModes = 0;
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		DXGI_MODE_DESC* displayModeList;
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(hr))
		{
			return false;
		}
		unsigned int mWidth = GetSystemMetrics(SM_CXSCREEN);             //获取和当前屏幕相匹配的分辨率
		unsigned int mHeight = GetSystemMetrics(SM_CYSCREEN);            //获取和当前屏幕相匹配的分辨率
		unsigned int numerator, denominator;                             //用于保存和当前屏幕分辨率匹配的刷新率
		for (unsigned int i = 0; i<numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)mWidth
				&& displayModeList[i].Height == (unsigned int)mHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************获取显卡显存以及显卡的型号******************************/
		/******************************获取显卡显存以及显卡的型号******************************/
		//获取显存
		DXGI_ADAPTER_DESC adapterDesc;
		hr = adapter->GetDesc(&adapterDesc);
		if (FAILED(hr))
		{
			return false;
		}

		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		int error;
		unsigned int stringLength;
		error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}
		MessageBoxA(NULL, m_videoCardDescription, "当前显卡型号", MB_OK);
		delete[] displayModeList;
		displayModeList = NULL;
		adapterOutput->Release();
		adapterOutput = NULL;
		adapter->Release();
		adapter = NULL;
		factory->Release();
		factory = NULL;
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************创建D3D设备，D3D设备上下文******************************/
		/******************************创建D3D设备，D3D设备上下文******************************/
		//创建D3D设备，D3D设备上下文
		D3D_FEATURE_LEVEL featureLevels[6] = {
			D3D_FEATURE_LEVEL_11_0,        //特性等级
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		D3D_FEATURE_LEVEL curLevel;

		hr = D3D11CreateDevice(
			NULL,                            //显卡指针,NULL使用默认的适配器
			D3D_DRIVER_TYPE_HARDWARE,        //驱动类型,当硬件不支持时可以使用D3D_DRIVER_TYPE_REFERENCE
			NULL,                            //software driver，DLL中的一个软件光栅句柄，如果使用硬件加速则为NULL
			0,                               //D3D_CREATE_DEVICE_FLAG
			featureLevels,                   //指向一个D3D_FEATURE_LEVEL数组的指针
			6,                               //pFeatureLevels中的元素数量
			D3D11_SDK_VERSION,               //SDK版本，使用D3D11_SDK_VERSION
			&m_device,                       //返回一个创建好的D3D设备对象的指针地址
			&curLevel,                       //返回一个D3D_FEATURE_LEVEL指针
			&m_deviceContext);               //返回一个D3D设备上下文对象指针
		if (FAILED(hr))
		{
			return false;
		}
		if (curLevel != D3D_FEATURE_LEVEL_11_0)
		{
			if (IDNO == MessageBoxW(NULL, L"您的机器不支持D3D11特性，程序可能无法正确显示效果，要继续吗？", L"警告", MB_YESNO))
			{
				return false;
			}
		}
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************创建交换链以及渲染目标视图******************************/
		/******************************创建交换链以及渲染目标视图******************************/
		//获取多重采样支持等级
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_x4MultiQuality);
		//填写交换链描述
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferDesc.Width = EECore::s_EECore->GetWidth();                          //缓冲区宽度
		swapChainDesc.BufferDesc.Height = EECore::s_EECore->GetHeight();                        //缓冲区高度
		if (m_isVsync)                                                    //刷新率，这里对是否垂直同步进行不同的设置
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 70;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;             //缓冲区数据格式
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		if (m_isSSAA)
		{
			swapChainDesc.SampleDesc.Count = 4;                                      //多重采样（反锯齿）中每点像素的采样点个数
			swapChainDesc.SampleDesc.Quality = m_x4MultiQuality - 1;                     //质量等级，0等级表示不使用多重采样
		}
		else
		{
			swapChainDesc.SampleDesc.Count = 1;                                      //多重采样（反锯齿）中每点像素的采样点个数
			swapChainDesc.SampleDesc.Quality = 0;                                      //质量等级，0等级表示不使用多重采样
		}
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;        //Usage
		swapChainDesc.BufferCount = 1;                                      //缓冲区个数  
		swapChainDesc.OutputWindow = EECore::s_EECore->GetHWnd();                                 //主窗口句柄
		if (EECore::s_EECore->GetIsFullScreen())                                                  //是否窗口化
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;                    //枚举型，交换方式
		swapChainDesc.Flags = 0;                                                //枚举型，swap-chain behavior
		//创建交换链
		IDXGIDevice *pDxgiDevice(NULL);
		hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDxgiDevice));
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"Get DXGIDevice failed!", L"ERROR", MB_OK);
			return false;
		}
		IDXGIAdapter *pDxgiAdapter(NULL);
		hr = pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDxgiAdapter));
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"Get DXGIAdapter failed!", L"ERROR", MB_OK);
			return false;
		}
		IDXGIFactory *pDxgiFactory(NULL);
		hr = pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDxgiFactory));
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"Get DXGIFactory failed!", L"ERROR", MB_OK);
			return false;
		}
		hr = pDxgiFactory->CreateSwapChain(m_device,                     //指向一个创建好的D3D设备对象的指针地址
			&swapChainDesc,               //指向一个交换链描述的指针
			&m_swapChain);                //返回一个交换链对象的指针地址，这个交换链用于渲染
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"Create swap chain failed!", L"ERROR", MB_OK);
			return false;
		}
		pDxgiFactory->Release();
		pDxgiFactory = NULL;
		pDxgiAdapter->Release();
		pDxgiAdapter = NULL;
		pDxgiDevice->Release();
		pDxgiDevice = NULL;

		//创建渲染目标视图(将后置缓存绑定到渲染目标视图上)
		ID3D11Texture2D* backBufferPtr;
		hr = m_swapChain->GetBuffer(0,                            //后置缓存的引索，一般只有一个后置缓存
			__uuidof(ID3D11Texture2D),    //缓存接口类型
			(LPVOID*)&backBufferPtr);     //获得交换链后缓存地址，存放在backBufferPtr
		if (FAILED(hr))
		{
			return false;
		}

#ifndef _PIX_DEBUG_
		hr = m_device->CreateRenderTargetView(backBufferPtr,                                    //视图对应资源
			NULL,                                             //视图描述，先前已经定义了后置缓存的数据类型
			&m_renderTargetView);                             //要创建的视图（指针的地址）
		if (FAILED(hr))
		{
			return false;
		}
#endif
		//backBufferPtr->Release();
		//backBufferPtr = 0;
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************创建深度/模版缓存，深度/模版视图******************************/
		/******************************创建深度/模版缓存，深度/模版视图******************************/
		//填写深度模版缓存描述
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
		depthStencilBufferDesc.Width = EECore::s_EECore->GetWidth();             //深度缓存的宽度
		depthStencilBufferDesc.Height = EECore::s_EECore->GetHeight();            //深度缓存的高度
		depthStencilBufferDesc.MipLevels = 1;                                 //贴图细化
		depthStencilBufferDesc.ArraySize = 1;                                 //纹理数组
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;     //数据格式，一般为DXGI_FORMAT_D24_UNORM_S8_UINT,24位用于深度，8位用于模板  
		if (m_isSSAA)
		{
			depthStencilBufferDesc.SampleDesc.Count = 4;                                 //多重采样（必须与前面的一样）
			depthStencilBufferDesc.SampleDesc.Quality = m_x4MultiQuality - 1;                //多重采样（必须与前面的一样）
		}
		else
		{
			depthStencilBufferDesc.SampleDesc.Count = 1;                                 //多重采样（必须与前面的一样）
			depthStencilBufferDesc.SampleDesc.Quality = 0;                                 //多重采样（必须与前面的一样）
		}
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;               //其中DEFAULT为只让GPU读写资源 ，而不让CPU读写
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;          //绑定类型，在这里是深度/模版缓存
		depthStencilBufferDesc.CPUAccessFlags = 0;                                 //设0为CPU不可访问，深度/模版缓存只需要GPU读写就可以了
		depthStencilBufferDesc.MiscFlags = 0;                                 //这个参数在这里不适用于于深度/模版缓存
		//创建深度模板缓存
		hr = m_device->CreateTexture2D(&depthStencilBufferDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(hr))
			return false;

		//初始化深度模板视图的描述体
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;     //如果使用了多重采样这边要使用特定宏（此部分没有使用，预留）
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//创建深度模板视图
		hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);
		if (FAILED(hr))
			return false;
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************将渲染目标视图和深度模版视图绑定到渲染管线******************************/
		/******************************将渲染目标视图和深度模版视图绑定到渲染管线******************************/
		//将渲染目标视图和深度模版视图绑定到渲染管线(Output Merger Stage!!!)
		m_deviceContext->OMSetRenderTargets(1,                                //RenderTarget个数
			&m_renderTargetView,              //RenderTarget数组
			m_depthStencilView);              //DepthStencil view
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************创建视口******************************/
		/******************************创建视口******************************/
		//设置视口信息
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;                                //视口左上角在屏幕上x坐标
		viewport.TopLeftY = 0.0f;                                //视口左上角在屏幕上y坐标
		viewport.Width = (float)EECore::s_EECore->GetWidth();                      //视口宽度
		viewport.Height = (float)EECore::s_EECore->GetHeight();                     //视口高度
		viewport.MinDepth = 0.0f;                                //最小深度值：0.0f
		viewport.MaxDepth = 1.0f;                                //最大深度值：1.0f  

		//创建视口
		m_deviceContext->RSSetViewports(1,               //要绑定的视口数量（可用于屏幕分区，例如双人游戏模式等）
			&viewport);      //视口数组
		/******************************************************************************************/
		/******************************************************************************************/

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EED3D::Shutdown()
	{
		//m_swapChain->SetFullscreenState(false, NULL);

		SAFE_RELEASE(m_depthStencilView);
		SAFE_RELEASE(m_depthStencilBuffer);
		SAFE_RELEASE(m_renderTargetView);
		SAFE_RELEASE(m_deviceContext);
		SAFE_RELEASE(m_device);
		SAFE_RELEASE(m_swapChain);

		return;
	}

	//----------------------------------------------------------------------------------------------------
	void EED3D::BeginScene(float red, float green, float blue, float alpha)
	{
		float color[4];

		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		return;
	}

	//----------------------------------------------------------------------------------------------------
	void EED3D::BeginScene(const FLOAT4 &_float4)
	{
		float color[4];

		color[0] = _float4.x;
		color[1] = _float4.y;
		color[2] = _float4.z;
		color[3] = _float4.w;

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		return;
	}

	//----------------------------------------------------------------------------------------------------
	void EED3D::EndScene()
	{
		if (m_isVsync)
		{
			m_swapChain->Present(1, 0);
		}
		else
		{
			m_swapChain->Present(0, 0);
		}

		EECore::s_EECore->ClearMouseDeltaXY();
		EECore::s_EECore->ClearMouseDeltaM();

		return;
	}

	//----------------------------------------------------------------------------------------------------
	bool EED3D::SetDevice(ID3D11Device* _para) { m_device = _para; return true; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::SetDeviceeContext(ID3D11DeviceContext* _para) { m_deviceContext = _para; return true; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::SetScreenNear(float _para) { m_screenNear = _para; return true; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::SetScreenDeep(float _para) { m_screenDeep = _para; return true; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::SetIsSSAA(bool _para) { m_isSSAA = _para; return true; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::SetIsVsync(bool _para) { m_isVsync = _para; return true; };

	//----------------------------------------------------------------------------------------------------
	ID3D11Device* EED3D::GetDevice() const { return m_device; };

	//----------------------------------------------------------------------------------------------------
	ID3D11DeviceContext* EED3D::GetDeviceContext() const { return m_deviceContext; };

	//----------------------------------------------------------------------------------------------------
	float EED3D::GetScreenNear() const { return  m_screenNear; };

	//----------------------------------------------------------------------------------------------------
	float EED3D::GetScreenDeep() const { return  m_screenDeep; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::GetIsSSAA() const { return  m_isSSAA; };

	//----------------------------------------------------------------------------------------------------
	bool EED3D::GetIsVsync() const { return  m_isVsync; }; 

	//----------------------------------------------------------------------------------------------------
	void EED3D::GetVideoCardInfo(char* cardName, int& memory)
	{
		strcpy_s(cardName, 128, m_videoCardDescription);
		memory = m_videoCardMemory;
		return;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11RenderTargetView* EED3D::GetRenderTargetView() const { return m_renderTargetView; }

	//----------------------------------------------------------------------------------------------------
	ID3D11DepthStencilView* EED3D::GetDepthStencilView() const { return m_depthStencilView; }

	//EED3D_APIs
	//----------------------------------------------------------------------------------------------------
	void EEBeginScene(float red, float green, float blue, float alpha) { EECore::s_EECore->GetEED3D()->BeginScene(red, green, blue, alpha); }

	//----------------------------------------------------------------------------------------------------
	void EEBeginScene(const FLOAT4 &_float4) { EECore::s_EECore->GetEED3D()->BeginScene(_float4); }

	//----------------------------------------------------------------------------------------------------
	void EEEndScene() { EECore::s_EECore->GetEED3D()->EndScene(); }

	//----------------------------------------------------------------------------------------------------
	ID3D11Device* EEGetDevice() { return EECore::s_EECore->GetEED3D()->GetDevice(); }
	ID3D11DeviceContext* EEGetDeviceContext() { return EECore::s_EECore->GetEED3D()->GetDeviceContext(); }
	float EEGetScreenNear() { return EECore::s_EECore->GetEED3D()->GetScreenNear(); }
	float EEGetScreenDeep() { return EECore::s_EECore->GetEED3D()->GetScreenDeep(); }
	bool EEGetIsSSAA() { return EECore::s_EECore->GetEED3D()->GetIsSSAA(); }
	bool EEGetIsVsync() { return EECore::s_EECore->GetEED3D()->GetIsVsync(); }
	void EEGetVideoCardInfo(char* _data, int& _length) { return EECore::s_EECore->GetEED3D()->GetVideoCardInfo(_data, _length); }
	ID3D11RenderTargetView* EEGetRenderTargetView() { return EECore::s_EECore->GetEED3D()->GetRenderTargetView(); }
	ID3D11DepthStencilView* EEGetDepthStencilView() { return EECore::s_EECore->GetEED3D()->GetDepthStencilView(); }
}