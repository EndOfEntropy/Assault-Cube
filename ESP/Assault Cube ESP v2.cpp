#include "stdafx.h"
#include <d2d1.h>
#include "Basewin.h"
#include "Menuwin.h"
#include "guicon.h"
#include "Bypass.h"
#include "Offsets.h"
#include <string>
#include "Matrix.h"
#pragma comment(lib, "d2d1")

using namespace std;

//Forward declarations
MenuWindow menu;
wchar_t* GetWC(const char *c);
wchar_t* WstringToWchar(const std::wstring &ws);
wchar_t* wStr;

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
	ScreenSettings ss;
	PlayerList pl;
	Game g;
	Player p;
	Player bot;
	ViewMatrix cam;

	//Read screen settings to get resolution
	bypass.Read(ss.convertScreenSettingsPtr(ss.GetInstance()), &ss, sizeof(ss), 0);
	bypass.Read(g.convertGamePtr(g.GetInstance()), &g, sizeof(g), 0);

	//Creates a window
	MainWindow win;

	if (!win.Create(L"ESP", WS_EX_TOPMOST | WS_POPUP, 0, 0, 0, ss.m_Width, ss.m_Height)) //Arg#2: dwStyle ; Arg#3: dwExStyle  
	{return 0;}
	//if (!menu.Create(hInstance))
	//{return 0;}
	
	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //hwnd set to NULL so both window messages and thread messages are processed.
		{
			DispatchMessage(&msg);
		}
		else
		{
			//Update in-game data
			bypass.Read(g.convertGamePtr(g.GetInstance()), &g, sizeof(g), 0);
			uintptr_t m_LocalPlayerPtr = { g.convertPlayerPtr(g.m_LocalPlayer) };
			uintptr_t m_OtherPlayersPtr = { g.convertPlayerListPtr(g.m_OtherPlayers) };
			bypass.Read(m_OtherPlayersPtr, &pl, sizeof(pl), 0);
			bypass.Read(m_LocalPlayerPtr + 0x0004, &p, sizeof(p), 0);

			win.BeginDraw();
			win.ClearScreen(); //Clear screen
			win.EndDraw();

			for (int i = 1; i < g.m_PlayerCount; ++i)
			{
				//Loop through all the entities
				bypass.Read(pl.convertPlayerPtr(pl.GetPlayer(i)) + 0x0004, &bot, sizeof(bot), 0);
				bypass.Read(cam.convertViewMatrixPtr(cam.GetInstance()), &cam, sizeof(cam), 0);
				if (bot.m_Health <= 0) //if player is dead go to the next
				{continue;}

				//Variable conversions
				int fDist = roundf(GetDistance(p.m_HeadPos, bot.m_HeadPos));
				wstring hp = { to_wstring(bot.m_Health) };
				wchar_t* wcsHp = WstringToWchar(hp);
				wstring dist = { to_wstring(fDist) + L"m" };
				wchar_t* wcsDist = WstringToWchar(dist);

				//WorldToScreen
				Vec3 drawHead = {};
				Vec3 drawFoot = {};
				if (WorldToScreen(bot.m_HeadPos, drawHead, cam, ss.m_Width, ss.m_Height))
				{
					WorldToScreen(bot.m_FootPos, drawFoot, cam, ss.m_Width, ss.m_Height);
					
					//Draw ESP
					win.BeginDraw();
					win.SetBrushColor(p.m_Team, bot.m_Team);
					win.DrawRect(drawHead.m_xPos - 30.0f, drawHead.m_yPos, drawHead.m_xPos + 30.0f, drawFoot.m_yPos, 1.0f);
					win.DrawSnapLines(static_cast<float>(ss.m_Width / 2), static_cast<float>(ss.m_Height), drawFoot.m_xPos, drawFoot.m_yPos, 1.0f);
					win.DrawEspText(GetWC(bot.m_Name), drawHead.m_xPos - 30.0f, drawHead.m_yPos - 20.0f, drawHead.m_xPos + 30.0f, drawHead.m_yPos - 10.0f);
					win.DrawEspText(wcsHp, drawFoot.m_xPos - 15.0f, drawFoot.m_yPos + 10.0f, drawFoot.m_xPos + 15.0f, drawFoot.m_yPos + 20.0f);
					win.DrawEspText(wcsDist, drawHead.m_xPos + 35.0f, drawHead.m_yPos, drawHead.m_xPos + 75.0f, drawHead.m_yPos);
					win.EndDraw();

					delete wStr;
				}
			}
		}
	}

	return 0;
}

wchar_t* GetWC(const char *c)
{
	size_t outSize;
	const size_t cSize = strlen(c) + 1;
	wchar_t* wStr = new wchar_t[cSize];
	mbstowcs_s(&outSize, wStr, cSize, c, cSize - 1);

	return wStr;
}

wchar_t* WstringToWchar(const std::wstring &ws)
{
	return const_cast<wchar_t*>(ws.c_str());
}


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		if (!Graphics())
		{
			DiscardGraphicsResources();
			return -1;
		};
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_INSERT:
			if (IsWindowVisible(menu.Window()) == 0)
			{
				ShowWindow(menu.Window(), SW_SHOW);
			}
			else
			{
				ShowWindow(menu.Window(), SW_HIDE);
			}
		}
		return 0;
	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_COMMAND:
	{

		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LRESULT MenuWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) //currently WindowProc handles BaseMenu events, thus HandleMessage is unusable
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (!Graphics())
		{
			DiscardGraphicsResources();
			SafeRelease(&pFactory);
			return -1;
		};
		return 0;

	case WM_PAINT:
		SetText();
		return 0;
	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
