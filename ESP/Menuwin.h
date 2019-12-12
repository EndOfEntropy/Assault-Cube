#pragma once
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static const wchar_t *title = TEXT("Check Box");

enum Id
{
	ID_CHKBOX1,
	ID_CHKBOX2,
	ID_CHKBOX3,
	ID_CHKBOX4,
	ID_CHKBOX5,
	ID_MAX_VALUE
};


class BaseMenu
{
public:

	BOOL Create(HINSTANCE hInstance)
	{
		WNDCLASS wc = { 0 };
		wc.lpszClassName = TEXT("Check Box");
		wc.hInstance = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpfnWndProc = WindowProc;
		wc.hCursor = LoadCursor(0, IDC_ARROW);

		RegisterClass(&wc);
		m_hwnd = CreateWindow(wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 150, 150, 200, 300, 0, 0, hInstance, 0);

		m_hwndB1 = CreateWindow(TEXT("button"), TEXT("Draw ESP"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, 20, 20, 185, 20, m_hwnd, (HMENU)ID_CHKBOX1, NULL, NULL);
		CheckDlgButton(m_hwndB1, ID_CHKBOX1, BST_UNCHECKED);

		m_hwndB2 = CreateWindow(TEXT("button"), TEXT("Draw Teammates"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, 20, 40, 185, 20, m_hwnd, (HMENU)ID_CHKBOX2, NULL, NULL);
		CheckDlgButton(m_hwndB2, ID_CHKBOX2, BST_UNCHECKED);

		m_hwndB3 = CreateWindow(TEXT("button"), TEXT("Draw Snaplines"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, 20, 60, 185, 20, m_hwnd, (HMENU)ID_CHKBOX3, NULL, NULL);
		CheckDlgButton(m_hwndB3, ID_CHKBOX3, BST_UNCHECKED);

		m_hwndB4 = CreateWindow(TEXT("button"), TEXT("Draw player names"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, 20, 80, 185, 20, m_hwnd, (HMENU)ID_CHKBOX4, NULL, NULL);
		CheckDlgButton(m_hwndB4, ID_CHKBOX4, BST_UNCHECKED);

		m_hwndB5 = CreateWindow(TEXT("button"), TEXT("Draw player health"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, 20, 100, 185, 20, m_hwnd, (HMENU)ID_CHKBOX5, NULL, NULL);
		CheckDlgButton(m_hwndB5, ID_CHKBOX5, BST_UNCHECKED);

		return (m_hwnd ? TRUE : FALSE);

	}

	HWND Window() const { return m_hwnd; }

protected:

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	HWND m_hwnd;
	HWND m_hwndB1;
	HWND m_hwndB2;
	HWND m_hwndB3;
	HWND m_hwndB4;
	HWND m_hwndB5;
};


class MenuWindow : public BaseMenu //currently WindowProc handles BaseMenu events, thus HandleMessage is unusable
{
public:
	ID2D1Factory			*pFactory;
	ID2D1HwndRenderTarget   *pRenderTarget;
	ID2D1SolidColorBrush	*pBrush;


	MenuWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
	{
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void BeginDraw() { pRenderTarget->BeginDraw(); }
	void EndDraw() { pRenderTarget->EndDraw(); }
	void ClearScreen() { pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DimGray)); } //Clear screen

	void DiscardGraphicsResources()
	{
		SafeRelease(&pRenderTarget);
		SafeRelease(&pBrush);
	}

	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	bool Graphics() //this can broken down in function to be more modular
	{
		HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		if (res != S_OK) return false;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		res = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);
		if (res != S_OK) return false;

		const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::DimGray);
		res = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		if (res != S_OK) return false;

		return true;
	}

	void SetMenu()
	{
		PAINTSTRUCT ps;
		BeginPaint(MenuWindow::m_hwnd, &ps);
		BeginDraw();
		ClearScreen();
		EndDraw();
		EndPaint(MenuWindow::m_hwnd, &ps);
	}

	void SetText() //Only works inside WM_PAINT as its GDI.
	{
		LPCWSTR message = L"Hello\r\nWorld";
		RECT rect;
		HDC wdc = GetWindowDC(MenuWindow::m_hwnd);
		GetClientRect(MenuWindow::m_hwnd, &rect);
		SetTextColor(wdc, RGB(255, 0, 0));
		SetBkMode(wdc, TRANSPARENT);
		rect.left = 10;
		rect.top = 10;
		DrawText(wdc, message, -1, &rect, DT_NOCLIP);
		ReleaseDC(MenuWindow::m_hwnd, wdc);
	}

};