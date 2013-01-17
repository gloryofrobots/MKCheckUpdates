#include "WinApiView.h"
#include "UpdatesModel.h"
#include "GuiComponents.h"
#include "CommCtrl.h"
#include <WindowsX.h>

#define ID_BUTTON_CLOSE 1
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

//////////////////////////////////////////////////////////////////
static void s_createLabel(ControlFieldStatic& labelName, HWND hWnd, const char * txt , int x ,int y, int width ,int height)
{
	labelName.setPosition(x , y , width ,height);
	labelName.setWindowAttr( "", WS_CHILD | WS_VISIBLE | WS_OVERLAPPED , hWnd, NULL, NULL );
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

	m_hWnd = CreateWindow( wc.lpszClassName
		, "Evolution updates"
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

	int width = 150;
	int height = 25;

	int widthInfo = 200;
	int heightInfo = 20;
	
	ControlFieldStatic labelName;
	s_createLabel(labelName, m_hWnd, "Name", x, y, widthInfo, height);
	labelName.changeColor( RGB(255, 255, 255) );
	x += widthInfo + offsetX;

	ControlFieldStatic labelStatus;
	s_createLabel(labelStatus, m_hWnd, "Status", x, y, width, height);
	labelStatus.changeColor( RGB(30, 50, 60) );
	
	y += heightInfo + offsetY;
	x = leftIndent;
	
	for( TVectorUpdatesInfo::const_iterator
		it = rows.begin(),
		it_end = rows.end();
	it != it_end;
	it++ )
	{
		const UpdatesInfo& row = *(it);
		
		char * status;

		if( row.exist == false)
		{
			status = "does not exist";
		} 
		else
		{
			status = "exist";
		}

		ControlFieldStatic rowView;
		s_createLabel( rowView, m_hWnd, row.name.c_str(), x, y, widthInfo, heightInfo );
		
		x += widthInfo + offsetX;
		s_createLabel( rowView, m_hWnd, status, x, y, widthInfo, heightInfo );
		
		y += heightInfo + offsetY;
		x = leftIndent;
	}

	x = ( WINDOW_WIDTH / 2 ) - ( width / 2 ) - leftIndent;
	y += heightInfo + offsetY;

	ControlFieldButton buttonClose;
	buttonClose.setPosition( x, y, width, height );
	buttonClose.setWindowAttr( "Close" ,WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE ,m_hWnd ,(HMENU)ID_BUTTON_CLOSE, NULL	);
	buttonClose.createWindow();

	x += width + offsetX;
	y += heightInfo + offsetY;

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