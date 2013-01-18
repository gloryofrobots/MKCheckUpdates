#include "WinApiView.h"
#include "UpdatesModel.h"
#include "GuiComponents.h"
#include "CommCtrl.h"
#include "resource.h"
#include <WindowsX.h>

#define ID_BUTTON_CLOSE 1
#define ID_STATIC_NAME 2
#define ID_STATIC_STATUS 3

#define ID_STATIC_EXIST 4
#define ID_STATIC_NOT_EXIST 5

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

//////////////////////////////////////////////////////////////////
static void s_createLabel(ControlFieldStatic& labelName, HWND hWnd, const char * txt , int x ,int y, int width ,int height, HMENU hMenu = NULL )
{
	labelName.setPosition(x , y , width ,height);
	labelName.setWindowAttr( "", WS_CHILD | WS_VISIBLE | WS_OVERLAPPED , hWnd, hMenu, NULL );
	labelName.createWindow();
	labelName.setText( txt );
}

static WinApiView * GuiInstance_S = 0;

long MainWndProc(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam)
{
	if( GuiInstance_S == 0  )
	{
		return (long)DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return GuiInstance_S->windowProc(hWnd, Msg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////
WinApiView::WinApiView( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PWSTR _pCmdLine, int _nCmdShow )
	: m_instance(_hInstance)
	, m_hWnd(NULL)
	, m_countRows(0)
{
}
//////////////////////////////////////////////////////////////
WinApiView::~WinApiView(void)
{
}
//////////////////////////////////////////////////////////////
bool WinApiView::init()
{
	if( this->_initWindow() == false )
	{
		return false;
	} 

	GuiInstance_S = this;
	return true;
}
//////////////////////////////////////////////////////////////
bool WinApiView::_initWindow()
{ 
	if( m_hWnd != NULL )
	{
		return false;
	}

	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW | WS_OVERLAPPED /*| WS_HSCROLL | WS_VSCROLL*/;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.lpszClassName = "Window";
	wc.hInstance     = m_instance;
	//(HBRUSH)(COLOR_WINDOW+1);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc   = (WNDPROC)MainWndProc;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

	m_wndClassId = RegisterClass(&wc);

	if ( m_wndClassId == NULL )
	{
		return false;
	}

	String title = this->_loadStringResource( IDS_STRING_TITLE );
	m_hWnd = CreateWindow( wc.lpszClassName
		, title.c_str()
		, WS_OVERLAPPEDWINDOW | WS_VISIBLE
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, WINDOW_WIDTH
		, WINDOW_HEIGHT
		, NULL
		, NULL
		, m_instance
		, NULL);  
	
	if( m_hWnd == NULL )
	{
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////
bool WinApiView::show( const CheckUpdatesModel * _model )
{
	if ( m_hWnd == NULL )
	{
		return false;
	}

	ShowWindow( m_hWnd, SW_SHOW );

	const TVectorUpdatesInfo & rows = _model->getRows();
	int count = rows.size();
	
	if( count == 0 )
	{
		return false;
	}
	
	int leftIndent = 10;
	int rightIndent = 10;
	int topIndent = 10;
	int offsetX = 5;
	int offsetY = 5;
	int x = leftIndent;
	int y = topIndent;

	int offsetXWidth = offsetX * count;

	int width = 175;
	int height = 25;

	int widthInfo = 175;
	int heightInfo = 20;
	
	//////////////////////////////////////////////////////////////////////
	ControlFieldStatic labelName;
	String nameTxt = this->_loadStringResource( IDS_STRING_NAME );
	s_createLabel(labelName, m_hWnd, nameTxt.c_str(), x, y, widthInfo, height, (HMENU)ID_STATIC_NAME );
	
	//////////////////////////////////////////////////////////////////////
	x += widthInfo + offsetX;
	ControlFieldStatic labelStatus;
	String statusTxt = this->_loadStringResource( IDS_STRING_STATUS );
	s_createLabel(labelStatus, m_hWnd, statusTxt.c_str(), x, y, width, height, (HMENU)ID_STATIC_STATUS);
	
	//////////////////////////////////////////////////////////////////////
	y += heightInfo + offsetY + offsetY;
	x = leftIndent;
	String existTxt = this->_loadStringResource( IDS_STRING_EXIST );
	String notExistTxt = this->_loadStringResource( IDS_STRING_NOT_EXIST );

	for( TVectorUpdatesInfo::const_iterator
		it = rows.begin(),
		it_end = rows.end();
	it != it_end;
	it++ )
	{
		const UpdatesInfo& row = *(it);
		ControlFieldStatic rowView;
		
		String * status = NULL;
		HMENU wParam;
		if( row.exist == false)
		{
			status = &notExistTxt;
			wParam = (HMENU) ID_STATIC_NOT_EXIST;
		} 
		else
		{
			status = &existTxt;
			wParam = (HMENU) ID_STATIC_EXIST;
		}

		s_createLabel( rowView, m_hWnd, row.name.c_str(), x, y, widthInfo, heightInfo, wParam );
		
		x += widthInfo + offsetX;
		
		s_createLabel( rowView, m_hWnd, status->c_str(), x, y, widthInfo, heightInfo, wParam );
		y += heightInfo + offsetY;
		x = leftIndent;
	}
	
	//////////////////////////////////////////////////////////////////////
	x = ( WINDOW_WIDTH / 2 ) - ( width / 2 ) - leftIndent;
	
	RECT rect = this->_getWindowsRect();
	size_t buttonY = rect.bottom - heightInfo - offsetY - height;
	
	if( y < buttonY )
	{
		y = buttonY;
	}
	else
	{
		y += heightInfo + offsetY;
	}
	
	String closeTxt = this->_loadStringResource( IDS_STRING_CLOSE );
	ControlFieldButton buttonClose;
	buttonClose.setPosition( x, y, width, height );
	buttonClose.setWindowAttr( closeTxt.c_str() ,WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE ,m_hWnd ,(HMENU)ID_BUTTON_CLOSE, NULL	);
	buttonClose.createWindow();

	y += heightInfo + offsetY +height;
	//////////////////////////////////////////////////////////////////////
	SD_OnInitDialog( m_hWnd, WINDOW_WIDTH, y, WINDOW_HEIGHT );
	ScrollWindow( m_hWnd, 0, 0, NULL, NULL );
	
	MSG msg;
	while ( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg ); 
		DispatchMessage( &msg ); 
	} 

	return true;
}

HBRUSH hBrush_S = CreateSolidBrush(RGB(230,230,230));

//////////////////////////////////////////////////////////////
long WinApiView::windowProc(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam)
{
	if(hWnd != m_hWnd)
	{
		return (long)DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	switch (Msg)
	{
	case WM_CREATE:
		return 0;
		break;
	case WM_DESTROY:
		//UnregisterClass( m_wndClassName,NULL );
		m_hWnd = NULL;
		PostQuitMessage(0);
		return 0;
		break;
	case WM_NCDESTROY:
		return 0;
		break;
	case WM_CTLCOLORSTATIC:
		{
			DWORD CtrlID = GetDlgCtrlID((HWND)lParam); //Window Control ID
			if ( CtrlID == ID_STATIC_NAME ||  CtrlID == ID_STATIC_STATUS ) //If desired control
			{
				HDC hdcStatic = (HDC) wParam;
				SetTextColor(hdcStatic, RGB(0,0,0));
				SetBkColor(hdcStatic, RGB(230,230,230));
				return (INT_PTR)hBrush_S;
			}
			else if ( CtrlID == ID_STATIC_NOT_EXIST ) //If desired control
			{
				HDC hdcStatic = (HDC) wParam;
				SetBkMode(hdcStatic,TRANSPARENT);
				SetTextColor(hdcStatic, RGB(255,0,0));
				//COLORREF old = GetBkColor(hdcStatic);
				//SetBkColor(hdcStatic, old);
				return (INT_PTR)hBrush_S;
			}
			else if ( CtrlID == ID_STATIC_EXIST ) //If desired control
			{
				HDC hdcStatic = (HDC) wParam;
				SetBkMode(hdcStatic,TRANSPARENT);
				SetTextColor(hdcStatic, RGB(0,100,0));
				//COLORREF old = GetBkColor(hdcStatic);
				//SetBkColor(hdcStatic, old);
				return (INT_PTR)hBrush_S;
			}
		}
	case WM_COMMAND:
		if(wParam == ID_BUTTON_CLOSE)
		{ 
			//MessageBox(NULL,"You pressed my button OK","MessageBox",MB_OK|MB_ICONWARNING);	
			clear();
		}
		
		return 0;
		break;
		
		/*HANDLE_MSG(m_hWnd, WM_INITDIALOG, SD_OnInitDialog);*/
		HANDLE_MSG(m_hWnd, WM_SIZE, SD_OnSize);
		HANDLE_MSG(m_hWnd, WM_HSCROLL, SD_OnHScroll);
		HANDLE_MSG(m_hWnd, WM_VSCROLL, SD_OnVScroll);
	}
	
	return (long)DefWindowProc(hWnd, Msg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////
void WinApiView::clear()
{
	if(m_hWnd == NULL)
	{
		return;
	}

	DestroyWindow(m_hWnd);
}
//////////////////////////////////////////////////////////////////
String WinApiView::_loadStringResource( UINT _id )
{
	char buffer[255] = {'\0'};

	LoadString(m_instance, _id, buffer, 255);
	return String(buffer);
}
//////////////////////////////////////////////////////////////////
RECT WinApiView::_getWindowsRect()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	return rc;
}
//////////////////////////////////////////////////////////////////