#include "stdafx.h"
#include <d2d1.h>
#include "basewin.h"
#include "Bypass.h"
#include "Offsets.h"
#include "Vector.h"
#include "guicon.h"
#pragma comment(lib, "d2d1")


//Variables and forward declarations
const int iRadarRadius = { 150 };
const float fRadarRadius = { 150 };
const float radiusIcon = { 4 };
Vec2 dirVec {};

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class MainWindow : public BaseWindow<MainWindow>
{
public:
	ID2D1Factory            *pFactory;
	ID2D1HwndRenderTarget   *pRenderTarget;
	ID2D1SolidColorBrush    *pBrush;
	D2D1_ELLIPSE            ellipse;
	D2D1_ELLIPSE            pEllipse;
	D2D1_ELLIPSE            botEllipse;
	D2D1_RECT_F				pRect;
	D2D1_RECT_F				botRect;
	

	void    CalculateLayout();
	void	PlayerIcon();
	void	BotIcon(Vec2 &v);
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint();
	void    Resize();

	MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
	{
	}

	PCWSTR  ClassName() const { return L"myWin"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

// Recalculate drawing layout when the size of the window changes.
void MainWindow::CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

void MainWindow::PlayerIcon()
{
	if (pRenderTarget != NULL)
	{
		pEllipse = D2D1::Ellipse(D2D1::Point2F(fRadarRadius, fRadarRadius), radiusIcon, radiusIcon);
		pRect = D2D1::RectF(fRadarRadius, fRadarRadius, fRadarRadius, fRadarRadius);
	}
}

void MainWindow::BotIcon(Vec2 &v)
{
	if (pRenderTarget != NULL)
	{
		botEllipse = D2D1::Ellipse(D2D1::Point2F(fRadarRadius + v.getxPos(), fRadarRadius + v.getyPos()), radiusIcon, radiusIcon);
		botRect = D2D1::RectF(fRadarRadius + v.getxPos(), fRadarRadius - v.getyPos(), fRadarRadius - v.getxPos(), fRadarRadius + v.getyPos());
	}
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);
	}

	if (SUCCEEDED(hr))
	{
		const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::DarkGray);
		hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

		if (SUCCEEDED(hr))
		{
			CalculateLayout();
		}
	}

	return hr;
}

void MainWindow::DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DimGray));
		pRenderTarget->FillEllipse(ellipse, pBrush);

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}


void MainWindow::Resize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	// Adding Console I/O to a GUI App	
	#ifdef _DEBUG
	RedirectIOToConsole(); 
	#endif

	//Bypass
	myGame assaultCube{ TEXT("AssaultCube") };
	HWND windowHandle = { assaultCube.findWindow() };
	DWORD PID = { assaultCube.getPID(windowHandle) };
	Bypass bypass;
	bypass.Attach(PID);

	//Game data
	uintptr_t tempPtr = { 0 };
	PlayerList pl;
	Game g;
	Player p;
	Player bot;

	//Creates a window
	MainWindow win;

	if (!win.Create(L"Radar", WS_OVERLAPPEDWINDOW, 0, 0, 0, iRadarRadius*2, iRadarRadius*2))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, win.Window(), 0, 0, PM_REMOVE))
		{DispatchMessage(&msg);}
		else
		{
			//Update in-game data
			bypass.Read(g.convertGamePtr(g.GetInstance()), &g, sizeof(g), 0);
			uintptr_t m_LocalPlayerPtr = { g.convertPlayerPtr(g.m_LocalPlayer) };
			uintptr_t m_OtherPlayersPtr = { g.convertPlayerListPtr(g.m_OtherPlayers) };
			bypass.Read(m_OtherPlayersPtr, &pl, sizeof(pl), 0);
			bypass.Read(m_LocalPlayerPtr + 0x0004, &p, sizeof(p), 0);
			bypass.Read(pl.convertPlayerPtr(pl.GetPlayer(1)) + 0x0004, &bot, sizeof(bot), 0); //if (playerList[i].iHealth <= 0) continue;

			//Obtain vec2 of player
			Vec2 myPos = { myPos.vec3ToVec2(p.m_HeadPos) }; 
			Vec2 currPlayer = { myPos.vec3ToVec2(bot.m_HeadPos) }; 

			//Reduce it to a direction-vector
			Vec2 dirVec = { myPos.subVector(currPlayer) };

			//Scale it and clip it if it is out of our radar-bounds
			if (dirVec.getVectorLength() > fRadarRadius)
			{
				dirVec.normalizeVector();
				dirVec.multiVector(fRadarRadius);
			}

			//Rotate the relative vector by the local player's yaw
			//dirVec = dirVec.RotatePoint(myPos, p.m_Yaw, false);  This feature needs some rework


			//Render radar
			win.PlayerIcon();
			win.BotIcon(dirVec);
			win.pRenderTarget->BeginDraw();

			win.pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DimGray));
			win.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkGray));
			win.pRenderTarget->FillEllipse(win.ellipse, win.pBrush);

			win.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gold));
			win.pRenderTarget->FillEllipse(win.pEllipse, win.pBrush);
			//pRenderTarget->FillRectangle(pRect, pBrush);

			win.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			win.pRenderTarget->FillEllipse(win.botEllipse, win.pBrush);			
			//pRenderTarget->FillRectangle(botRect, pBrush);

			win.pRenderTarget->EndDraw();

		}
	}

	return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_SIZE:
		Resize();
		return 0;

	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}