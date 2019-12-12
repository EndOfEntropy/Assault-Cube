#pragma once
#include <Windows.h>
#include <dwmapi.h>
#include <Dwrite.h>
#pragma comment (lib, "dwmapi.lib")
#pragma comment(lib, "Dwrite")

template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);


		RECT rect = { x, y, nWidth, nHeight };
		AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			rect.right - rect.left, rect.bottom - rect.top, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }

protected:

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};


class MainWindow : public BaseWindow<MainWindow>
{
public:
	ID2D1Factory			*pFactory;
	ID2D1HwndRenderTarget   *pRenderTarget;
	ID2D1SolidColorBrush	*pBrush;
	IDWriteFactory			*pWriteFactory;
	IDWriteTextFormat		*pTextFormat;


	MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), pWriteFactory(NULL), pTextFormat(NULL)
	{
	}

	PCWSTR  ClassName() const { return L"MainWindow"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void BeginDraw() { pRenderTarget->BeginDraw(); }
	void EndDraw() { pRenderTarget->EndDraw(); }
	void ClearScreen() { pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f))); } //Clear screen

	void DiscardGraphicsResources()
	{
		SafeRelease(&pRenderTarget);
		SafeRelease(&pBrush);
		SafeRelease(&pWriteFactory);
		SafeRelease(&pTextFormat);
	}

	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	

	bool Graphics() //this can be broken down in function to be more modular
	{
		HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		if (res != S_OK) return false;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		res = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget); //Allows alpha blending D2D1_ALPHA_MODE_PREMULTIPLIED
		if (res != S_OK) return false;

		// Define a colour for transparency and set the transparency. (shit method, creates pixelation on text)
		//SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);
		MARGINS margin = { -1, -1, -1, -1 };
		res = DwmExtendFrameIntoClientArea(m_hwnd, &margin); //Margin for the window
		LONG cur_style = GetWindowLong(m_hwnd, GWL_EXSTYLE);
		SetWindowLong(m_hwnd, GWL_EXSTYLE, cur_style | WS_EX_TRANSPARENT | WS_EX_LAYERED); //set window click-through and overlay
		SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //Keep the window always on top
		if (res != S_OK) return false;

		const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Red);
		res = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		if (res != S_OK) return false;

		// Create a DirectWrite factory.
		res = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pWriteFactory), reinterpret_cast<IUnknown **>(&pWriteFactory)); 
		if (res != S_OK) return false;

		// Create a DirectWrite text format object.
		res = pWriteFactory->CreateTextFormat(L"Calibri", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14, L"", &pTextFormat);
		if (res != S_OK) return false;

		// Center the text horizontally and vertically.
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);
		EndPaint(m_hwnd, &ps);

		return true;
	}

	void DrawRect(float left, float top, float right, float bottom, float thickness)
	{
		pRenderTarget->DrawRectangle(D2D1::RectF(left, top, right, bottom), pBrush,  thickness, 0);
	}

	void DrawSnapLines(float x0, float y0, float x1, float y1, float thickness)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(x0, y0), D2D1::Point2F(x1, y1), pBrush, thickness, 0);
	}

	void SetBrushColor(int pTeam, int botTeam)
	{
		if (pTeam == botTeam)
		{pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));}
		else
		{pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));}
	}

	void DrawEspText(wchar_t *name, float left, float top, float right, float bottom)
	{
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		pRenderTarget->DrawTextW(name, wcslen(name), pTextFormat, D2D1::RectF(left, top, right, bottom), pBrush);
	}

};


