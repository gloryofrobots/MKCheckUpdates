#pragma once

#include <Windows.h>

class CheckUpdatesModel;

class WinApiView
{
public:
	WinApiView( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PWSTR _pCmdLine, int _nCmdShow );
	~WinApiView(void);
	
	bool init();
	bool show( const CheckUpdatesModel * _model );
	void clear();
	long windowProc(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam);
protected:
	bool _initWindow();
	HWND m_hWnd;

	ATOM m_wndClassId;
	LPCSTR m_wndClassName;

	HINSTANCE m_instance;
};

